//Ashkan Hosseini
//Section - 006
//Date: 4/30/10

#include "Map.h" //Map class specification
#include <fstream> //used to read from file
#include <sstream> //used to store a single line read from file

//Constructor:
Map::Map()
{
}


//Deconstructor:
Map::~Map()
{
}



//This member function will read the file and store the Sectors that the file specifies in a 2d vector of Sectors (includes setting the name, size, and directional pointers):
void Map::fillVector(string mapFileName, string sizeFileName)
{
	ifstream infile, infile2; //stores the contents of the file
	infile.open(mapFileName.c_str()); //opens the file
	string line; //stores one line of the file

	//The following set of code will loop through the file and fill sectorVector according to the specifications of the file:
	getline(infile, line); //the firest line of the file is stored into variable line
	while(infile)
	{	
		string name; //stores the name of the Sector
		int directions; //stores the hexadecimal number that specifies which directions the Sector points to

		vector<Sector> tempSectorVector; //a temp vector of Sectors that stores the Sectors of one line
		Sector tempSector; //a temp Sector object that will be stored into tempSectorVector after it stores the name and direction read in from the file
		istringstream istringStream(line); //the first line that was stored into line is now stored into istringStream so we can store items which are seperated by spaces into variables name and directions
		istringStream >> name; //the first name is stored into variable name
		while(istringStream)
		{

			istringStream >> hex; //this line is so the next item in the stream is stored as int type
			istringStream >> directions; //the hexadecimal number in the file is stored in variable directions

			tempSector.setName(name); //the string stored in variable name is stored into tempSector
			tempSector.setDirections(directions); //int value stored in directions is stored in tempSector

			tempSectorVector.push_back(tempSector); //tempSector is stored into tempSectorVector
			istringStream >> name; //the name of the next Sector is stored into variable name
		}

		sectorVector.push_back(tempSectorVector); //all of the Sectors in one line are stored into sectorVector
		tempSectorVector.clear(); //tempSectorVector is cleared for use with the next line in the file

		getline(infile, line); //the next line in the file is stored into variable line
	}
	infile.close(); //will close the file


	//The following will open the file containing the sizes of the sectors and will assign those sizes to their correct sectors:
	infile2.open(sizeFileName.c_str());
	getline(infile2, line);
	string name; //The size in the file related to this name is stored in the Sector with similar name
	int size; //used to store size read from file
	while(infile2)
	{
		istringstream istringStream(line);
		istringStream >> name; //name is stored
		istringStream >> dec;
		istringStream >> size; //size is stored

		findByName(name)->setSize(size); //size read from file is stored in the Sector with the same name as the one corresponding to size

		//sectPtr = findByName(name);
		//sectPtr->setSize(size);

		getline(infile2, line);
	}
	infile2.close(); //file with sizes is closed


	//This set of loops will set the appropriate pointers of each Sector:
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			//Each if statement uses bitwise comparisons to decide whether or not a pointer should be set for each direction
				//A check will also be made to make sure Sector doesn't point outside the map
			if(sectorVector[x][y].getDirections() & 1)
			{
				if(x != 0)
				{
					sectorVector[x][y].setNorthLink(&sectorVector[x-1][y]);
				}
				else
				{
					sectorVector[x][y].setNorthLink(NULL);
				}
			}
			if(sectorVector[x][y].getDirections() & 2)
			{
				if(y != sectorVector[1].size()-1)
				{
					sectorVector[x][y].setEastLink(&sectorVector[x][y+1]);
				}
				else
				{
					sectorVector[x][y].setEastLink(NULL);
				}
			}
			if(sectorVector[x][y].getDirections() & 4)
			{
				if(x != sectorVector.size()-1)
				{
					sectorVector[x][y].setSouthLink(&sectorVector[x+1][y]);
				}
				else
				{
					sectorVector[x][y].setSouthLink(NULL);
				}
			}
			if(sectorVector[x][y].getDirections() & 8)
			{
				if(y != 0)
				{
					sectorVector[x][y].setWestLink(&sectorVector[x][y-1]);
				}
				else
				{
					sectorVector[x][y].setWestLink(NULL);
				}
			}
		}
	}
}



//The following member function will return the Sector that has the same name as the parameter name:
Sector* Map::findByName(string name)
{
	//The following set of loops will go through sectorVector and compare parameter name with the names of each Sector stored in sectorVector
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{ 
			//if the string of parameter name is the same as the string stored in variable name of the Sector that is stored in sectorVector at x and y, that Sector is returned 
			if(sectorVector[x][y].getName() == name)
			{
				return &sectorVector[x][y]; //pointer to the Sector with same name as parameter name is returned
			}
		}
	}

	//If parameter name does not match the name of any of the Sectors in sectorVector, a Sector with the name "No such Sector" is returned to indicate that there was no such Sector with the name entered by user:
	noSuchSector.setName("No such Sector");
	return &noSuchSector;
}



