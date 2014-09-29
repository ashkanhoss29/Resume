/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary:
 *   Driver will act similar to the other morse assignments, only
 *   this time it will call send() and receive() for testing. For
 *   an English input, the Morse output is what is written and then
 *   read from the file.
 *
 * Note:
 *   The main header can be found in the file MEMsg.h 
 *   where we have been asked to place it.
 *
 */

#include <iostream>
#include "MEMsg.h"

using namespace std;

int main()
{
	string userInput = "";
	string filename = "data.bin";

	//Initialize our maps/translations
	MEMsg::initStaticVariables();

	//Reads for input from user until eof is detected
	while(getline(cin, userInput))
	{
		//Figure out whether input is morse or english:
		if(MEMsg::isEnglish(userInput))
		{
			MEMsg::send(filename, MEMsg::toMorse(userInput), false);
			cout << MEMsg::receive(filename, false) << endl;
		}
		else
		{
			cout << MEMsg::toEnglish(userInput) << endl;
		}
	}


	return 0;
}
