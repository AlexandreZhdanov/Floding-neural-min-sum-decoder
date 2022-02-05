#ifndef _queue_h_
#define _queue_h_
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
//! Integer value container 
struct Item{
	struct Item *p;/*!<pointer to the may be prev or may be not item*/
	struct Item *next;/*!<pointer to the next item*/
	int val;/*!<container value*/
};
//! constructor
struct Item * create_item(int n);
//! destroy first element in the queue
struct Item * destroy_first(struct Item * ptr);
//! destroy queue from the first item 
void destroy_queue(struct Item * first);
//! create queue and put array here
struct Item * arrayTOqueue(int *array,int len);
//! put queue container into array
int queueTOarray(struct Item * first,int *array);
//! add item to queue into specified adress
int add_node_to_queue(struct Item *ptr,int adress,int number);
//! compute queue length
int queue_length(struct Item * ptr);
//! find number of item that has value more than val_ins
int find(struct Item* ptr, int val_ins);
//! add connections to parity bits in repeat accumulative codes
int add_snake(struct Item * var_connect,struct Item* intleav,struct Item* check_connect);
#ifdef __cplusplus
}
#endif /* extern "C" */


#endif