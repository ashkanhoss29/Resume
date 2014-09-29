#include "Vector3d.h"
#include <math.h>

Vector3d::Vector3d()
{
	x = 0;
	y = 0;
	z = 0;
}
	
Vector3d::Vector3d(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
	
void Vector3d::normalize()
{
	double denom = (double) sqrt(x*x + y*y + z*z);
	if(denom != 0)
	{
		x = x / denom;
		y = y / denom;
		z = z / denom;
	}
	else
	{
		x = 0;
		y = 0;
		z = 0;
	}
}
	
void Vector3d::addVector(Vector3d &v)
{
	x = x + v.x;
	y = y + v.y;
	z = z + v.z;
}

double Vector3d::Dot(Vector3d &v1, Vector3d &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3d Vector3d::Cross(Vector3d &v1, Vector3d &v2)
{
	return Vector3d((v1.y * v2.z) - (v1.z * v2.y),
					(v1.z * v2.x) - (v1.x * v2.z),
					(v1.x * v2.y) - (v1.y * v2.x));
}
