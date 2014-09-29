/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary:
 *   Test driver that is used with target testme. Will only
 *   take one input and output with send and recieve verbose
 *   set to true, which means they will output the binary
 *   and hex values of the bytes that are written or read
 *   from the file message.bin
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
	string userInput;
	string filename = "message.bin";

	//Initialize our maps/translations
	MEMsg::initStaticVariables();

	//Reads for input from user until eof is detected
	getline(cin, userInput);

	//Figure out whether input is morse or english:
	if(MEMsg::isEnglish(userInput))
	{
		MEMsg::send(filename, MEMsg::toMorse(userInput), true);
	}
	else
	{
		MEMsg::send(filename, userInput, true);
	}

	cout << MEMsg::receive(filename, true) << endl;

	return 0;
}
