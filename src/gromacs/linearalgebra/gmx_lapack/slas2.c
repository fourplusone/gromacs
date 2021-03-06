#include <math.h>
#include "gromacs/utility/real.h"

#include "../gmx_lapack.h"

void
F77_FUNC(slas2,SLAS2)(float *f,
       float *g,
       float *h,
       float *ssmin,
       float *ssmax)
{
  float fa = fabs(*f);
  float ga = fabs(*g);
  float ha = fabs(*h);
  float fhmin,fhmax,tmax,tmin,tmp1,tmp2;
  float as,at,au,c;

  fhmin = (fa<ha) ? fa : ha;
  fhmax = (fa>ha) ? fa : ha;
  
  if(fabs(fhmin)<GMX_FLOAT_MIN) {
    *ssmin = 0.0;
    if(fabs(fhmax)<GMX_FLOAT_MIN) 
      *ssmax = ga;
    else {
      tmax = (fhmax>ga) ? fhmax : ga;
      tmin = (fhmax<ga) ? fhmax : ga;
      tmp1 = tmin / tmax;
      tmp1 = tmp1 * tmp1;
      *ssmax = tmax*sqrt(1.0 + tmp1);
    }
  } else {
    if(ga<fhmax) {
      as = 1.0 + fhmin / fhmax;
      at = (fhmax-fhmin) / fhmax;
      au = (ga/fhmax);
      au = au * au;
      c = 2.0 / ( sqrt(as*as+au) + sqrt(at*at+au) );
      *ssmin = fhmin * c;
      *ssmax = fhmax / c;
    } else {
      au = fhmax / ga;
      if(fabs(au)<GMX_FLOAT_MIN) {
	*ssmin = (fhmin*fhmax)/ga;
	*ssmax = ga;
      } else {
	as = 1.0 + fhmin / fhmax;
	at = (fhmax-fhmin)/fhmax;
	tmp1 = as*au;
	tmp2 = at*au;
	c = 1.0 / ( sqrt(1.0+tmp1*tmp1) + sqrt(1.0+tmp2*tmp2));
	*ssmin = (fhmin*c)*au;
	*ssmin = *ssmin + *ssmin;
	*ssmax = ga / (c+c);
      }
    }
  }
  return;
}