//This member function will return the Sector at the index of parameters row and column:
Sector Map::findByLocation(int row, int column)
{
	return sectorVector[row][column];
}


//This member function will return the Sector with the largest size:
Sector Map::largestSize()
{
	int maxSize = 0; //used to store the maximum size so far
	
	//The following will store the index of sectorVector where the maximum size was read:
	int row = 0;
	int column = 0;

	//The following set of loops will find the Sector with the maximum size and store its location in sectorVector
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			if(sectorVector[x][y].getSize() > maxSize)
			{
				row = x;
				column = y;
			}
		}
	}

	return sectorVector[row][column]; //once all of sectorVector has been looked at, the index at which the Sector with the largest size was found is used to return that same Sector
}



//The following member function will output to the screen the Sectors in alphabetical order by name:
void Map::printSectorsByName()
{	
	vector<Sector*> sectPtrs; //vector of Sector pointers is used to store Sectors in order without moving actual Sectors
	Sector* tempPtr; //used for swapping and temp. placeholder for putting pointers into sectPtrs

	//sectPtrs vector is assigned pointers to all Sectors
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			tempPtr = &sectorVector[x][y]; //address of Sector at index xy is stored in tempPtr
			sectPtrs.push_back(tempPtr); //pointer is pushed back into sectPtrs vector
		}
	}

	//This set of loops will use selection sort to sort Sectors in alph. order:
	for(unsigned int x = 0; x < sectPtrs.size(); x++)
	{
		int maxString = 0;

		for(unsigned int y = 1; y < sectPtrs.size()-x; y++)
			if(sectPtrs[maxString]->getName() < sectPtrs[y]->getName())
				maxString = y;

		tempPtr = sectPtrs[maxString];
		sectPtrs[maxString] = sectPtrs[sectPtrs.size()-x-1];
		sectPtrs[sectPtrs.size()-x-1] = tempPtr;
	}

	//Sorted Sectors (via Sector pointers) are printed to the screen in proper format:
	for(unsigned int x = 0; x < sectPtrs.size(); x++)
	{
		cout << "Name:" << sectPtrs[x]->getName()
			<< " Size:" << sectPtrs[x]->getSize()
			<< " Outlets :";

		//If a particular pointer is equal to NULL, we know that it does not point to anything, thus it can be skipped
		//If a particular direction does point to a Sector, that Sector's name is added to the string
		if(sectPtrs[x]->getNorthLink() != NULL)
		{
			tempPtr = sectPtrs[x]->getNorthLink();
			cout << "N(" << tempPtr->getName() << ") ";
		}
		if(sectPtrs[x]->getEastLink() != NULL)
		{
			tempPtr = sectPtrs[x]->getEastLink();
			cout << "E(" << tempPtr->getName() << ") ";
		}
		if(sectPtrs[x]->getSouthLink() != NULL)
		{
			tempPtr = sectPtrs[x]->getSouthLink();
			cout << "S(" << tempPtr->getName() << ") ";
		}
		if(sectPtrs[x]->getWestLink() != NULL)
		{
			tempPtr = sectPtrs[x]->getWestLink();
			cout << "W(" << tempPtr->getName() << ") ";
		}
		cout << endl;
	}
}

//This function is similar to printByName function.  For more info please refer to that function:
void Map::printSectorsBySize()
{
	Sector* tempPtr;

	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			tempPtr = &sectorVector[x][y];
			sectPtrs.push_back(tempPtr);
		}
	}

	//Sectors are sorted using selection sort:
	for(unsigned int x = 0; x < sectPtrs.size(); x++)
	{
		int maxString = 0;

		for(unsigned int y = 1; y < sectPtrs.size()-x; y++)
			if(sectPtrs[maxString]->getSize() > sectPtrs[y]->getSize()) //The > operator is the only bit different than printByName()
				maxString = y;

		tempPtr = sectPtrs[maxString];
		sectPtrs[maxString] = sectPtrs[sectPtrs.size()-x-1];
		sectPtrs[sectPtrs.size()-x-1] = tempPtr;
	}

	//Sorted Sectors are printed in proper format:
	for(unsigned int x = 0; x < sectPtrs.size(); x++)
	{
		cout << "Name:" << sectPtrs[x]->getName()
			<< " Size:" << sectPtrs[x]->getSize() << endl;
	}
}

