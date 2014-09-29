#ifndef CUBE_H
#define CUBE_H

#include "TriPrimitive.h"

class Cube: public TriPrimitive
{
public:
	void create(int subdivision);
};

#endif CUBE_H