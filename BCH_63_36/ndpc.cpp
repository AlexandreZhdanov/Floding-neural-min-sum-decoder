//g++ -g2 ndpc.cpp alist.c queue.c interleaver.c wgn.c
//https://arxiv.org/pdf/1701.05931.pdf
#include <stdio.h>
#include <math.h>
#include "memcont.h"
#include "TQueue.h"
#include <stdio.h>
#include "alist.h"
#define TRAINING 1
#define CHECK 2
#define NO_WEIGHTS 4
#define MAX(a,b) (((a)>(b))?(a):(b))
#include "wgn.h"
//#include <iostream>
void teta_scaled(int *src,int len,int *dst/*,int iter,double S*/);
int RELU(int a,int b);
int sign(double v);
class CNode{
	protected:
	int Mode;
	int NodeDegree;
	int m[2];
	int m_ind[2];
	int s;
	public:
	MemoryController<int> VarNodeNumbers;
	MemoryController<int> IncomingMessages;
	MemoryController<int> OutcomingMessages;
	MemoryController<int> TMPMessages;
	MemoryController<int> Weights;
	TQueue<int> Input;
	int addVarNodeNumber(int num);
	int addMatlabVarNodeNumber(int num);
	CNode();
	virtual ~CNode();
	int configure();
	int node_degree(){return NodeDegree;}
	void set_mode(int mode){Mode=mode;}
	int run(int *ptr);
	int check(int *ptr);
	int correction(int *src,int len,int *dst,int *weights);
	void reset(int default_weight=-1);
	int training(int *ptr,int node_number=-1);
	int set_weights(int node_number,int *ptr,int val);
	void update(int *ptr,int node_number=-1);
};

