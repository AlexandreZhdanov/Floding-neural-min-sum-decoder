#include "queue.h"
/*!
	item constructor
	\param int n initial value
	\return pointer to creted item
*/
struct Item * create_item(int n){
	struct Item * Ptr = (Item *)malloc(sizeof(struct Item));
	Ptr->next = NULL;
	Ptr->p = NULL;
	Ptr->val = n;
	return Ptr;
}
/*!
	\param Item * ptr pointer to the first item to be deleted in the queue 
	\return pointer to the first item in the queue except deleted or NULL
*/
struct Item * destroy_first(struct Item * ptr){
	struct Item * next;
	if(ptr == NULL)return NULL;
	next = ptr->next;
	free(ptr);
	return next;
}
/*!
	\param Item * ptr pointer to the first item in the queue to be deleted

*/
void destroy_queue(struct Item * first){
	do{
		first = destroy_first(first);
	}while(first);
}
/*!
	\param int *array source array
	\param int len its length
	\return created queue with array data
*/
struct Item * arrayTOqueue(int *array,int len){
	int i;
	struct Item * first;
	struct Item * ptr = create_item(array[0]);
	first=ptr;
	for(i=1;i<len;i++){
		ptr->next=create_item(array[i]);
		ptr=ptr->next;
	}
	return first;
}
/*!
	\param struct Item * first source array
	\param int *array destination array
	\return number of writing elements
*/
int queueTOarray(struct Item * first,int *array){
	int i = 0;
	do{
		if(array)array[i] = first->val;
		i++;
		first = first->next;
	}while(first);
	return i;
}
/*!
	\param Item *ptr pointer to the queue
	\param int adress adress to insert element
	\param number to be inserted
	\return shift adress of desired destination
*/
int add_node_to_queue(struct Item *ptr,int adress,int number){
	struct Item *tmp1;
	struct Item *tmp2;
	while(adress-- && ptr){
		tmp1 = ptr;
		ptr = ptr->next;
	}
	tmp2 = create_item(number);
	tmp2->next = tmp1->next;
	tmp1->next = tmp2;
	return adress;
}
/*!
	\param Item *ptr pointer to the queue
	\return queue length
	
*/
int queue_length(struct Item * ptr){

	int i = 1;
	if(ptr == NULL)return 0;
	while(ptr->next){
		i++;
		ptr=ptr->next;
	}
	return i;
}
/*!
	\param Item *ptr pointer to the queue
	\param int val_ins threshold value
	\return adress of finding element	
*/
int find(struct Item* ptr, int val_ins){
	int i=0;
	while(ptr && ptr->val <= val_ins){
		ptr=ptr->next;
		i++;
	}
	return i;
}
/*!
	\param Item * var_connect pointer to variable nodes connections
	\param Item* intleav pointer to interleaver
	\param Item* check_connect pointer to consraint node connections
	\return number of inserted nodes
*/
int add_snake(struct Item * var_connect,struct Item* intleav,struct Item* check_connect){
	int size_i;int size_v;int size_c;
	struct Item* ptr = var_connect;
	int var_node_num = 0;
	int check_node_num = 0;	
	int par_node_num = 0;
	int cur_size = 0;
	int i=0;
	int cur_par_num=0;
	int num_ins;
	
	/********************************
	*
	*
	*********************************/
	

	size_i = queue_length(intleav);
	size_v = queue_length(var_connect);
	size_c = queue_length(check_connect);
	if(size_c != size_i || size_c != size_v || size_i != size_v){
		return 0;
	}

	
	ptr = check_connect;
	while(ptr->next){ptr=ptr->next;}
	check_node_num = ptr->val+1;

	par_node_num = check_node_num;

	ptr = var_connect;	
	while(ptr->next){ptr=ptr->next;}
	var_node_num = ptr->val+1;

	cur_size = size_v;
	for(i=0;i<check_node_num;i++){
		num_ins = find(check_connect,i);
		add_node_to_queue(check_connect,num_ins,i);
		add_node_to_queue(intleav,num_ins,cur_size++);

		num_ins = find(check_connect,i);
		add_node_to_queue(check_connect,num_ins,i);
		add_node_to_queue(intleav,num_ins,cur_size++);
		printf("%d\r",i);
	}
	//getch();
	for(i=0;i<check_node_num;i++){
		add_node_to_queue(var_connect,cur_size,var_node_num++);
		add_node_to_queue(var_connect,cur_size,var_node_num);
		printf("%d\r",i);
	}
	return cur_size;
}
