// Name: Ashkan Hosseini
// Email: ashkan.hosseini@uky.edu
// Section: 002
// Date: 10/14/09

/*
The purpose of this program is to calculate the intersecting point of two lines.
This program will ask the user to input the coordinates of the points of two lines.
It will then calculate the slope and y intercept of each line and output the results.
A check will be made to see whether the two lines are similar or parallel.
If either case is true, the program will notify the user and will not continue the calculation of the intersecting point.
If not, the program will calculate the intersecting point of the two lines and will then output and draw the results.
*/


#include <iostream>
#include "graphics.h" //To draw the two lines and their intersecting point by using lines and a circle from the library.
using namespace std;

int main ()
{
	// Variables use type float to allow the user to input points are not integers:
	float a, b, c, d, e, f, g, h; //These represent the x and y coordinates of the points of the two lines.
	float x, y; //The x and y of the intersecting point of the two lines.
	float slope1, slope2; //Slope of the two lines.
	float y_intercept1, y_intercept2; //y intercept of the two lines.


	cout << "****Intersection Calculator****" << endl;
	cout << endl;

	cout << "Enter the x and y of the first point on line1: ";
	cin >> a >> b; //a and b are the x and y of the first point of line1.

	cout << "Enter the x and y of the second point on line1: ";
	cin >> c >> d; //c and d are the x and y of the second point of line1.
	cout << endl;

	cout << "Enter the x and y of the first point on line2: ";
	cin >> e >> f; //e and f are the x and y of the first point of line2.

	cout << "Enter the x and y of the second point on line2: ";
	cin >> g >> h; //g and h are the x and y of the second point of line2.
	cout << endl;


	// Calculations for the slopes of line1 and line2.
	slope1 = (d - b) / (c - a); //slope of line1.
	slope2 = (h - f) / (g - e); //slope of line2.

	// Calculations for the y intercepts of the two lines:
	y_intercept1 = b - (slope1 * a); //y intercept of line1.
	y_intercept2 = f - (slope2 * e); //y intercept of line2.


	cout << "Line 1:  (" << a << ", " << b << ")  and (" << c << ", " << d << ")" << endl;
	cout << "Equation of Line 1:  y = " << slope1 << " x + " << y_intercept1 << endl;
	cout << endl;

	cout << "Line 2:  (" << e << ", " << f << ")  and (" << g << ", " << h << ")" << endl;
	cout << "Equation of Line 2:  y = " << slope2 << " x + " << y_intercept2 << endl;
	cout << endl;


	// The following line of code will use if statements to take care of the problems of parallel lines and similar lines:
	if (slope1 == slope2)
	{
		if (y_intercept1 == y_intercept2)
		{
			cout << "These lines are the same!" << endl;
		}
		else
		{
			cout << "These lines are parallel!" << endl;
		}
	}
	// If the two lines are not similar or parallel the program will continue and calculate the point of intersection:
	else
	{
		// Calculations for the x and y of the intersecting point:
		x = -(y_intercept1 - y_intercept2) / (slope1 - slope2);
		y = y_intercept1 + (slope1 * x);

		cout << "Point of intersection = (" << x << ", " << y << ")" << endl;

		// Will draw the two lines and the intersecting point by using the "graphics.h" library:
		initwindow(800, 600, "Intersection Calculator"); //Creates window for graphics.
		line (int(a), int(b), int(c), int(d)); //Type casting used to change the float variables to int since the graphics library only uses type int.
		line (int(e), int(f), int(g), int(h));
		circle (int(x), int(y), 5); //Draws a circle with radius 5 at the intersecting point.

		getch();
		closegraph();
	}



	return 0;
}