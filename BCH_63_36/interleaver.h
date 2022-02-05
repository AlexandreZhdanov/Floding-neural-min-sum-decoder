#ifndef _interleaver_h_
#define _interleaver_h_
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
extern "C" {
#endif
//! make deinterleaver
int * make_deintleav(int *dst,int *src,int len);
//! make self permutable interleaver
int * dumb_interleaver(int * intleav,int length);
//! generate interleaver with row and columns
void generate_standart_interleaver(int * ptr, int NumRow, int frame_length);
//! make shift inside specified region
void inner_recyclic_shift(int *ptr, int start_index, int stop_index);
//! make shift interleaver
void recyclic_shift(int *ptr,int t,int len );
//! separate interleaver and make recycle shift
void make_rcs_interleaver_2on2(int *intlv, int t, int l);
//! join two interleavers
void combined_interleaver(int *dst, int *first,int len_f,int *second,int len_s);
//! make dithered interleaver
void dithered_interleaver(int *dst,int length,const int *I, int len_I, int s, int p, char *shifts);
#ifdef __cplusplus
}
#endif /* extern "C" */


#endif