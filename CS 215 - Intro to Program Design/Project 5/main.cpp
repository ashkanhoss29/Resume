//Ashkan Hosseini
//Section - 006
//Date: 4/30/10

#include "Map.h" //Map class specification

int main()
{
	Map m; //Map object created
	Sector s; //Sector object created
	string firstFileName, secondFileName; //used to store the users input for file name


	cout << "Enter map file name: "; //asks the user to enter a file name
	cin >> firstFileName; //stores the users input

	cout << "Enter sizes file name: ";
	cin >> secondFileName;


	m.fillVector(firstFileName.c_str(), secondFileName.c_str()); //member function fillVector will open and read the file specified by the user and use the contents to created a 2d vector of Sectors
	cout << endl << endl;

	cout << "Test for printSectorsByName member function:" << endl << endl;
	m.printSectorsByName(); //member function printSector is called
	cout << endl << endl;

	cout << "Test for printSectorsBySize member function:" << endl << endl;
	m.printSectorsBySize();
	cout << endl << endl;

	cout << "Test for findByLocation member function:" << endl;
	s = m.findByLocation(2, 1); //Sector object s is set to the value of the Sector that is in sectorVector at index 2 and 1
	cout << s.getName() << endl << endl; //the name of the Sector is output to the screen


	cout << "Test for findByName member function:" << endl;
	s = *m.findByName("q-45"); //member function will look through sectorVector and will try find the Sector that has the same name as a-1
	cout << s.getName() << endl;
	s = *m.findByName("r-8"); //member function will look through sectorVector and will try find the Sector that has the same name as r-8
	cout << s.getName() << endl << endl;


	cout << "Test for largestSize member function:" << endl;
	s = m.largestSize(); //the Sector with the largest size is returned and stored in s
	cout << s.getSize() << endl << endl; //the size of s is output to the screen

	m.pathFinder();

	return 0;
}