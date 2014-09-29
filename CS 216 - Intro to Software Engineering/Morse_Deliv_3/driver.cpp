/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary:
 *   Driver that will test MEMsg.  Will get morse or english
 *   input from user and identify it as morse or english by
 *   calling MEMsg::isEnglish().  Once that is done, we will
 *   output the result and read input again until \n is found
 *
 * Note:
 *   The main header can be found in the file MEMsg.h 
 *   where we have been asked to place it.
 *
 */

#include <iostream>
#include "MEMsg.h"

int main()
{
	string userInput = "... - --#-..- ...-"; //stores one line of user input

	//Initialize our maps/translations
	MEMsg::initStaticVariables();

	MEMsg::send("data.bin", userInput);
	cout << MEMsg::receive("data.bin") << endl;

	//Reads for input from user until eof is detected
	while(getline(cin, userInput))
	{
		//Figure out whether input is morse or english:
		if(MEMsg::isEnglish(userInput))
		{
			cout << MEMsg::toMorse(userInput) << endl;
		}
		else
		{
			cout << MEMsg::toEnglish(userInput) << endl;
		}
	}

	return 0;
}