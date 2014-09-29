// Ashkan Hosseini
// Section 002
// 12/09/09
// ashkan.hosseini@uky.edu


/*****Program Purpose*****/
/*  The purpose of this program is to create a guessing game.
	This program will generate a table that is 4x4, with a total of 16 cards.
	The user flips two cards:
		If the two cards are the same they stay flipped over.
		If the two cards are not the same they are flipped back.
	The user wins when all the cards are flipped.
*/


/*****Libraries*****/
#include <iostream> // used for input and output from and to the console
#include <iomanip> // uses function setw() to make sure table is even
#include <string> // used for variables with type string
#include <fstream> // used to read in strings from a text file
#include <cstdlib> // random numbers
#include <ctime> // time() used as the seed of random number to generate a random number based on the time.
#include "graphics.h" // will use delay function to create an animation of the grid appearing


using namespace std;
const int num = 16; // will be used to set the size of arrays to 16.  Also used in some loops


/*****Function Prototypes*****/
/*  The following function will get 8 strings from a file.
	The fucntion will duplicate the 8 strings so that there are a total of 16 strings
	Afterwards the 16 strings are randomly swapped
*/
void getgrid(string arr[]);


//The following function will draw a set of 56 dashes (---- etc.)
void drawbar();


/*  This function will output to the console a 4x4 table.
	The function will display either the position number, or the string of that 
	position based on two conditions:
		The first of these conditions is whether or not the user has entered that position as their choice
		The second condition is if the user has gotten a match
*/
void displaygrid(string arr[], bool arr_check[]);


/*  This function will get the users choice
	It will check to see if the users entered choice is valid or not:
		The users choice is not valid if the position has been entered befored
		The users choice is also not valid if they enter a value that is out of bounds
*/
int getchoice(bool arr_check[]);



/*****Main Function*****/
int main()
{
	string arr[num]; // array will store strings in 16 positions
	string enter; // used to get user to press enter (see line 97)
	bool arr_check[num] = {0}; // used to determine whether or not displaygrid() should display, at a given position, the postion number or string
	int p1, p2; // used to store users input for their guess (the position of array)
	int ct = 0; // used to count how many correct guesses the user has gotten
	int ct2 = 0; // used to count the total times the user has guessed

	cout << "How's Your Memory?" << endl;
	cout << "how long will it take you to match all 16 entries? " << endl << endl;

	getgrid(arr); // gets the strings from a file

	//This loop will break when the user has gotten 16 correct guesses (all correct):
	while(ct < num-1)
	{
		displaygrid(arr, arr_check); // output to the console the grid

		p1 = getchoice(arr_check); // get and store the users guess into variable p1
		arr_check[p1] = true; // make the position of array the user guessed to true
		displaygrid(arr, arr_check); // display the grid, this time showing the users first guess

		p2 = getchoice(arr_check); // get and store the users guess into variable p2
		arr_check[p2] = true; // make the position of array the user guessed to true
		displaygrid(arr, arr_check); // display the grid, now showing the users first and second guess

		//This if statement will check to see if the strings at the users guessed positions are the same
		if(arr[p1] == arr[p2])
		{
			cout << "You got a match!" << endl; // tells the user they got a match 
			ct = ct + 2; // adds 2 to ct, which is then used to break or keep the loop going
		}
		//If the strings do not match, the two positions the user guessed are flipped back
		else
		{
			arr_check[p1] = false;
			arr_check[p2] = false;
		}


		// The following gets the user to press enter:
		cout << "Press enter";
		getline(cin, enter);
		getline(cin, enter);

		system("cls"); // will clear the console
		
		ct2++; // adds to ct2 so program knows how many turns the user has taken
	}

	displaygrid(arr, arr_check); // displays the final grid after the user has won, showing all the strings
	cout << "It took you " << ct2 << " turns to solve it!" << endl; // tells the user how many turns they took


	return 0;
}



/*****Functions*****/

void drawbar()
{
	cout << "--------------------------------------------------------" << endl;
}



void getgrid(string arr[])
{
	int ct = 0; // used for loop and array positions
	int randnum, randnum2; // used to generate two random numbers for swapping of strings
	string file_name; // used to store users input of file name as type string
	string temp; // used for swapping of strings
	ifstream infile; // used to read in strings from a file


	//The following will get the user to input the name of their file:
	cout << "Enter filename ";
	getline(cin, file_name);
	infile.open(file_name.c_str());
	cout << endl;


	// Seed for random numbers. Uses time() for more randomness:
	srand(time(NULL));


	infile >> arr[ct]; // initial read from file

	// Loop used to read in 7 more strings from file:
	while (ct < num)
	{
		arr[ct + 1] = arr[ct]; // dublicates strings so there are a total of 16 strings in program
		ct = ct + 2;
		infile >> arr[ct]; // reads in strings from file
	}

	infile.close(); // closes the file

	// Loop used to randomly swap the strings of the 16 positions of the array:
	for (int p = 0; p < 450; p++)
	{
		randnum = rand() % 7;
		randnum2 = rand() % 16;
		temp = arr[randnum2];
		arr[randnum2] = arr[randnum];
		arr[randnum] = temp;
	}
}



void displaygrid(string arr[], bool arr_check[])
{
	int ct = 0;
	int ct2;
	int p = 0;

	drawbar();

	// The following set of loops will output to the console a 4x4 grid
	// Based on conditions, the loops will output either strings or position numbers
	while(ct < 4)
	{
		cout << "|";
		ct2 = 0;
		while(ct2 < 4)
		{
			delay(25); // delays output of each square by 25 milliseconds
			if(arr_check[p])
				cout << setw(10) << arr[p];
			else
				cout << "  ***" << setw(2) << p+1 << "***";
			cout << setw(4) << "|";
			p++;
			ct2++;
		}
		cout << endl;
		ct++;
	}

	drawbar();
}



int getchoice(bool arr_check[])
{
	int p;
	bool check = false;


	// This loop will get user input and check to see if that input is valid:
	while (!check)
	{
		cout << "Enter your choice (1-16) ";
		cin >> p;
		p--;

		/* Users input is only valid if the card they've chosen has not 
		   been matched and if their chosen number is in the range of 1-16 */
		if(arr_check[p] == true || p < 0 || p > num-1)
		{
			cout << "Enter a valid choice!" << endl;
		}
		else
		{
			check = true; // if the user enteres a valid number, check becomes true to end the loop
		}
	}

	return p; // returns users input
}