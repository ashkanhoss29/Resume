//Ashkan Hosseini
//Section - 006
//Date: 4/30/10

#ifndef SECTOR_H
#define SECTOR_H

#include <string> //string is used to store the name of the Sector

using namespace std;

class Sector
{
public:
	Sector(); //constructor
	~Sector(); //deconstructor

	//Get and Set functions for variable size
	void setSize(int s);
	int getSize();

	//Get and Set functions for non pointer variables:
	void setName(string n);
	string getName();
	void setDirections(int d);
	int getDirections();
	void setDirectionTraveled(int direction);
	int getDirectionTraveled();

	//Functions to set the pointers of each direction:
	void setNorthLink(Sector* link);
	void setEastLink(Sector* link);
	void setSouthLink(Sector* link);
	void setWestLink(Sector* link);

	//Functions that will return the value of the pointers of each direction:
	Sector* getNorthLink();
	Sector* getEastLink();
	Sector* getSouthLink();
	Sector* getWestLink();

private:
	string name; //name of sector
	int directions; //the directions in which the Sector points to is stored in this variable
	int size; //size of sector
	int directionTraveled; //used to store direction traveled in path

	//Pointers to the four directions that the sector points to:
	Sector* northLink;
	Sector* eastLink;
	Sector* southLink;
	Sector* westLink;
};

#endif