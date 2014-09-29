#include "Primitive.h"

void Primitive::setColor(double r, double g, double b)
{
	color.x = r;
	color.y = g;
	color.z = b;
}


Vector3d Primitive::getColor()
{
	return color;
}