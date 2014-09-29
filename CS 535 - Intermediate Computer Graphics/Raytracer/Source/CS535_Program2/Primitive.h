#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "Vector3d.h"

struct Data
{
	double t;
	Vector3d point;
	Vector3d normal;
	Vector3d color;
};

class Primitive
{
protected:
	Vector3d color;

public:
	void setColor(double r, double g, double b);
	Vector3d getColor();
	virtual Data rayIntersect(Vector3d eye, Vector3d ray) = 0;
};

#endif