#include "collisionsphere.h"

collisionsphere::collisionsphere()
{
	center.v[0]=center.v[1]=center.v[2]=0;
	r=0;
}

collisionsphere::collisionsphere(vec3 c,float r2)
{
	center=c;
	r=r2;
}
