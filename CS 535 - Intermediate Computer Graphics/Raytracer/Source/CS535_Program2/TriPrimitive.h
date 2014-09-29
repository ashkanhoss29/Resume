#ifndef TRIPRIMITIVE_H
#define TRIPRIMITIVE_H

#include "Primitive.h"
#include <vector>
#include "Vector3d.h"

class TriPrimitive : public Primitive
{
protected:
	int subdivision;
	int triangleCount;
	std::vector<Vector3d> vertices;
	std::vector<double> indices;
	std::vector<Vector3d> normals;

	Vector3d t;
	Vector3d s;

	void calculateNormals();

public:
	virtual void create(int subdivision) = 0;
	int getTriangleCount();
	Data rayIntersect(Vector3d eye, Vector3d ray);
	void invertNormals();
	void translate(Vector3d t);
	void scale(Vector3d s);
	void draw();
};

#endif