// Name: Ashkan Hosseini
// Email: ashkan.hosseini@uky.edu
// Section: 002
// Date: 11/02/09

/*
The purpose of this program is to calculate the trajectories of projectiles based on which planet/moon they are on (Earth, Moon, or Mars).
The program will:
1. get values of speed and angle in degrees from user.
2. convert angle in degrees to radians, and calculate the horizontal and veritical distance treveled on each planet/moon.
3. output what the user entered, degrees in radians, and horizontal and vertical distance on each planet/mooon.
4. compare the horizontal and vertical distances of each planet/moon and will output the largest value. 
5. calculate and output five points on the three trajectories.
6. draw the trajectories.
*/


#include <iostream>
#include <iomanip> //To be able to set number of decimal places to two
#include <cmath>   //Will use the following: pow(), sin(), cos(), tan()
#include "graphics.h" //Will use graphics functions to draw the three trajectories and set each one to a particular color
using namespace std;

//Prototypes for the functions (see functions for details on their purpose):
double degree_converter(float degrees);
double distance_calculator(float speed, float angle_radians, float gravity);
double height_calculator(float speed, float angle_radians, float gravity, float distance);
float height_comparison(float earth, float moon, float mars);
void coordinates(float speed, float angle_radians, float G, float planet_x4, float planet_y2, int color);
int x_pixels(float x);
int y_pixels(float y);


int main()
{
	//Variables are type float and double for precision:
	float speed;
	float angle_degrees;
	float angle_radians;
	float earth_distance, moon_distance, mars_distance; //Variables for maximum horizontal distances
	float earth_x4, moon_x4, mars_x4; //Variables for half of horizontal distances for maximum height calculations
	float earth_height, moon_height, mars_height; //Variables for maximum heights

	//Gravitational constants:
	const double G_Earth = 9.81;
	const double G_Moon = 1.62;
	const double G_Mars = 3.77;


	cout << "Trajectory Plotter!" << endl;
	//The following will set the output of values to have two decimal places:
	cout << fixed << showpoint << setprecision(2);

	cout << "Enter a speed of launch (mps): ";
	cin >> speed;
	cout << "Enter the angle of launch (in degrees): ";
	cin >> angle_degrees;

	//Will use degree_converter to convert degrees to radians and set value to variable angle_radians for later use:
	angle_radians = degree_converter(angle_degrees);

	cout << "Speed = " << speed << " mps, "
	     << "Angle = " << angle_degrees << " degrees = " << angle_radians << " radians" << endl << endl;


	//Calculations  for maximum horizontal and vertical distances:
	// Earth calculations:
	earth_distance = distance_calculator(speed, angle_radians, G_Earth);
	earth_x4 = earth_distance * 0.5; //Used for maximum height calculation
	earth_height = height_calculator(speed, angle_radians, G_Earth, earth_x4);
	// Moon calculations:
	moon_distance = distance_calculator(speed, angle_radians, G_Moon);
	moon_x4 = moon_distance * 0.5; //Used for maximum height calculation
	moon_height = height_calculator(speed, angle_radians, G_Moon, moon_x4);
	// Mars calculations:
	mars_distance = distance_calculator(speed, angle_radians, G_Mars);
	mars_x4 = mars_distance * 0.5; //Used for maximum height calculation
	mars_height = height_calculator(speed, angle_radians, G_Mars, mars_x4);


	cout << "Distance traveled on Earth = " << earth_distance << " m,  max height = " << earth_height << " m" << endl;
	cout << "Distance traveled on Moon = " << moon_distance << " m,  max height = " << moon_height << " m" << endl;
	cout << "Distance traveled on Mars = " << mars_distance << " m,  max height = " << mars_height << " m" << endl << endl;

	cout << "Maximum distance traveled of the 3 planets = " << height_comparison(earth_distance, moon_distance, mars_distance) << endl;
	cout << "Maximum height reached of the 3 planets = " << height_comparison(earth_height, moon_height, mars_height) << endl << endl;

	//Will draw a window:
	initwindow(640, 480, "Trajectory Plotter!");


	//The following calculates and outputs 5 points on the trajectories of each planet/moon:
	cout << "For Earth, the 5 points of the parabola would be: " << endl;
	coordinates(speed, angle_radians, G_Earth, earth_distance, earth_height, 5);
	outtextxy(10, 400, "Earth"); //Draws string Earth

	cout << "For Moon, the 5 points of the parabola would be: " << endl;
	coordinates(speed, angle_radians, G_Moon, moon_distance, moon_height, 2);
	outtextxy(10, 380, "Moon"); //Draws string Moon

	cout << "For Moon, the 5 points of the parabola would be: " << endl;
	coordinates(speed, angle_radians, G_Mars, mars_distance, mars_height, 3);
	outtextxy(10, 360, "Mars"); //Draws string Mars


	getch();
	closegraph();
	return 0;
}


