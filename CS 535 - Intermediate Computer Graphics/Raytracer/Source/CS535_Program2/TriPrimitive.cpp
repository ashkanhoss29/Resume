#include <iostream>
#include "TriPrimitive.h"
#include <glut.h>


int TriPrimitive::getTriangleCount()
{
	return triangleCount;
}


void TriPrimitive::calculateNormals()
{
	for(int i = 0; i < vertices.size(); i++)
	{
		normals.push_back(Vector3d(0,0,0));
	}

	for(int i = 0; i < triangleCount; i++)
	{
		// Get the indices for this triangle
		int i0 = indices[i*3+0];
		int i1 = indices[i*3+1];
		int i2 = indices[i*3+2];

		// Get the vertices for this triangle
		Vector3d v0 = vertices[i0];
		Vector3d v1 = vertices[i1];
		Vector3d v2 = vertices[i2];

		Vector3d e0(
			v1.x - v0.x,
			v1.y - v0.y,
			v1.z - v0.z
			);
		Vector3d e1(
			v2.x - v0.x,
			v2.y - v0.y,
			v2.z - v0.z
			);
		
		// Cross product
		Vector3d cross(
			e0.y*e1.z - e0.z*e1.y,
			e0.z*e1.x - e0.x*e1.z,
			e0.x*e1.y - e0.y*e1.x
			);

		normals[i0].x = normals[i0].x + cross.x;
		normals[i0].y = normals[i0].y + cross.y;
		normals[i0].z = normals[i0].z + cross.z;

		normals[i1].x = normals[i1].x + cross.x;
		normals[i1].y = normals[i1].y + cross.y;
		normals[i1].z = normals[i1].z + cross.z;

		normals[i2].x = normals[i2].x + cross.x;
		normals[i2].y = normals[i2].y + cross.y;
		normals[i2].z = normals[i2].z + cross.z;
	}

	for(int i = 0; i < normals.size(); i++)
	{
		double xSquared = normals[i].x*normals[i].x;
		double ySquared = normals[i].y*normals[i].y;
		double zSquared = normals[i].z*normals[i].z;

		double norm = sqrt(xSquared + ySquared + zSquared);

		normals[i].x = normals[i].x / norm;
		normals[i].y = normals[i].y / norm;
		normals[i].z = normals[i].z / norm;
	}

}

Data TriPrimitive::rayIntersect(Vector3d eye, Vector3d ray)
{
	Data data;
	data.t = 1000000.0;

	int triCnt = 0;
	bool output = false;
	double t = 1000000.0;
	for(int i = 0; i < triangleCount; i++)
	{
		triCnt++;
		//color.x = color.x - 0.1;
		Vector3d p1(vertices[indices[i*3]].x, vertices[indices[i*3]].y, vertices[indices[i*3]].z);
		Vector3d p2(vertices[indices[i*3+1]].x, vertices[indices[i*3+1]].y, vertices[indices[i*3+1]].z);
		Vector3d p3(vertices[indices[i*3+2]].x, vertices[indices[i*3+2]].y, vertices[indices[i*3+2]].z);

		Vector3d d = eye - p1; //ray.o - p1;
		Vector3d e1 = p2 - p1;
		Vector3d e2 = p3 - p1;
		Vector3d s1 = Vector3d::Cross(ray, e2);
		Vector3d s2 = Vector3d::Cross(d, e1);
		double divisor = Vector3d::Dot(s1, e1);

		if(divisor == 0.0)
			continue;

		double invDivisor = 1.0 / divisor;

		//Compute first barycentric coordinate
		double b1 = Vector3d::Dot(d, s1) * invDivisor;
		if(b1 < 0.0 || b1 > 1.0)
			continue;

		//Compute second barycentric coordinate
		double b2 = Vector3d::Dot(ray, s2) * invDivisor;
		if(b2 < 0.0 || b1 + b2 > 1.0)
			continue;

		//Compute t to intersection point
		double T = Vector3d::Dot(e2, s2) * invDivisor;

		double epsilon = 0.000001;
		if(T < t && T > epsilon)
		{
			t = T;
			
			color = Vector3d(1,1,1);
			Vector3d light(0,0,5);
			//light.normalize();
			light = light - (eye + ray*t);
			light.normalize();
			double b0 = 1.0 - b1 - b2;
			Vector3d n = normals[indices[i*3]]*b0 + normals[indices[i*3+1]]*b1 + normals[indices[i*3+2]]*b2;
			n.normalize();
			//n = n * -1.0;
			double diffuse = 0.9*Vector3d::Dot(light, n);
			if(diffuse < 0.0) diffuse = 0.0;
			//color = color*diffuse;

			data.t = t;
			data.point = eye + ray*t;
			data.normal = n;
			data.color = color*diffuse;
		}
	}
	
	return data;
}

void TriPrimitive::invertNormals()
{
	for(int i = 0; i < normals.size(); i++)
	{
		normals.at(i) = normals.at(i) * -1.0;
	}
}

void TriPrimitive::translate(Vector3d t)
{
	this->t = t;
}

void TriPrimitive::scale(Vector3d s)
{
	this->s = s;
}

void TriPrimitive::draw()
{
	for(int i = 0; i < triangleCount; i++)
	{
		glNormal3d(
			normals[indices[i*3]].x,
			normals[indices[i*3]].y,
			normals[indices[i*3]].z
			);
		glVertex3d(
			vertices[indices[i*3]].x,
			vertices[indices[i*3]].y,
			vertices[indices[i*3]].z
			);
		
		glNormal3d(
			normals[indices[i*3+1]].x,
			normals[indices[i*3+1]].y,
			normals[indices[i*3+1]].z
			);
		glVertex3d(
			vertices[indices[i*3+1]].x,
			vertices[indices[i*3+1]].y,
			vertices[indices[i*3+1]].z
			);
		
		glNormal3d(
			normals[indices[i*3+2]].x,
			normals[indices[i*3+2]].y,
			normals[indices[i*3+2]].z
			);
		glVertex3d(
			vertices[indices[i*3+2]].x,
			vertices[indices[i*3+2]].y,
			vertices[indices[i*3+2]].z
			);
		
		//std::cout << vertices[indices[i*3]].x << " " << vertices[indices[i*3]].y << " " << vertices[indices[i*3]].z << std::endl;
		//std::cout << vertices[indices[i*3+1]].x << " " << vertices[indices[i*3+1]].y << " " << vertices[indices[i*3+1]].z << std::endl;
		//std::cout << vertices[indices[i*3+2]].x << " " << vertices[indices[i*3+2]].y << " " << vertices[indices[i*3+2]].z << std::endl << std::endl;
	}
}

