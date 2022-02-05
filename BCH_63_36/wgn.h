#ifndef __wgn_h__
#define __wgn_h__

#include <math.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
//! simple complex number implementation
struct cOmplex{
	double re;/*!<real part*/
	double im;/*!<image part*/
};
extern struct cOmplex cmp;
//! additive white gaussian noise implementation
struct cOmplex WhiteNoise();
#define PI 3.14159265358
/*#define RAND_MAX        0x7fffffff*/
//! one more additive white gaussian noise implementation
double wgn(void);
//! yet another additive white gaussian noise implementation
double gauss(void);
void energy_realloc(double *ptr,int n,int n1,double A,int q);
#ifdef __cplusplus
}
#endif /* extern "C" */


#endif