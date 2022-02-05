#include "alist.h"
int count(char *ptr,char subj,char end){
	int ans=0;
	while(*ptr != end){
		if(ptr[0] == subj)ans++;
		ptr++;
	}	
	return ans;
}
int string_length(char *ptr,char end){
	int ans = 0;
	while(*ptr != end){
		ans++;
		ptr++;
	}
	return ans++;
}

struct Item * string2queue(char *ptr,char separator,char end){
	struct Item *first = create_item(-1);
	struct Item *cur = first;
	int len=strlen(ptr);
	int n=0;
	while(1){
		if(ptr[0] == separator){
			ptr++;n++;
			continue;
		}
		if(ptr[0] == end || n>=len){
			break;
		}
		cur->next = create_item(-1);
		cur = cur->next;
		cur->val = atoi(ptr);
		while(ptr[0] != separator && ptr[0] != end ){
			ptr++;n++;
		}
	}
	return destroy_first(first);
}



void read_alist(char *fname){//,struct Item * var_connect,struct Item* intleav,struct Item* check_connect){
	//int size_i;int size_v;int size_c;
	
	//int var_node_num = 0;
	//int check_node_num = 0;	
	struct Item* main_var_connect = create_item(0);
	struct Item* ptr_var_connect = main_var_connect;
	struct Item* main_intleav = create_item(0);
	struct Item* ptr_intleav = main_intleav;
	struct Item* main_check_connect = create_item(0);
	struct Item* ptr_check_connect = main_check_connect;
	struct Item* ptr = NULL;
	struct Item* main_queue = create_item(0);
	struct Item* ptr_main_queue = main_queue;
	struct Item** main_check_nodes;
	struct Item* ptr_main_check_nodes;
	int ql = 0;
	int length = -1;
	char *str1;
	char *str2;
	char separator;
	FILE *fp = fopen(fname,"r");
	int counter=0;
	int var_counter = 0;
	int tmp,tmp1,tmp2;
	int sum=0;
	int i,j;
	int variable_nodes_number,check_nodes_number;
	int intlv_size;
	int err,size;
	if(fp==NULL){
		printf("File could not de opened\n");
		return;
	}
	//length =_filelength(_fileno(fp));
	//-----
	err = fseek(fp, 0, SEEK_END);
        if(err != 0) {
         printf("Error seeking to end of file\n");
         exit(-1);
        }

        size = ftell(fp);
        if(size < 0) {
         printf("Error getting file position\n");
         exit(-1);
        }

       err = fseek(fp, 0, SEEK_SET);
       if(err != 0) {
        printf("Error seeking to start of file\n");
        exit(-1);
       }
	length=size+1;
	//------
	str1 = (char *)malloc(length);
	while(str2=fgets(str1,length,fp)){
		tmp = string_length(str2,'\n');
		tmp1 = count(str2,' ','\n');
		tmp2 = count(str2,'\t','\n');
		if(tmp1)separator = ' ';
		if(tmp2)separator = '\t';
		//printf("%d:		%s",counter,str2);
		counter++;
		
		ptr = string2queue(str2,separator,'\n');
			
		/*
		while(ptr){
			printf("%d \n",ptr->val);
			ptr = ptr->next;
		}
		*/
		
		ql = queue_length(ptr);
		if(ql == 0){
			ql=0;
		}
		main_queue->val++;
		ptr_main_queue->next = create_item(ql);
		ptr_main_queue = ptr_main_queue->next;
		ptr_main_queue->p = ptr;
		//printf("----------  %d\n",ql);
		//destroy_queue(ptr);
	}
	printf("Number strings: %d\n",main_queue->val);
	ptr_main_queue = main_queue;
	//getch();
	tmp=0;
	for(i=0;i<main_queue->val;i++){
		ptr = (struct Item *)ptr_main_queue->next->p;
		//printf("%d: :%d: ",i,ptr_main_queue->next->val);
		sum=0;
		var_counter=0;
		
		while(ptr){
			printf("%d, ",ptr->val);
			sum += ptr->val;			
			if(i==1){
				/*if(ptr->val!=2)*/for(j=0;j<ptr->val;j++){
					ptr_var_connect->next = create_item(counter);
					ptr_var_connect = ptr_var_connect->next;
				}
				var_counter++;
			}
			if(i>1){
				main_check_nodes[ptr->val]->p = create_item(tmp);
				main_check_nodes[ptr->val]->val++;
				ptr_main_check_nodes = main_check_nodes[ptr->val];
				while(ptr_main_check_nodes->next)ptr_main_check_nodes=ptr_main_check_nodes->next;
				ptr_main_check_nodes->next = main_check_nodes[ptr->val]->p;
				tmp++;
			}
			ptr=ptr->next;
		}
		//printf("--- %d \n",sum);
		printf("\n");
		
		if(i==0){
			ptr = ptr_main_queue->next->p;
			variable_nodes_number = ptr->val;
			check_nodes_number = ptr->next->val;
			main_check_nodes = (struct Item **)malloc(sizeof(struct Item *)*check_nodes_number);
			for(j=0;j<check_nodes_number;j++){
				main_check_nodes[j] = create_item(0);
				main_check_nodes[j]->p = NULL;
			}
		}
		if(i==1){
			intlv_size = sum;
		}
		ptr_main_queue = ptr_main_queue->next;
	}
	
	free(str1);
	ptr_main_queue = main_queue;
	for(i=0;i<main_queue->val;i++){
		ptr = (struct Item *)ptr_main_queue->next->p;
		destroy_queue(ptr);
		ptr_main_queue = ptr_main_queue->next;
	}
	main_var_connect = destroy_first(main_var_connect);
	ptr_var_connect = main_var_connect;
	printf("------------------------------------------------\n");
	//getch();
	/*
	while(ptr_var_connect){
		printf("%d ",ptr_var_connect->val);
		ptr_var_connect = ptr_var_connect->next;
	}
	*/
	printf("%d\n",queue_length(main_var_connect));
	destroy_queue(main_queue);
	fclose(fp);
	destroy_queue(main_var_connect);
	destroy_queue(main_check_connect);
	destroy_queue(main_intleav);
}

