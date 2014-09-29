#ifndef SPHERE_H
#define SPHERE_H

#include "TriPrimitive.h"

class Sphere: public TriPrimitive
{
public:
	void create(int subdivision);
};

#endif SPHERE_H