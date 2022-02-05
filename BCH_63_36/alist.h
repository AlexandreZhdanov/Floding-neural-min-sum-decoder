#ifndef _alist_h_
#define _alist_h_
#include "queue.h"
#include "interleaver.h"
#include <stdio.h>
//#include <sys/io.h>
#include <conio.h>
#include <io.h>
#ifdef __cplusplus
extern "C" {
#endif
//! Structure describe bipartite graph 
/*! The bipartite graph represents a parity chec matrix of low dencity parity check code */
struct Bipartite{
	struct Item* var_connect; /*!< pointer to list of connections variable nodes*/
	struct Item* check_connect; /*!< pointer to list of connections check nodes*/
	struct Item* intleav; /*!< pointer to interlever*/
	struct Item* irregular_repeat;/*!< pointer to  irregular repetition pattrn*/
	int info_len;/*!< number of information bits in frame*/
	int check_node_num;/*!< number of constraint nodes in frame*/
	int enc_intleav_size;/*!< is equal to repeating symbol number*/
};

struct Item* read_alist2(char *fname);

void make_encoder_queues(struct Item* main_queue,struct Bipartite* B,int permute_zig_zag,int preved_from_matlab);

void destroy_main_queue(struct Item* main_queue);
//void read_alist(char *fname);//,struct Item * var_connect,struct Item* intleav,struct Item* check_connect);
//void read_pure_alist(char *fname);//,struct Item * var_connect,struct Item* intleav,struct Item* check_connect);
#ifdef __cplusplus
}
#endif /* extern "C" */


#endif