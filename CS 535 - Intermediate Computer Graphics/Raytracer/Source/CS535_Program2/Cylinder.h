#ifndef CYLINDER_H
#define CYLINDER_H

#include "TriPrimitive.h"

class Cylinder: public TriPrimitive
{
public:
	void create(int subdivision);
};

#endif CYLINDER_H