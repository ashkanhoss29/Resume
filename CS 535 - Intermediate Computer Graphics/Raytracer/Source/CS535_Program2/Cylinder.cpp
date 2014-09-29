#define _USE_MATH_DEFINES

#include "Cylinder.h"
#include <math.h>

void Cylinder::create(int subdivision)
{
	// Center and Top vertices
	vertices.push_back(Vector3d(0,0,-1)); //center vertex
	vertices.push_back(Vector3d(0,0,1)); //top vertex

	// Circle vertices
	double dTheta = (2.0 * M_PI) / (5.0 + subdivision);
	double theta = 0;
	for(int i = 0; i < 5+subdivision; i++)
	{
		vertices.push_back(Vector3d(cos(theta), sin(theta), -1));
		vertices.push_back(Vector3d(cos(theta), sin(theta), 1));
		theta = theta + dTheta;
	}

	// Indices / Triangles
	for(int i = 0; i < 5 + subdivision; i++)
	{
		//indices.push_back(0);

		// Top and bottom circles
		indices.push_back(0);
		if(i != 4 + subdivision)
		{
			indices.push_back((i+1)*2+2);
		}
		else
		{
			indices.push_back(2);
		}
		indices.push_back((i+1)*2);


		indices.push_back(1);
		indices.push_back((i+1)*2+1);
		if(i != 4 + subdivision)
		{
			indices.push_back((i+1)*2+3);
		}
		else
		{
			indices.push_back(3);
		}



		// Sides
		if(i != 4 + subdivision)
		{
			indices.push_back((i+1)*2+2);
		}
		else
		{
			indices.push_back(2);
		}
		indices.push_back((i+1)*2+1);
		indices.push_back((i+1)*2);


		indices.push_back((i+1)*2+1);
		if(i != 4 + subdivision)
		{
			indices.push_back((i+1)*2+2);
			indices.push_back((i+1)*2+3);
		}
		else
		{
			indices.push_back(2);
			indices.push_back(3);
		}


		triangleCount = triangleCount + 4;
	}

	calculateNormals();
}