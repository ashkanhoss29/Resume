//Ashkan Hosseini
//Section - 006
//Date: 4/30/10

#ifndef MAP_H
#define MAP_H

#include "Sector.h" //Sector class specification
#include <vector> //used for 2d vector sectorVector
#include <iostream> //used in map.cpp and main.cpp


//This ADT is used because it will let us store the vector of Sector pointers (Sectors in path) while also storing flags
	//Flags used: lastDirectionTaken - this flag is used to eliminate loops (going east, then west again, then east again, etc.)
//The negative side to this ADT is that the implementation code using this will look confusing because we have a vector inside
struct path
{
	int lastDirectionTaken; //used to make sure the path does not go backwards
	vector< Sector* > sectorInPath; //stores pointers to sectors traveled
};

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
	void fillVector(string mapFileName, string sizeFileName); //will open the file, fill the 2d vector with appropriate 
	Sector* findByName(string name); //the Sector with the same name as parameter name is returned
	Sector largestSize(); //the Sector with the largest size is returned
	Sector findByLocation(int row, int column); //the Sector in sectorVector at the index of parameters row and column is returned
	void printSectorsByName(); //the Sectors stored in sectorVector are printed to the screen in alphabetical order
	void printSectorsBySize(); //prints to console the Sectors from largest to smallest
	void pathFinder(); //will find a path from Sector with size 0 to Sector with size 100

private:
	vector< vector<Sector> > sectorVector; //this variable will store all of the sectors of a map
	vector< Sector* > sectPtrs; //will store Sectors by size from largest to smallest (notice that printBySize must be called in order for pathFinder() to work)
	Sector noSuchSector; //used for findByName() so that if no such name is found, this is returned
	vector< path > allPaths; //this vector will store the paths created. last path indicates correct path from 0 to 100
};

#endif