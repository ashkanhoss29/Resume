/***************************************************
 *  A simple raytracer with reflections and refractions
 **************************************************/
#define _USE_MATH_DEFINES

//cs domain version
#include <time.h>
#include <iostream>
#include <sstream>
#include <glut.h>
#include <math.h>
#include <vector>
#include "Vector3d.h"
#include "Cone.h"
#include "Cylinder.h"
#include "Cube.h"
#include "SphereR.h"


int window_width = 512;
int window_height = 512;
const int size = window_width * window_height;

float* pixels;
SphereR sphere;
Cube cube;
Cube glass;
Cone cone;

double nr = 1.0 / 1.51;

std::vector<Primitive*> objects;


bool refract(Vector3d eye, Vector3d ray, Vector3d normal, double n, Vector3d &p, Vector3d &t)
{

	double pCosPhi = 1.0 - ((n*n) * (1.0 - Vector3d::Dot(ray, normal)*Vector3d::Dot(ray, normal)));
	if(pCosPhi < 0.0)
		return false;

	Vector3d sinPhi = ((ray - normal * (Vector3d::Dot(ray, normal))) * n);
	Vector3d cosinePhi = normal * sqrt(pCosPhi);
	t = sinPhi - cosinePhi;
	t.normalize();

	return true;
}

Vector3d rayColor(Vector3d eye, Vector3d ray, int depth)
{
	if(depth > 12)
		return Vector3d(0,0,0);

	Data data;
	Vector3d color;
	double t = 1000000.0;

	int hit = -1;
	for(int i = 0; i < objects.size(); i++)
	{
		//if(i == 0 && depth == 1)
			//continue;

		Data tempData = objects.at(i)->rayIntersect(eye, ray);
		if(tempData.t < t)
		{
			data = tempData;
			hit = i;
			t = data.t;
			color = data.color;
			//objects.at(i)->setColor(1,1,1);
		}
	}


	if(hit != 0)
	{
		Vector3d t;
		Vector3d p = data.point;
		Vector3d r = ray - data.normal * Vector3d::Dot(ray, data.normal) * 2.0;
		double c = 0.0;
		Vector3d k(1,1,1);

		if(Vector3d::Dot(ray, data.normal) < 0.0)
		{
			refract(data.point, ray, data.normal, nr, p, t);
			c = Vector3d::Dot(ray*-1.0, data.normal);
		}
		else
		{
			k.x = exp(-0.05*data.t);
			k.y = exp(-0.0*data.t);
			k.z = exp(-0.05*data.t);

			if(refract(data.point, ray, data.normal*-1.0, 1/nr, p, t))
			{
				c = Vector3d::Dot(t, data.normal);
			}
			else
			{
				return k * rayColor(data.point, r, depth+1);
			}
		}

		double R0 = (nr - 1.0)/(nr + 1.0); R0 = R0*R0;
		double R = R0 + pow((1.0 - R0) * (1.0 - c), 5);

		color = k * (rayColor(p, r, depth+1)*R + rayColor(p, t, depth+1)*(1 - R));
	}


	return color;
}

void create()
{
	//Create some nice colours (3 floats per pixel) from data -10..+10
	pixels = new float[size*3];


	// Create objects

	cube.translate(Vector3d(0,0,6));
	cube.scale(Vector3d(10,10,10));
	cube.create(1);
	cube.setColor(1,1,1);
	cube.invertNormals();
	objects.push_back(&cube);

	glass.translate(Vector3d(-3,-3,0));
	glass.scale(Vector3d(2,2,2));
	glass.create(4);
	glass.setColor(1,1,1);
	//objects.push_back(&glass);

	cone.create(20);
	cone.setColor(1,1,1);
	//objects.push_back(&cone);

	sphere = SphereR(1.0, Vector3d(2.5,2,0));
	sphere.setColor(1, 1, 1);
	objects.push_back(&sphere);

	SphereR sphere1 = SphereR(1, Vector3d(0,-2,1));
	sphere1.setColor(1, 1, 1);
	objects.push_back(&sphere1);

	SphereR sphere2 = SphereR(1, Vector3d(-2,-2,0));
	sphere2.setColor(1, 1, 1);
	objects.push_back(&sphere2);

	SphereR sphere3 = SphereR(1, Vector3d(-1.5,-1.5,1));
	sphere3.setColor(1, 1, 1);
	//objects.push_back(&sphere3);



	// Create camera
	Vector3d eye(6,4,0);
	Vector3d up(0,1,0);
	Vector3d lookAt(0,0,0);
	Vector3d camera_right = Vector3d::Cross(lookAt, up);
	double zoomFactor = 1.0;


	// Run ray-trace algorithm
	for(int i = 0; i < window_height; i++)
	{
		//For every pixel
		for(int j = 0; j < window_width; j++)
		{
			double t = 1000000;
			Vector3d color(0,0,0);

			// Find ray for pixel
			double xp = ((double)i/(double)window_height) - 0.5;
			double yp = ((double)j/(double)window_width) - 0.5;
			Vector3d w = lookAt - eye; w.normalize();
			Vector3d u = Vector3d::Cross(up, w);
			Vector3d v = Vector3d::Cross(w, u);
			Vector3d ray = w*zoomFactor + u*xp + v*yp;
			ray.normalize();
			

			// Find color for pixel
			color = rayColor(eye, ray, 0);


			// Apply color to pixel
			int index = i*window_width + j;
			pixels[index*3] = color.x;
			pixels[index*3+1] = color.y;
			pixels[index*3+2] = color.z;
		}
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDrawPixels(window_width,window_height,GL_RGB,GL_FLOAT,pixels);
	
	glutSwapBuffers();
}

int main(int argc, char** argv) {

	create();
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("OpenGL glDrawPixels demo");
	
	glutDisplayFunc(display);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glutMainLoop();
}
