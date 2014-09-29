//Ashkan Hosseini
//Section - 006
//Date: 4/13/10

#include "Map.h" //Map class specification

int main()
{
	Map m; //Map object created
	Sector s; //Sector object created
	string filename; //used to store the users input for file name


	cout << "Enter the file name: "; //asks the user to enter a file name
	cin >> filename; //stores the users input


	m.fillVector(filename.c_str()); //member function fillVector will open and read the file specified by the user and use the contents to created a 2d vector of Sectors
	cout << endl;

	cout << "Test for printSectors member function:" << endl;
	m.printSectors(); //member function printSector is called
	cout << endl;


	cout << "Test for findByLocation member function:" << endl;
	s = m.findByLocation(2, 1); //Sector object s is set to the value of the Sector that is in sectorVector at index 2 and 1
	cout << s.getName() << endl << endl; //the name of the Sector is output to the screen


	cout << "Test for findByName member function:" << endl;
	s = m.findByName("a-1"); //member function will look through sectorVector and will try find the Sector that has the same name as a-1
	cout << s.getName() << endl;
	s = m.findByName("r-8"); //member function will look through sectorVector and will try find the Sector that has the same name as r-8
	cout << s.getName() << endl << endl;


	cout << "Test for largestSize member function:" << endl;
	s = m.largestSize(); //the Sector with the largest size is returned and stored in s
	cout << s.getSize() << endl; //the size of s is output to the screen

	return 0;
}