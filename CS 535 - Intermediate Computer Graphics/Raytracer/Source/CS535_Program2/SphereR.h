#ifndef SPHERER_H
#define SPHERER_H

#include "Primitive.h"
#include "Vector3d.h"

class SphereR : public Primitive
{
public:
	SphereR();
	SphereR(double r);
	SphereR(double radius, Vector3d center);
	Data rayIntersect(Vector3d e, Vector3d d);

private:
	double r;
	Vector3d c;
};

#endif SPHERERH