//! Read data from file in alist format
/*!
	\param fname alist format file name
	\return pointer to the first element in queue
*/
struct Item* read_alist2(char *fname){
	struct Item* ptr = NULL;
	struct Item* main_queue = create_item(0);
	struct Item* ptr_main_queue = main_queue;
	int ql = 0;
	int length = -1;
	char *str1;
	char *str2;
	char separator;
	int counter;
	int tmp,tmp1,tmp2;
	int err,size;
	//int i;
	FILE *fp = fopen(fname,"r");
	
	if(fp==NULL){
		printf("File could not de opened\n");
		return NULL;
	}
	//length =_filelength(_fileno(fp));
	//-----
	err = fseek(fp, 0, SEEK_END);
        if(err != 0) {
         printf("Error seeking to end of file\n");
         exit(-1);
        }

        size = ftell(fp);
        if(size < 0) {
         printf("Error getting file position\n");
         exit(-1);
        }

       err = fseek(fp, 0, SEEK_SET);
       if(err != 0) {
        printf("Error seeking to start of file\n");
        exit(-1);
       }
	length=size+1;
	//------
	str1 = (char *)malloc(length);
	counter = 0;
	while(str2=fgets(str1,length,fp)){
		tmp = string_length(str2,'\n');
		tmp1 = count(str2,' ','\n');
		tmp2 = count(str2,'\t','\n');
		if(tmp1)separator = ' ';
		if(tmp2)separator = '\t';
		ptr = string2queue(str2,separator,'\n');
		printf("%d:%d		%s\n",counter,strlen(str2),str2);
		//getch();
		
		
		
		ql = queue_length(ptr);
		/*
		if(ql == 2){
			printf("%d %d ^&&*\n",counter,ql);
		}
		*/
		if(ql>0){
			counter++;
			main_queue->val++;
			//printf("ql: %d\n",ql);
			ptr_main_queue->next = create_item(ql);
			ptr_main_queue = ptr_main_queue->next;
			ptr_main_queue->p = ptr;
		}
	}
	printf("Number strings: %d\n",main_queue->val);
	free(str1);
	
	
	fclose(fp);
	return main_queue;
}
//! destroy queue
/*!
	\param main_queue data conteiner	
*/
void destroy_main_queue(struct Item* main_queue){
	int i;
	struct Item *ptr_main_queue = main_queue;
	struct Item *ptr;
	for(i=0;i<main_queue->val;i++){
		ptr = (struct Item *)ptr_main_queue->next->p;
		destroy_queue(ptr);
		ptr_main_queue = ptr_main_queue->next;
	}	
	destroy_queue(main_queue);
}
int is_anyone_equal(int number,struct Item* ptr){
	while(ptr){
		if(ptr->val == number)return 1;
		ptr=ptr->next;
	}
	return 0;
}

