//Ashkan Hosseini
//Section - 006
//Date: 4/13/10

#ifndef MAP_H
#define MAP_H

#include "Sector.h" //Sector class specification
#include <vector> //used for 2d vector sectorVector
#include <iostream> //used in map.cpp and main.cpp


//A class is used as an ADT because I believe it is the most practical method of solving the problem specified for this program.
//It will allow easy access to functions while maintaining security by hiding variables.
   //For example I used a 2d vector to store all of the Sector objects.  This vector is hidden from the user but with the use of
   //a class the user can easily specify by several means which Sector they want to access.
//Personal experience with such a method also allows this to be more efficiant than other ADT's

class Map
{
public:
	Map(); //constructor
	~Map(); //deconstructor
	void fillVector(string fileName); //will open the file, fill the 2d vector with appropriate 
	Sector findByName(string name); //the Sector with the same name as parameter name is returned
	Sector largestSize(); //the Sector with the largest size is returned
	Sector findByLocation(int row, int column); //the Sector in sectorVector at the index of parameters row and column is returned
	void printSectors(); //the Sectors stored in sectorVector are printed to the screen in order

private:
	vector< vector<Sector> > sectorVector; //this variable will store all of the sectors of a map
};

#endif