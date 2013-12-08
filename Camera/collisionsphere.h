#include "maths_funcs.h"
#ifndef COLLISIONSPHERE_H
#define COLLISIONSPHERE_H

class collisionsphere{
	public:
	float r;
	vec3 center;
	collisionsphere(vec3 c,float rad);
	collisionsphere();
};

#endif
