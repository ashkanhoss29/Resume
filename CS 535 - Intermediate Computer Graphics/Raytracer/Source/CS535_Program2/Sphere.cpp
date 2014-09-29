#define _USE_MATH_DEFINES

#include "Sphere.h"
#include <math.h>

void Sphere::create(int subdivision)
{
	// Side 3
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)(i)/(double)subdivision, 0, (double)j/(double)subdivision));
		}
	}

	// Side 4
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)i/(double)subdivision, 1, (double)j/(double)subdivision));
		}
	}

	// Side 1
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)i/(double)subdivision, (double)j/(double)subdivision, 1));
		}
	}

	// Side 2
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d((double)(i)/(double)subdivision, (double)(j)/(double)subdivision, 0));
		}
	}

	// Side 5
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d(1, (double)(i)/(double)subdivision, (double)(j)/(double)subdivision));
		}
	}

	// Side 6
	for(int i = 0; i < subdivision+1; i++)
	{
		for(int j = 0; j < subdivision+1; j++)
		{
			vertices.push_back(Vector3d(0, (double)i/(double)subdivision, (double)j/(double)subdivision));
		}
	}


	for(int i = 0; i < vertices.size(); i++)
	{
		vertices[i].x = (vertices[i].x - 0.5) * 2;
		vertices[i].y = (vertices[i].y - 0.5) * 2;
		vertices[i].z = (vertices[i].z - 0.5) * 2;

		double xSquared = vertices[i].x*vertices[i].x;
		double ySquared = vertices[i].y*vertices[i].y;
		double zSquared = vertices[i].z*vertices[i].z;
		
		double norm = sqrt(xSquared + ySquared + zSquared);
		
		vertices[i].x = (vertices[i].x / norm);
		vertices[i].y = (vertices[i].y / norm);
		vertices[i].z = (vertices[i].z / norm);


		//vertices[i].x = vertices[i].x * sqrt(1 - ySquared/2 - zSquared/2 + (ySquared*zSquared)/3);
		//vertices[i].y = vertices[i].y * sqrt(1 - zSquared/2 - xSquared/2 + (zSquared*xSquared)/3);
		//vertices[i].z = vertices[i].z * sqrt(1 - xSquared/2 - ySquared/2 + (xSquared*ySquared)/3);
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

	calculateNormals();
}