//Ashkan Hosseini
//Section - 006
//Date: 4/13/10

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
void Map::fillVector(string fileName)
{
	ifstream infile; //stores the contents of the file
	infile.open(fileName.c_str()); //opens the file
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



	//This set of loops will set the appropriate pointers of each Sector:
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			//Each if statement uses bitwise comparisons to decide whether or not a pointer should be set for each direction
			if(sectorVector[x][y].getDirections() & 1)
			{
				sectorVector[x][y].setNorthLink(&sectorVector[x-1][y]);
			}
			if(sectorVector[x][y].getDirections() & 2)
			{
				sectorVector[x][y].setEastLink(&sectorVector[x][y+1]);
			}
			if(sectorVector[x][y].getDirections() & 4)
			{
				sectorVector[x][y].setSouthLink(&sectorVector[x+1][y]);
			}
			if(sectorVector[x][y].getDirections() & 8)
			{
				sectorVector[x][y].setWestLink(&sectorVector[x][y-1]);
			}
		}
	}
}



//The following member function will return the Sector that has the same name as the parameter name:
Sector Map::findByName(string name)
{
	//The following set of loops will go through sectorVector and compare parameter name with the names of each Sector stored in sectorVector
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{ 
			//if the string of parameter name is the same as the string stored in variable name of the Sector that is stored in sectorVector at x and y, that Sector is returned 
			if(sectorVector[x][y].getName() == name)
			{
				return sectorVector[x][y];
			}
		}
	}

	//If parameter name does not match the name of any of the Sectors in sectorVector, a Sector with the name "No such Sector" is returned to indicate that there was no such Sector with the name entered by user:
	Sector sector;
	sector.setName("No such Sector");
	return sector;
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
void Map::printSectors()
{	
	vector<string> outputVector; //used to store string representation of the sectors in the format Name: Size: Outlets:

	//The following code will fill the string vector outputVector with strings representing each of the sectors by name, size, and what they point to:
	for(unsigned int x = 0; x < sectorVector.size(); x++)
	{
		for(unsigned int y = 0; y < sectorVector[x].size(); y++)
		{
			stringstream addStrings;
			addStrings << "Name:" << sectorVector[x][y].getName();
			addStrings << " Size:" << sectorVector[x][y].getSize();
			addStrings << " Outlets:";

			Sector* tempSectorPtr; //used to temp store the value of the pointer of a direction

			//If a particular pointer is equal to NULL, we know that it does not point to anything, thus it can be skipped
			//If a particular direction does point to a Sector, that Sector's name is added to the string
			if(sectorVector[x][y].getNorthLink() != NULL)
			{
				tempSectorPtr = sectorVector[x][y].getNorthLink();
				addStrings << "N(" << tempSectorPtr->getName() << ") ";
			}
			if(sectorVector[x][y].getEastLink() != NULL)
			{
				tempSectorPtr = sectorVector[x][y].getEastLink();
				addStrings << "E(" << tempSectorPtr->getName() << ") ";
			}
			if(sectorVector[x][y].getSouthLink() != NULL)
			{
				tempSectorPtr = sectorVector[x][y].getSouthLink();
				addStrings << "S(" << tempSectorPtr->getName() << ") ";
			}
			if(sectorVector[x][y].getWestLink() != NULL)
			{
				tempSectorPtr = sectorVector[x][y].getWestLink();
				addStrings << "W(" << tempSectorPtr->getName() << ") ";
			}

			outputVector.push_back(addStrings.str()); //the completed string for the Sector is stored in outputVector
		}
	}


	//The following code uses the selection sorting algorithm to sort the string vector:
	for(unsigned int x = 0; x < outputVector.size(); x++)
	{
		int maxString = 0;

		for(unsigned int y = 1; y < outputVector.size()-x; y++)
			if(outputVector[maxString] < outputVector[y])
				maxString = y;

		string tempString = outputVector[maxString];
		outputVector[maxString] = outputVector[outputVector.size()-x-1];
		outputVector[outputVector.size()-x-1] = tempString;
	}


	//This loop will go through the sorted string vector and output at each index the strings:
	for(unsigned int x = 0; x < outputVector.size(); x++)
	{
		cout << outputVector[x] << endl;
	}
}