//Function to convert degrees to radians:
double degree_converter(float degrees)
{
	const double PI = 3.1415927;
	return PI * (degrees / 180);
}


//Function to calculate maximum horizontal distance:
double distance_calculator(float speed, float angle_radians, float gravity)
{
	return ((pow(speed, 2)) * (sin(2 * angle_radians))) / (gravity);
}


//Function to calculate height based upon horizontal distance:
double height_calculator(float speed, float angle_radians, float gravity, float distance)
{
	return (distance * tan(angle_radians)) - ((gravity * pow((distance), 2) / (2 * ((speed * cos(angle_radians) * (speed * cos(angle_radians)))))));
}


//Function to determine which of the planets have the heighest vertical distance (height):
float height_comparison(float earth, float moon, float mars)
{
	if (earth > moon)
	{
		if (earth > mars)
			return earth;
		else
			return mars;
	}
	else
		return moon;
}

//Function used to convert x coordinates from meters to pixels:
int x_pixels(float x)
{
	return (10 + ((620 * int(x)) / 10000));
}

//Function used to convert y coordinates from meters to pixels:
int y_pixels(float y)
{
	return (470 - ((460 * int(y)) / 2000));
}


//Function to calculate and output 5 coordinates on the trajectorie of a planet/moon
//Will also draw the trajectories:
void coordinates(float speed, float angle_radians, float G, float planet_x4, float planet_y2, int color)
{
	float x0, x1, x2, x3;
	float y0, y1, y3, y4;
	int int_x0, int_x1, int_x2, int_x3, int_x4;
	int int_y0, int_y1, int_y2, int_y3, int_y4;

	//Calculations for x coordinates:
	x0 = 0;
	x1 = (planet_x4 * 0.25);
	x2 = (planet_x4 * 0.5);
	x3 = (planet_x4 * 0.75);

	//Calculations for y coordinates:
	y0 = 0;
	y1 = height_calculator(speed, angle_radians, G, x1);
	y3 = height_calculator(speed, angle_radians, G, x3);
	y4 = 0;

	//Output of x and y coordinates:
	cout << " x0, y0 (" << x0 << ", " << y0 << ")" << endl;
	cout << " x1, y1 (" << x1 << ", " << y1 << ")" << endl;
	cout << " x2, y2 (" << x2 << ", " << planet_y2 << ")" << endl;
	cout << " x3, y3 (" << x3 << ", " << y3 << ")" << endl;
	cout << " x4, y4 (" << planet_x4 << ", " << y4 << ")" << endl << endl;

	//The following will draw the trajectories while also setting each one to a particular color:
	setcolor(color);

	int_x0 = x_pixels(x0);
	int_x1 = x_pixels(x1);
	int_x2 = x_pixels(x2);
	int_x3 = x_pixels(x3);
	int_x4 = x_pixels(planet_x4);

	int_y0 = y_pixels(y0);
	int_y1 = y_pixels(y1);
	int_y2 = y_pixels(planet_y2);
	int_y3 = y_pixels(y3);
	int_y4 = y_pixels(y4);

	line (int_x0, int_y0, int_x1, int_y1);
	line (int_x1, int_y1, int_x2, int_y2);
	line (int_x2, int_y2, int_x3, int_y3);
	line (int_x3, int_y3, int_x4, int_y4);

	//These will draw a small circle where the points on the trajectories are:
	circle(int_x0, int_y0, 5);
	circle(int_x1, int_y1, 5);
	circle(int_x2, int_y2, 5);
	circle(int_x3, int_y3, 5);
	circle(int_x4, int_y4, 5);

}