#include "wgn.h"

struct cOmplex cmp;

/*!
	first algorithm AWGN generation
	\return complex sample of AWGN
*/
struct cOmplex WhiteNoise() {
 double v1, v2, s;
 do {
  v1 = rand()/16383.5 - 1;
  v2 = rand()/16383.5 - 1;
 } while((s = v1*v1 + v2*v2) >= 1);
 if(s < 1e-9)s = 1e-9;
 s = sqrt(-2*log(s)/s);
	cmp.re = s*v1;
	cmp.im = s*v2;
 return cmp;
};
/*!
	second algorithm AWGN generation
	\return double sample of AWGN
*/
double gauss(void){
	static int _flag = 0;
	static double _v1, _v2, _s;
	if(_flag == 0){
		_flag = 1;
		do {
			_v1 = rand()/16383.5 - 1;
			_v2 = rand()/16383.5 - 1;
		} while((_s = _v1*_v1 + _v2*_v2) >= 1);
		if(_s < 1e-9)_s = 1e-9;
		_s = sqrt(-2*log(_s)/_s);
		return _s*_v1;
	}else{
		_flag = 0;
		return _s*_v2;
	}
}
/*!
	one more algorithm
	\return double sample of AWGN
*/
double wgn(void)
{
  static  int  _sw = 0;
  static  double  _t, _u, _x;
  if ( _sw == 0 )
    {
      _sw = 1;
	  _x = (1.0 / (RAND_MAX + 1.0)) * rand();
	  while(!_x)_x = (1.0 / (RAND_MAX + 1.0)) * rand();
      _t  = sqrt(-2 * log(_x));
      _u  = 2.0 * PI * ((1.0 / (RAND_MAX + 1.0)) * rand());
      return _t*cos(_u);
     }
  else
    {
      _sw = 0;
      return _t*sin(_u);
     }
}
void energy_realloc(double *ptr,int n,int n1,double A,int q){
	double A1,tmp;
	int i;
	if(q>0)n--;
	A1 = sqrt((n-n1*A*A)/(n-n1));
	tmp = (n-n1)*A1*A1+n1*A*A;
	if(q>0)n++;
	if(!ptr)return;
	for(i=0;i<n1;i++){
		ptr[i]*=A;
	}
	for(;i<n;i++){
		if(i==q)continue;
		ptr[i]*=A1;
	}
}
