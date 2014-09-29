#define _USE_MATH_DEFINES

#include "Cone.h"
#include <math.h>

void Cone::create(int subdivision)
{
	// Center and Top vertices
	vertices.push_back(Vector3d(0,0,0)); //center vertex
	vertices.push_back(Vector3d(0,0,1)); //top vertex

	// Circle vertices
	double dTheta = 2.0 * M_PI / (5.0 + subdivision);
	double theta = 0;
	for(int i = 0; i < 5+subdivision; i++)
	{
		vertices.push_back(Vector3d(cos(theta), sin(theta), 0));
		theta = theta + dTheta;
	}

	// Indices / Triangles
	for(int i = 0; i < 5 + subdivision; i++)
	{
		indices.push_back(0);
		if(i != 4 + subdivision)
		{
			indices.push_back(i+3);
		}
		else
		{
			indices.push_back(2);
		}
		indices.push_back(i+2);

		indices.push_back(1);
		indices.push_back(i+2);
		if(i != 4 + subdivision)
		{
			indices.push_back(i+3);
		}
		else
		{
			indices.push_back(2);
		}

		triangleCount = triangleCount + 2;
	}

	calculateNormals();
}