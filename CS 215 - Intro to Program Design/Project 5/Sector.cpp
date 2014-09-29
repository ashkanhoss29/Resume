//Ashkan Hosseini
//Section - 006
//Date: 4/30/10

#include "Sector.h" //Sector class specification


Sector::Sector()
{
	northLink = eastLink = southLink = westLink = NULL; //the pointers for each direction are set to NULL to indicate that they do not point to anything
	size = directionTraveled = 0;
	name = "";
}

Sector::~Sector()
{
}



void Sector::setName(string n)
{
	name = n; //value of parameter n is stored into variable name
}

string Sector::getName()
{
	return name; //value of variable name is returned
}



void Sector::setDirections(int d)
{
	directions = d; //value of parameter d is stored into variable directions
}

int Sector::getDirections()
{
	return directions; //value of direction is returned
}



void Sector::setSize(int s)
{
	size = s; //value stored in parameter s is stored into variable size
}

int Sector::getSize()
{
	return size; //value stored in size is returned
}

void Sector::setDirectionTraveled(int direction)
{
	directionTraveled = direction; //value stored in parameter direction is stored into variable directionTraveled
}

int Sector::getDirectionTraveled()
{
	return directionTraveled; //value stored in directionTraveled is returned
}


void Sector::setNorthLink(Sector *link)
{
	northLink = link; //value of parameter link is stored in northLink
}

void Sector::setEastLink(Sector* link)
{
	eastLink = link; //value of parameter link is stored in eastLink
}

void Sector::setSouthLink(Sector *link)
{
	southLink = link; //value of parameter link is stored in southLink
}

void Sector::setWestLink(Sector *link)
{
	westLink = link; //value of parameter link is stored in westLink
}



Sector* Sector::getNorthLink()
{
	return northLink; //value of variable northLink is returned
}

Sector* Sector::getEastLink()
{
	return eastLink; //value of variable eastLink is returned
}

Sector* Sector::getSouthLink()
{
	return southLink; //value of variable southLink is returned
}

Sector* Sector::getWestLink()
{
	return westLink; //value of variable westLink is returned
}