#define _USE_MATH_DEFINES

#include "Cube.h"
#include <math.h>

void Cube::create(int subdivision)
{
	// Side 3
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)(i)/(double)subdivision, 0, (double)j/(double)subdivision));
			normals.push_back(Vector3d(0,-1,0));
		}
	}

	// Side 4
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)i/(double)subdivision, 1, (double)j/(double)subdivision));
			normals.push_back(Vector3d(0,1,0));
		}
	}

	// Side 1
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)i/(double)subdivision, (double)j/(double)subdivision, 1));
			normals.push_back(Vector3d(0,0,1));
		}
	}

	// Side 2
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)(i)/(double)subdivision, (double)(j)/(double)subdivision, 0));
			normals.push_back(Vector3d(0,0,-1));
		}
	}

	// Side 5
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d(1, (double)(i)/(double)subdivision, (double)(j)/(double)subdivision));
			normals.push_back(Vector3d(1,0,0));
		}
	}

	// Side 6
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d(0, (double)i/(double)subdivision, (double)j/(double)subdivision));
			normals.push_back(Vector3d(-1,0,0));
		}
	}

	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = (vertices[i].x - 0.5) * 2;
		vertices[i].y = (vertices[i].y - 0.5) * 2;
		vertices[i].z = (vertices[i].z - 0.5) * 2;
	}

	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vertices[i] * s;
		//vertices[i] = vertices[i] * 10.0;
		//vertices[i].y = vertices[i].y * 0.5;
		vertices[i].x = vertices[i].x + t.x;
		vertices[i].y = vertices[i].y + t.y;
		vertices[i].z = vertices[i].z + t.z;
	}

	// Index all sides
	for(int k = 0; k < 6; k++)
	{
		for(int i = 0; i < subdivision; i++)
		{
			for(int j = 0; j < subdivision; j++)
			{

				if(k == 0)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
				else if(k == 1)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
				else if(k == 2)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
				else if(k == 3)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
				else if(k == 4)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
				else if(k == 5)
				{
					int planerIndex = ((subdivision+1)*(subdivision+1))*k;

					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+(j+1)+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					indices.push_back((i+1)*(subdivision+1)+j+planerIndex);
					indices.push_back((i)*(subdivision+1)+j+planerIndex);
					indices.push_back((i+1)*(subdivision+1)+(j+1)+planerIndex);

					triangleCount = triangleCount + 2;
				}
			}
		}
	}

	//calculateNormals();
}