void Map::pathFinder()
{
	path startPath; //initializes a path object to store information of the Sector with size 0 (or start of the path)
	startPath.sectorInPath.push_back(sectPtrs[sectPtrs.size()-1]); //this stores a pointer to the Sector with size 0 by looking at sectPtrs vector (which has the Sectors stored in order by size)
	allPaths.push_back(startPath); //startPath object of type path is stored in a vector of path's 
	bool pathFound = false; //this boolean variable will be set to true if a path is found (if the Sector pushed back into sectorInPath in path object has size of 100)
	int x = 0; // this will be a counter for the index of the vector of path's that will be evaluated

	//This while loop looks at the Sector in the last index of sectorInPath vector at index x of allPaths vector until it finds the correect path
	 //If statements will look at which directions the Sector being evaluated pointes to
		//If this Sector points to a particular direction, the path object being evaluated is dublicated in allPaths and the Sector in that direction is pushed back into sectorInPath of newly created path object
	while(!pathFound)
	{
		//There are a total of 4 if statements for each direction which will determine what the next Sector in the path is
			//Two conditions are checked:
				//1. We will see which directions the Sector at the particular index points to, so we know if we should add to the path
				//2. We will check to see whether or not the direction we are going was the previous location or not
					//This is done by evaluating the lastDirectionTaken flag in our path object
			//If these two conditions are true, we add the Sector which is in the particular direction to our path
		if((allPaths[x].sectorInPath[allPaths[x].sectorInPath.size()-1]->getDirections() & 1) && (allPaths[x].lastDirectionTaken != 4))
		{
			allPaths.push_back(allPaths[x]);
			allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->setDirectionTraveled(1); //This adds the value of the direction taken to the directionTaken variable of the Sector (before the new Sector is added to path object)
			allPaths[allPaths.size()-1].sectorInPath.push_back(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getNorthLink());

			//Will check to see if the latest Sector added has size of 100
				//If Sector added has size of 100, loop will exit
					//Since loop is exited in this fasion, we know that the last path in allPaths is the correct path
				//If Sector added is not 100, we will give the value of the direction taken to lastDirectionTaken inside path object
			if(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getSize() == 100)
			{
				pathFound = true;
				break;
			}
			else
			{
				allPaths[allPaths.size()-1].lastDirectionTaken = 1; //Direction last traveled is added so we don't loop
			}
		}
		if((allPaths[x].sectorInPath[allPaths[x].sectorInPath.size()-1]->getDirections() & 2) && (allPaths[x].lastDirectionTaken != 8))
		{
			allPaths.push_back(allPaths[x]);
			allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->setDirectionTraveled(2);
			allPaths[allPaths.size()-1].sectorInPath.push_back(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getEastLink());

			if(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getSize() == 100)
			{
				pathFound = true;
				break;
			}
			else
			{
				allPaths[allPaths.size()-1].lastDirectionTaken = 2;
			}
		}
		if((allPaths[x].sectorInPath[allPaths[x].sectorInPath.size()-1]->getDirections() & 4) && (allPaths[x].lastDirectionTaken != 1))
		{
			allPaths.push_back(allPaths[x]);
			allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->setDirectionTraveled(4);
			allPaths[allPaths.size()-1].sectorInPath.push_back(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getSouthLink());

			if(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getSize() == 100)
			{
				pathFound = true;
				break;
			}
			else
			{
				allPaths[allPaths.size()-1].lastDirectionTaken = 4;
			}
		}
		if((allPaths[x].sectorInPath[allPaths[x].sectorInPath.size()-1]->getDirections() & 8) && (allPaths[x].lastDirectionTaken != 2))
		{
			allPaths.push_back(allPaths[x]);
			allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->setDirectionTraveled(8);
			allPaths[allPaths.size()-1].sectorInPath.push_back(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getWestLink());

			if(allPaths[allPaths.size()-1].sectorInPath[allPaths[allPaths.size()-1].sectorInPath.size()-1]->getSize() == 100)
			{
				pathFound = true;
				break;
			}
			else
			{
				allPaths[allPaths.size()-1].lastDirectionTaken = 8;
			}
		}
		x++; //counter is incremented by one so the next path may be evaluated
	}

	//This for loop will go through the sectorInPath vector of the last path object inside allPaths and evaluate the Sectors stored inside.
	//For each Sector that is evaluated, its name and direction traveled is printed out to the screen
	for(unsigned int i = 0; i < allPaths[allPaths.size()-1].sectorInPath.size(); i++)
	{
		cout << allPaths[allPaths.size()-1].sectorInPath[i]->getName() << " " << allPaths[allPaths.size()-1].sectorInPath[i]->getDirectionTraveled() << endl;
	}
}