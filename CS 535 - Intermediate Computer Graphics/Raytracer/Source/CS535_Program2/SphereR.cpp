#include "SphereR.h"
#include <math.h>

SphereR::SphereR()
{
	r = 1.0;
	c = Vector3d(0,0,0);
	color = Vector3d(1,1,1);
}

SphereR::SphereR(double radius)
{
	r = radius;
	c = Vector3d(0,0,0);
	color = Vector3d(1,1,1);
}

SphereR::SphereR(double ra, Vector3d center)
{
	r = ra;
	c = center;
	color = Vector3d(1,1,1);
}


Data SphereR::rayIntersect(Vector3d e, Vector3d d)
{
	Data data;
	data.t = 1000000.0;

	//(d . (e - c))^2
	Vector3d B1 = e - c;
	double B2 = Vector3d::Dot(d, B1);
	double B = B2*B2;

	//(d . d)((e - c) . (e - c) - r^2)
	double AC1 = Vector3d::Dot(B1, B1);
	double AC2 = AC1 - r*r;
	double AC = Vector3d::Dot(d, d) * AC2;

	double discriminant = B - AC;

	if(discriminant <= 0.0)
		return data;

	double t = 0;
	double t1 = (-1.0*Vector3d::Dot(d, B1) + sqrt(discriminant)) / Vector3d::Dot(d, d);
	double t2 = (-1.0*Vector3d::Dot(d, B1) - sqrt(discriminant)) / Vector3d::Dot(d, d);

	double epsilon = 0.000001;
	if(t1 < epsilon)
		t1 = 1000000;
	if(t2 < epsilon)
		t2 = 1000000;

	if(t1 < t2)
		t = t1;
	else
		t = t2;

	Vector3d l(3,0,3); l.normalize();
	Vector3d p = e + d*t;
	Vector3d n = (p - c) * 2.0;
	n.normalize();
	double diffuse = 0.7*Vector3d::Dot(l, n);
	if(diffuse < 0.0) diffuse = 0.0;
	//color = color*diffuse;

	data.t = t;
	data.point.x = p.x; // = p;
	data.point.y = p.y;
	data.point.z = p.z;
	data.normal = n;
	data.color = color*diffuse;

	return data;
}