//! convert data from queue to bipartite graph representation
/*!
	\param main_queue data conteiner
	\param B destination conteiner
	\param permute_zig_zag flag of aditional permutation
	
*/
void make_encoder_queues(struct Item* main_queue,struct Bipartite* B,int permute_zig_zag,int preved_from_matlab){
	struct Item* ptr = main_queue->next->next->next;
	struct Item* ptr1;
	struct Item* ptr2;

	

	int tmp;

	struct Item* irregular_repeat = create_item(0);
	struct Item* ptr_irregular_repeat = irregular_repeat;

	
	struct Item* enc_var_connect = create_item(0);
	struct Item* ptr_enc_var_connect  = enc_var_connect;

	struct Item* enc_check_connect = create_item(0);
	struct Item* ptr_enc_check_connect  = enc_check_connect;

	struct Item* enc_intleav = create_item(0);
	struct Item* ptr_enc_intleav  = enc_intleav;


	int i,k,k1;
	int check_nodes_number = main_queue->next->p->next->val;
	struct Item** check_nodes = (struct Item**)malloc(check_nodes_number*(sizeof(struct Item*)) );
	int * intrlvr = (int *)malloc(check_nodes_number*sizeof(int));
	generate_standart_interleaver(intrlvr,permute_zig_zag,check_nodes_number);
	B->check_node_num = check_nodes_number;
	for(i=0;i<check_nodes_number;i++){
		check_nodes[i] = create_item(0);
	}
	k=0;
	k1=0;
	
	while(ptr){	
		if (ptr->val<2){
			tmp=0;
		}
		if(ptr->val >= 2){
			ptr1=ptr->p;
			tmp=0;
			while(ptr1){
				int ptr1_val = ptr1->val - preved_from_matlab;

				ptr2 = check_nodes[ptr1_val]->p;
				if(ptr2 == NULL){
					ptr2 = create_item(k1);
					check_nodes[ptr1_val]->val++;
					check_nodes[ptr1_val]->p = ptr2;
				}else{
					while(ptr2->next)ptr2 = ptr2->next;
					ptr2->next = create_item(k1);
					check_nodes[ptr1_val]->val++;
				}

				ptr_enc_var_connect->next = create_item(k);
				tmp++;
				ptr_enc_var_connect = ptr_enc_var_connect->next;

				
				ptr1=ptr1->next;
				k1++;
			}
			ptr_irregular_repeat->next = create_item(tmp);
			ptr_irregular_repeat = ptr_irregular_repeat->next;
			k++;
		}
		ptr=ptr->next;
	}
	B->enc_intleav_size = k1;
	B->info_len = k;
	enc_var_connect = destroy_first(enc_var_connect);
	irregular_repeat = destroy_first(irregular_repeat);
	k=0;
	k1=0;

	for(i=0;i<check_nodes_number;i++){
		ptr = check_nodes[intrlvr[i]]->p;
		while(ptr){

			ptr_enc_check_connect->next = create_item(k);
			ptr_enc_check_connect = ptr_enc_check_connect->next;

			ptr_enc_intleav->next = create_item(ptr->val);
			ptr_enc_intleav = ptr_enc_intleav->next;

			ptr = ptr->next;
			k1++;
		}
		k++;
	}
	enc_check_connect = destroy_first(enc_check_connect);
	enc_intleav = destroy_first(enc_intleav);
	B->check_connect = enc_check_connect;
	B->intleav = enc_intleav;
	B->var_connect = enc_var_connect;
	B->irregular_repeat = irregular_repeat;
	
	for(i=0;i<check_nodes_number;i++){
		destroy_queue(check_nodes[i]->p);
		destroy_queue(check_nodes[i]);
	}

	free(check_nodes);
	free(intrlvr);
	
	
}