CNode::CNode(){
	Mode=0;
	NodeDegree=0;
}
CNode::~CNode(){
	VarNodeNumbers.free_all();
	IncomingMessages.free_all();
	OutcomingMessages.free_all();
	TMPMessages.free_all();
	Weights.free_all();
}
int CNode::configure(){
	NodeDegree=Input.length();
	VarNodeNumbers.resize(NodeDegree);
	IncomingMessages.resize(NodeDegree);
	OutcomingMessages.resize(NodeDegree);
	TMPMessages.resize(NodeDegree);
	Weights.resize(NodeDegree*NodeDegree);
	reset();
	for(int i=0;i<NodeDegree;i++){
		VarNodeNumbers[i] = Input.remove();
	}
	return NodeDegree;
}
void CNode::reset(int default_weight){
	//memset((void *)VarNodeNumbers.getPtr(),0,sizeof(int)*NodeDegree);
	memset((void *)IncomingMessages.getPtr(),0,sizeof(int)*NodeDegree);
	memset((void *)OutcomingMessages.getPtr(),0,sizeof(int)*NodeDegree);
	memset((void *)TMPMessages.getPtr(),0,sizeof(int)*NodeDegree);
	if(default_weight>=0){
		memset((void *)Weights.getPtr(),default_weight,sizeof(int)*NodeDegree*NodeDegree);
	}
}
int CNode::addVarNodeNumber(int num){
	Input<<num;
}
int CNode::addMatlabVarNodeNumber(int num){
	Input<<(num-1);//matlab style matrices!
}
int CNode::run(int *ptr){
	int i;
	for(i=0;i<NodeDegree;i++){
		IncomingMessages[i]=ptr[VarNodeNumbers[i]];
		//printf("ru%d - %d - %d \n",IncomingMessages[i],VarNodeNumbers[i],ptr[VarNodeNumbers[i]]);
	}
	correction(IncomingMessages.getPtr(),NodeDegree,TMPMessages.getPtr(),Weights.getPtr());
	int sr=1;
	int minr=1000;
	for(i=0;i<NodeDegree;i++){
		//TMPMessages[i]=(3*TMPMessages[i])/4;
		ptr[VarNodeNumbers[i]]+=TMPMessages[i]-OutcomingMessages[i];
		OutcomingMessages[i]=TMPMessages[i];
		sr*=sign(ptr[VarNodeNumbers[i]]);
		int tmp = abs(ptr[VarNodeNumbers[i]]);
		if(tmp<minr)minr=tmp;
	}
	return sr*minr;
}
int CNode::check(int *ptr){
	s=1;
	m[0]=1000;
	
	for(int i=0;i<NodeDegree;i++){
		s*=sign(ptr[VarNodeNumbers[i]]);
		int tmp = abs(ptr[VarNodeNumbers[i]]);
		if(tmp<m[0]){
			m[0]=tmp;
			m_ind[0]=i;
		}
	}
	m[1]=1000;
	for(int i=0;i<NodeDegree;i++){
		if(i==m[0])continue;
		int tmp = abs(ptr[VarNodeNumbers[i]]);
		if(tmp<m[1]){
			m[1]=tmp;
			m_ind[1]=i;
		}
	}
	return s*m[0];
}
int CNode::training(int *ptr,int node_number){
	int sum=0;
	int i=0;
	if(node_number>-1){
		ptr[VarNodeNumbers[node_number]]=1;
	}
	//printf("In: ");
	for(int i=0;i<NodeDegree;i++){
		IncomingMessages[i]=ptr[VarNodeNumbers[i]]-OutcomingMessages[i];
		sum+=IncomingMessages[i];
		//printf("tr%d - %d - %d \n",IncomingMessages[i],VarNodeNumbers[i],ptr[VarNodeNumbers[i]]);
	}
	//printf(": %d \n",sum);
	//printf("Out: ");
	for(i=0;i<NodeDegree;i++){
		TMPMessages[i]=sum-IncomingMessages[i];
		if(node_number>=0)ptr[VarNodeNumbers[i]]+=TMPMessages[i];
		OutcomingMessages[i]=(i==node_number)?1:TMPMessages[i];
		//printf("%d ",OutcomingMessages[i]);
	}
	//printf("\n");
	return sum;
}
void CNode::update(int *ptr,int node_number){
	for(int i=0;i<NodeDegree;i++){
		if(node_number<0)ptr[VarNodeNumbers[i]]+=TMPMessages[i];
		//OutcomingMessages[i]=(i==node_number)?1:TMPMessages[i];
		//printf("%d ",OutcomingMessages[i]);
	}
}
int CNode::set_weights(int node_number,int *ptr,int val){
	int i;
	int tmp=0;
	for(i=0;i<NodeDegree;i++){
		//if(i==node_number)continue;
		if(OutcomingMessages[i]!=ptr[VarNodeNumbers[i]]){
			Weights[node_number*NodeDegree+i]=val;
			//printf("%d ",ptr[VarNodeNumbers[i]]);
			tmp=1;
		}
	}
	//printf("\n");
	return tmp;
}
int CNode::correction(int *src,int len,int *dst,int *weights){
	int i,j;
	int val=1000;
	int tmp,tmp2;
	s=1;
	for(i=0;i<len;i++){
		//printf("!%d\n",src[i]);
		s*=sign(src[i]);
		if (abs(src[i])<val){
			val=abs(src[i]);
			tmp=i;
			m_ind[0]=i;m[0]=val;
			
		}
	}
	//printf("m_ind[0]:%d, m[0]:%d s:%d\n",tmp,val,s);
	val=1000;
	for(i=0;i<len;i++){
		if(i==tmp)continue;
		if (abs(src[i])<val){
			val=abs(src[i]);
			tmp2=i;
			m_ind[1]=i;m[1]=val;
			
		}
	}
	//printf("m_ind[1]:%d, m[1]:%d\n",tmp2,val);
	for(i=0;i<len;i++){
		if(i==tmp){
			dst[i]= (Mode==NO_WEIGHTS) ? s*sign(src[i])*m[1] : s*sign(src[i])*RELU(m[1],weights[tmp2*len+i]);
		}else{
			dst[i]=(Mode==NO_WEIGHTS) ? s*sign(src[i])*m[0] :s*sign(src[i])*RELU(m[0],weights[tmp*len+i]);
		}
	}
	return m[0];
}
int sign(double v) {
  return (v > 0) - (v < 0);
}
void teta_scaled(int *src,int len,int *dst/*,int iter,double S*/){
	int i,j;
	int val;
	int tmp;
	//double Alpha = 1-pow(2,-ceil(((double) iter)/S));
	for(i=0;i<len;i++){
		val=-1000000;
		for(j=0;j<len;j++){
			if(i==j)continue;
			tmp = src[j];
			if(tmp<-256){tmp=-256;}
			if(tmp>256){tmp=256;}
			val = MAX(val,tmp) - MAX(0,val+tmp);			
		}
		if(val<-256){val=-256;}
		if(val>256){val=256;}
		//dst[i] = floor(val*Alpha);
		dst[i]=val;
	}
}
int RELU(int a,int b){
	int tmp=abs(a);
	int s=(a>0)?1:-1;
	return (tmp>b)? s*(tmp-b):0;
}
int print_array(char  *str,int *Var,int len){
	printf("%s: ",str);
	for(int i=0;i<len;i++){
		printf("%d ",Var[i]);
	}
	printf("\n");
}
double db2pow(double x){
    return pow(10,x/10);
}
int main(){
	int len=4;
	CNode C[2];
	int *Var = new int[len];
	C[0].addVarNodeNumber(0);
	C[0].addVarNodeNumber(2);
	C[0].addVarNodeNumber(3);
	C[0].configure();
	C[1].addVarNodeNumber(0);
	C[1].addVarNodeNumber(1);
	C[1].addVarNodeNumber(3);
	C[1].configure();
	memset((void *)Var,0,sizeof(int)*len);
	int Training_Node_Number=2;
	C[0].training(Var,Training_Node_Number);
	print_array((char *)"Var0",Var,4);
	C[1].training(Var);
	C[1].update(Var);
	print_array((char *)"Var1",Var,4);
	C[0].set_weights(Training_Node_Number,Var,8);
	print_array((char *)"W0: ",&C[0].Weights.getPtr()[Training_Node_Number*C[0].node_degree()],C[0].node_degree());
	Var[0]=-1;
	Var[1]=1;
	Var[2]=2;
	Var[3]=3;
	C[0].reset();
	int tmp=C[0].run(Var);printf(":%d\n",tmp);
	print_array((char *)"Var2",Var,4);
	//tmp=C[0].run(Var);printf(":%d\n",tmp);
	//print_array((char *)"Var3",Var,4);
	//tmp=C[0].run(Var);printf(":%d\n",tmp);
	//print_array((char *)"Var4",Var,4);
	delete Var;
	//---------------------------------------------------------------------------------
	struct Item* main_queue = read_alist2((char *)"BCH_63_36.alist");//"BCH_15_11.alist" "BCH_63_36.alist"
	struct Item* p1 = main_queue;
	struct Item* p11 = p1->next->next->next->p;
	int RepTableSize=p1->next->next->next->val;
	int *RepTable = new int[RepTableSize];
	printf("RepTable: ");
	for(int i=0;i<RepTableSize;i++){
		RepTable[i]=p11->val;
		p11=p11->next;
		printf("%d, ",RepTable[i]);
	}
	printf("\n");

	printf(" RepTableSize: %d\n",RepTableSize);
	int check_node_number=p1->next->p->next->val;
	int var_node_number=p1->next->p->val;
	CNode *CN = new CNode[check_node_number];
	int *VN = new int[var_node_number];
	printf("Check and Var Nodes Number: %d - %d\n",check_node_number,var_node_number);
	int string_num = p1->val;
	tmp = string_num - check_node_number+1;
	
	while(tmp--)p1=p1->next;
	
	printf("%d\n",p1->val);
	
	printf("%d\n",p1->p->val);
	
	for(int i=0;i<check_node_number;i++){
		if(!p1)break;
		tmp=p1->val;
		
		struct Item* p2=p1->p;
		
		printf("%d CN[%d]: ",tmp,i);
		
		while(tmp--){
			if(!p2)break;
			CN[i].addMatlabVarNodeNumber(p2->val);
			printf("%d ",p2->val);p2=p2->next;	
		}
		
		printf("\n");
		p1=p1->next;
	}
	
	for(int i=0;i<check_node_number;i++)CN[i].configure();
	int *VN2 = new int[var_node_number];
	int *S0 = new int[check_node_number];
	int i=0;
	int j=0;
	for(int i=0;i<check_node_number;i++){	
		memset((void* )VN,0,sizeof(int)*var_node_number);
		CN[i].reset(0);
		CN[i].training(VN,j);
		print_array((char *)"VN0:",VN,var_node_number);
		for(int k=0;k<check_node_number;k++){
			if(k==i)continue;
			CN[k].reset();
		}
		for(int k=0;k<check_node_number;k++){
			if(k==i)continue;
			CN[k].training(VN);
		}
		for(int k=0;k<check_node_number;k++){
			if(k==i)continue;	
			CN[k].update(VN);
			print_array((char *)"VN01                :",VN,var_node_number);break;
		}
		print_array((char *)"VN1:",VN,var_node_number);
		//memcpy((void* )VN2,(void* )VN,sizeof(int)*var_node_number);
		for(int j=0;j<CN[i].node_degree();j++){
			CN[i].set_weights(j,VN,128);
			printf("i,k: %d,%d ",i,j);
			print_array((char *)"Wi: ",&CN[i].Weights.getPtr()[j*CN[i].node_degree()],CN[i].node_degree());
			//memcpy((void* )VN,(void* )VN2,sizeof(int)*var_node_number);
		}
	}
	
	double BER=0;
	double FER=0;
	double Eb_N0=0;
	int Treshold=1;
	if(var_node_number==63)Treshold=6;
	int N1=0;
	for(int i=0;i<var_node_number;i++)if(RepTable[i]>Treshold)N1++;
	printf("NodeNumber: %d\n",N1);
	printf("\nEb_N0 : ");
	char InputBuf[256];
	//result = _cgets(InputBuf);
	char *result =gets(InputBuf);
	printf("\n");
	Eb_N0 = atof(result);
	double NoiseLevel = sqrt(db2pow(-Eb_N0))*sqrt(1.0/2);
	double ChannelParam = NoiseLevel*NoiseLevel/2;
	int Max_Num_Exp=100000;
	
	
	
	for(int num_exp=0;num_exp<Max_Num_Exp;num_exp++){
		int Event=0;
		for(int j=0;j<var_node_number-check_node_number;j++){
			double x= NoiseLevel*wgn();
			if(x>1){BER++;Event=1;}
		}
		if(Event)FER++;
	}
	printf("Uncoded BER:%f FER: %f\n",BER/(Max_Num_Exp*(var_node_number-check_node_number)),FER/Max_Num_Exp);
	BER=0;
	FER=0;
	NoiseLevel/=sqrt((var_node_number-check_node_number)/(double)var_node_number);
	for(int num_exp=0;num_exp<Max_Num_Exp;num_exp++){
		int Event=0;
		for(int j=0;j<var_node_number-check_node_number;j++){
			double x= NoiseLevel*wgn();
			if(x>1){BER++;Event=1;}
		}
		if(Event)FER++;
	}
	printf("Channel BER: %f FER: %f\n",BER/(Max_Num_Exp*(var_node_number-check_node_number)),FER/Max_Num_Exp);
	BER=0;
	FER=0;
	int Printable=0;
	int *Hist = new int[var_node_number];
	double FER1=0;
	double BER1=0;
	memset((void *)Hist,0,sizeof(int)*var_node_number);
	for(int num_exp=0;num_exp<Max_Num_Exp;num_exp++){
		for(int i=0;i<var_node_number;i++){
			VN[i]=(int)floor(8*(1.0+NoiseLevel*wgn())/ChannelParam+0.5);
			VN2[i]=0;
		}
		
		//VN[12]=-1;
		//VN[3]=-1;
		for(int i=0;i<check_node_number;i++)CN[i].reset();
		//i=0;
		//CN[1].run(VN);
		//CN[2].run(VN);
		//print_array((char *)"VNpre: ",VN,var_node_number);
		for(int num_iter=0;num_iter<16;num_iter++){
			for(int i=0;i<check_node_number;i++){
				//print_array((char *)"CN[]Out: ",CN[i].OutcomingMessages.getPtr(),CN[i].node_degree());
				S0[i]=CN[i].run(VN);
				//print_array((char *)"CN[]In: ",CN[i].IncomingMessages.getPtr(),CN[i].node_degree());
				//print_array((char *)"CN[]Out: ",CN[i].OutcomingMessages.getPtr(),CN[i].node_degree());
				//print_array((char *)"CN[]TMP: ",CN[i].TMPMessages.getPtr(),CN[i].node_degree());
				//print_array((char *)"VN-------------- ",VN,var_node_number);
				//printf("num_iter=%d; S0[%d]: %d\n",num_iter,i,S0[i]);
			}
			if(Printable)print_array((char *)"VN: ",VN,var_node_number);
			for(int i=0;i<var_node_number;i++)VN2[i]+=VN[i];
		} 
		if(Printable)print_array((char *)"VN2= ",VN2,var_node_number);
		for(int i=0;i<check_node_number;i++){
			CN[i].reset(0);
			S0[i]=CN[i].run(VN2);
		}
		if(Printable)print_array((char *)"VN2= ",VN2,var_node_number);
		int Event=0;
		int Event2=0;
		for(int i=0;i<var_node_number;i++){
			if((RepTable[i]>Treshold) && (VN[i]<0)){BER++;Event++;}//(VN2[i]<0) for 15, 11
			if((VN[i]<0))Hist[i]++;
			if(VN[i]<0)Event2++;
			
		}
		if(Event2>5){
			BER1+=Event2;
			FER1++;
		}
		if(Event)FER++;
		if(num_exp%1000==1 ){
			printf("Eb_N0 %f BER %f FER %f BER1 %f FER1 %f\r",Eb_N0,BER/(num_exp*(var_node_number-check_node_number)),FER/num_exp,BER1/(num_exp*(var_node_number-check_node_number)),FER1/num_exp);
		}
	}
	printf("Eb_N0 %f BER %f FER %f\n",Eb_N0,BER/(Max_Num_Exp*(var_node_number-check_node_number)),FER/Max_Num_Exp);
	print_array((char *)"Hist: ",Hist,var_node_number);
	print_array((char *)"RTab: ",RepTable,var_node_number);
	destroy_main_queue(main_queue);
	delete[] Hist;
	delete[] RepTable;
	delete[] CN;
	delete[] VN;
	delete[] VN2;
	delete[] S0;
	return 0;
}