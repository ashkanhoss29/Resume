#define _USE_MATH_DEFINES

//cs domain version
#include <time.h>
#include <iostream>
#include <sstream>
#include <glut.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

struct {
	int x;
	int y;
	int z;
};

int window_width = 1920;
int window_height = 1080;
const int size = window_width * window_height;
bool update = false;
int mouse_x = 0.0;
int mouse_y = 0.0;
double scale = 1.0;
double x_pos = 0.0;
double y_pos = 0.0;

float* pixels = new float[size*3];;
unsigned char* fpixels = new unsigned char[3];

void create()
{
	// File to write

	//scale = 1.0;
	//x_pos = -1.17;
	//y_pos = 0.0;

	double x_max = 1.0 * scale + x_pos;
	double x_min = -2.5 * scale + x_pos;
	double y_max = 1.0 * scale + y_pos;
	double y_min = -1.0 * scale + y_pos;

	// Store pixel data for glDrawPixels
	//pixels = new float[size*3];
	//fpixels = new unsigned char[3];

	//For every pixel
	for(int i = 0; i < window_height; i++)
	{
		for(int j = 0; j < window_width; j++)
		{
			// Scale x and y
			double x0; // scaled x (-2.5, 1)
			double y0; // scaled y (-1, 1)

			x0 = ((j * (x_max - x_min)) / (double)window_width) + x_min;
			y0 = ((i * (y_max - y_min)) / (double)window_height) + y_min;


			double x = 0;
			double y = 0;


			int iteration = 0;
			int max_iteration = 500;
			while (x*x + y*y < (1 << 16) && iteration < max_iteration)
			{
				double xtemp = x*x - y*y + x0;
				y = 2*x*y + y0;
				x = xtemp;
				iteration++;
			}


			// Apply color to pixel
			int index = i*window_width + j;
			double r;
			double g;
			double b;
			double d = 1.0 - (double)iteration / (double)max_iteration;

			r = pow(d, 4);
			g = pow(d, 5);
			b = pow(d, 5);

			pixels[index*3] = r;
			pixels[index*3+1] = g;
			pixels[index*3+2] = b;
			

		}
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glDrawPixels(window_width,window_height,GL_RGB,GL_FLOAT,pixels);
	
	glutSwapBuffers();
}

void myMouse(int button, int state, int x, int y)
{
	glutPostRedisplay();
	switch (button) {
	case GLUT_LEFT_BUTTON:
		x_pos = x_pos + 0.1;
		create();
		update = true;
		break;
    case GLUT_RIGHT_BUTTON:
        if (state == GLUT_DOWN)
			//mouseDown = false;
			//if(draw)
				//draw = false;
			//else
				//draw = true;
           //exit (0);
        break;
    default:
        break;
	}
}


void myMouseMotion(int x, int y)
{
	//if (mouseDown) 
	//{
	//	x_pos = x_pos + (double)(x - mouse_x)/1.0;
	//	y_pos = y_pos + (double)(y - mouse_y)/1.0;
	//	create();
	//	glutPostRedisplay();
	//}
}


void myUpdate(int te)
{
	if (update)
	{
		//create();
	}

	update = false;
	glutPostRedisplay();
	glutTimerFunc(20, myUpdate, 1);
}

int main(int argc, char** argv) {

	create();
	
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);
	glutCreateWindow("OpenGL glDrawPixels demo");
	
	glutDisplayFunc(display);
	glutMouseFunc(myMouse);								// register myMouse function
	//glutMotionFunc(myMouseMotion);
	glutTimerFunc(20, myUpdate, 1);
	
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	glutMainLoop();
}
