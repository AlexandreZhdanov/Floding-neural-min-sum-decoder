#include "interleaver.h"
/*!
	\param int *dst destination array
	\param int *src source array
	\param int len interleaver length
	\return *dst
*/
int * make_deintleav(int *dst,int *src,int len){
	int i;
	memset(dst,0,sizeof(int)*len);
	for(i=0;i<len;i++){
		dst[src[i]]++;
	}
	for(i=0;i<len;i++){
		if(dst[i] != 1){
			printf("intleav_error %d %d\n",dst[i],i);
		}
	}
	memset(dst,-1111,sizeof(int)*len);
	for(i=0;i<len;i++){
		if(src[i]<0 || src[i] >= len){
			printf("intleav_error out of bond");
		}
		dst[src[i]]=i;
	}
	return dst;
}
/*!
	\param int *intleav destination array
	\param int len interleaver length
	\return *intleav
*/
int * dumb_interleaver(int * intleav,int length){
	int i;
	for(i=0;i<length;i++){
		intleav[i] = i;
	}
	return intleav;
}
/*!
	\param int * ptr destination array
	\param int NumRow number of rows
	\param int frame_length
*/
void generate_standart_interleaver(int * ptr, int NumRow, int frame_length){
	int i,j;
	int counter = 0;
	for(i = 0;i < NumRow;i++){
		for(j = i;j < frame_length;j+=NumRow){
			ptr[counter++] = j;
		}
	}
}
/*!
	make a shift like ptr[i] = ptr[i-1] inside specified region
	\param int *ptr pointer to the interleaver
	\param int start_index initial shift positions
	\param int stop_index final shift positions
*/
void inner_recyclic_shift(int *ptr, int start_index, int stop_index){
	int i;
	int save;
	save = ptr[stop_index];
	for(i=stop_index;i > start_index;i--){
		ptr[i] = ptr[i-1];
	}
	ptr[start_index]=save;
}
/*!
	divide interleaver into t regions and performed inner resycle shift within regoins
	\param int *ptr pointer to interleaver
	\param int t number regions
	\param int len  interleaver length
*/
void recyclic_shift(int *ptr,int t,int len ){
	int i,j;
	int r,tmp;
	r = len%t;
	tmp=len/t;
	
	if(r<tmp){
		t++;
		for(i=0,j=0;j<r;j++){
			inner_recyclic_shift(ptr, i, i+t-1);
			i+=t;
		}
		t--;
		while(i<len){
			inner_recyclic_shift(ptr, i, i+t-1);
			i+=t;
		}
	}else{
		for(i=0,j=0;j<tmp;j++){
			inner_recyclic_shift(ptr, i, i+t-1);
			i+=t;
		}
		inner_recyclic_shift(ptr, i, len-1);
	}
}
/*!
	divide interleaver into odd and even part and performed resycle shift with specified parameters
*/
void make_rcs_interleaver_2on2(int *intlv, int t, int l){
	int l2,i,len;
	int *intlv2;
	len = l/2;
	l2 = len*2;
	intlv2 = (int *)malloc(sizeof(int)*l2);
	generate_standart_interleaver(&intlv2[len], t, len);
	for(i=0;i<len;i++){
		intlv2[2*i]= 2*intlv2[i+len];
		intlv2[2*i+1]=2*intlv2[i+len]+1;
	}
	recyclic_shift(intlv2,2*t-1,l2);
	make_deintleav(intlv,intlv2,l2);
	
	free(intlv2);
}
/*!
	\param int *dst destination path
	\param int *first pointer to first interleaver
	\param int len_f length of first interleaver
	\param int *second pointer to second interleaver atoi isdigit
*/
void combined_interleaver(int *dst, int *first,int len_f,int *second,int len_s){//dst length is equal len_f*len_s
	int i = 0;
	int j = 0;
	for(i=0;i<len_f;i++){
		for(j=0;j<len_s;j++){
			dst[i*len_s + j] = first[i]*len_s + second[j];
		}
	}
}
/*!
	\param int *dst destination path
	\param int length interleaver length
	\param int *I write dithering sequence
	\param int len_I dithering sequence length
	\param int s shift of linear permutation
	\param int p relative with length number
*/
void dithered_interleaver(int *dst,int length,const int *I, int len_I, int s, int p, char *shifts){
	int i = 0;
	int j = 0;
	int *ptr = NULL;
	int *ptr2= NULL;
	int *ptr3=NULL;
	int NumberShifts = 0;
	int StrLen = 0;
	int *shift_pos = NULL;
	int *shift_val = NULL;
	ptr = (int *)malloc(sizeof(int)*length);
	ptr2 = (int *)malloc(sizeof(int)*length);
	ptr3 = (int *)malloc(sizeof(int)*(length/len_I));
	for(i=0;i<length;i++){
		j = i%len_I;
		ptr[i] = (i-j) + I[j];
		ptr2[i]=0;
		dst[i]=ptr[i];
	}
	
	//generate_standart_interleaver(ptr2,p,length);
	for(i=0;i<length;i++){
		dst[i] = (s+ptr[i]*p)%length;
		ptr2[(s+i*p)%length]++;
	}
	//traditioanal shift of first column
	j=-1;
	for(i=0;i<length;i+=len_I){
		if(j>=0){
			int tmp = dst[j];
			dst[j]=dst[i];
			dst[i]=tmp;
		}
		j=i;	
	}
	
	if(shifts){
		int tmp1,tmp2,tmp3;
		int big_trouble_1=0;		
		StrLen = strlen(shifts);
		tmp1=0;
		tmp2=0;
		tmp3=0;
		for(i=0;i<StrLen;i++){
			if(shifts[i]=='('){
				if(!((tmp2==1)&&(tmp3==1))&&tmp1){big_trouble_1=1;}
				tmp1++;
				tmp2=tmp3=0;
			}
			if(shifts[i]==')'){tmp2++;}
			if(shifts[i]==','){tmp3++;}		
		}
		if(!big_trouble_1)NumberShifts=tmp1;
	}
	if(NumberShifts>0){
		char *tmp_ptr;
		shift_pos = (int *)malloc(sizeof(int)*NumberShifts);
		shift_val = (int *)malloc(sizeof(int)*NumberShifts);
		tmp_ptr=shifts;
		for(i=0;i<NumberShifts;i++){
			while(*tmp_ptr!='(')tmp_ptr++;
			shift_pos[i]= atoi(&tmp_ptr[1]);
			while(*tmp_ptr!=',')tmp_ptr++;
			shift_val[i]= atoi(&tmp_ptr[1]);			
		}
		
		for(i=0;i<NumberShifts;i++){
			int k,j,j_max;
			k = shift_pos[i]%length;
			j=0;
			while(k<length){
				ptr3[j]=dst[k];
				k+=len_I;j++;
			}
			k = shift_pos[i]%length;
			j_max=j;
			j=0;
			while(k<length){
				int tmp = (j+shift_val[i]+j_max)%j_max;
				while(tmp<0)tmp+=j_max;
				dst[k]=ptr3[tmp];
				k+=len_I;j++;
			}
		}
		


	}
	/*
	i=21;
	while(i<length)i+=len_I;
	i-=len_I;
	j=-1;
	for(;i>=21;i-=len_I){//for(i=21;i<length;i+=len_I){
		if(j>=0){	
			int tmp = dst[j];
			dst[j]=dst[i];
			dst[i]=tmp;
		}
		j=i;	
	}
	

	

	i=23;
	while(i<length)i+=len_I;
	i-=len_I;
	j=-1;
	for(;i>=23;i-=len_I){//for(i=23;i<length;i+=len_I){
		if(j>=0){	
			int tmp = dst[j];
			dst[j]=dst[i];
			dst[i]=tmp;
		}
		j=i;	
	}
	*/
	
	
	
	/*
	for(i=0;i<length;i++){		
		ptr2[i] = dst[ptr[i]];		
	}
	for(i=0;i<length;i++){		
		dst[i] = ptr2[i];		
	}
	*/
	
	
	make_deintleav(ptr,dst,length);
	/*
	for(i=0;i<length;i++){		
		dst[i] = ptr[i];		
	}
	*/
	
	
	//memcpy(dst,ptr2,length*sizeof(int));
	if(shift_pos){free(shift_pos);}
	if(shift_val){free(shift_val);}
	free(ptr);
	free(ptr2);
	free(ptr3);
}
