// Ashakn Hosseini
// CS115 Section 002
// E-mail: ashkan.hosseini@uky.edu
// 9/20/09
 
// This program will find the center and radius of a circle.


#include <iostream>
#include <cmath>  // pow(), sqrt()
#include "graphics.h" // initwindow(), circle(), getch(), closegraph()
using namespace std;

int main ()
{
	// Variables in this program will use type float so output will give accurate results and also so the (1/2) will not cuase the cneter to be (0,0)
	float a, b, c, d, e, f; // (a,b)=first observer, (c,d)=second observer, (e,f)=third observer.
	float x, y; // coordinates of circle
	float radius; // radius of circle

	// The following variables are used to accurately converting the type float variables to type int.
	// We do this because the graphics library only uses type int.
	int ai, bi, ci, di, ei, fi, xi, yi, iradius;


	cout << "****Lighthouse Location Calculator****" << endl;
	cout << endl;

	// Gets (x, y) for first observer:
	cout << "Enter the x and y of the first observer: ";
	cin >> a >> b;
	
	// Gets (x, y) for second observer:
	cout << "Enter the x and y of the second observer: ";
	cin >> c >> d;
	
	// Gets (x, y) for third observer:
	cout << "Enter the x and y of the third observer: ";
	cin >> e >> f;
    cout << endl;
	
	// Outputs the values entered by user:
	cout << "The three observers are at" << endl;
	cout << "(" << a << ", " << b << ")" << endl;
	cout << "(" << c << ", " << d << ")" << endl;
    cout << "(" << e << ", " << f << ")" << endl;
    cout << endl;

	// Calculates the center and radius of circle:
	x = (float(1) / float(2)) * ((pow(a, 2) + pow(b, 2)) * (f - d) + (pow(c, 2) + pow(d, 2)) * (b - f) + (pow(e, 2) + pow(f, 2)) * (d - b)) / (a * (f - d) + c * (b - f) + e * (d - b));
	y = (float(1) / float(2)) * ((pow(a, 2) + pow(b, 2)) * (e - c) + (pow(c, 2) + pow(d, 2)) * (a - e) + (pow(e, 2) + pow(f, 2)) * (c - a)) / (b * (e - c) + d * (a - e) + f * (c - a));
    radius = sqrt((a - x) * (a - x) + (b - y) * (b - y));


	// Outputs the center and radius of circle:
	cout << "The center of the circle is at (" << x << ", " << y << ")" << endl;
	cout << "The radius of the circle is " << radius << endl;


	// Uses type casting to convert float values to int. The addition of 0.5 is so the compiler will round up.
	ai = int(a + 0.5);
	bi = int(b + 0.5);
	ci = int(c + 0.5);
	di = int(d + 0.5);
	ei = int(e + 0.5);
	fi = int(f + 0.5);
	xi = int(x + 0.5);
	yi = int(y + 0.5);
	iradius = int(radius + 0.5);


	// Uses the graphics library to draw the circle, center of circle, and the location of the three observers:
	initwindow(800, 600, "Lighthouse Location Calculator");
	circle(xi, yi, iradius);
	circle(xi, yi, 5);
	circle(ai, bi, 5);
	circle(ci, di, 5);
	circle(ei, fi, 5);

	getch();
	closegraph();


	return 0;
}

