/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary: 
 *   Will read input from the user (keyboard input) and store
 *   it in a string. We then call the isEnglish method of MEMsg 
 *   to determine whether that input is morse or english. 
 *   Once we have figured out whether the input is morse or 
 *   english, we will output Morse or English with cout.
 *
 * Associated files:
 *   driver.cc
 *   MEMsg.h
 *   MEMsg.cc
 *   makefile
 *
 * Build:
 *   makefile
 *   target: runme
 *
 * Caveats:
 *   If anything other than dot, dash, space, or # is found, it is
 *   considered english. Otherwise it is morse. The caveat is that
 *   if we find for example -------, it is still considered morse
 *
 * Acknowledgement:
 *   cplusplus.com to learn about the different stl containers
 *
 * Key Notes and Reflection:
 *   Two big decisions that I made were in regards to stl container
 *   and static vs instance methods and members. I learned a lot about
 *   both and why they would be used.
 *
 */

#ifndef MEMSG_H
#define MEMSG_H

#include <string>
#include <map>

using namespace std;


/*
 * class MEMsg
 *
 * Reasoning for static methods and members:
 *   Considering we don't need objects/instances of this class and
 *   we only use it as a container of sorts, we use static methods
 *   members.  It is particularly important for the translation
 *   container "morseTranslation" to be static since multiple copies
 *   would take too much space for no reason.  Specifically regarding
 *   the methods, they are static so we don't need to create an
 *   instance of our class in order to use them or the translation
 *
 * Reasoning for using map stl container:
 *   A map container is used since it allows us to easily store
 *   our translation in pairs. For example A is paired with .- so
 *   whether we need to convert A to .- or .- to A, we can quickly
 *   look at our map container and find out our translation either way
 */
class MEMsg
{
public:
	//Holds translation information. See above for more info
	static map<char, string> morseTranslation;

	static void initMorseTranslation();

	/* Analysis string to see if it is english
	 *  Parameters:
	 *   const string& userInput - the string to analys
	 *  Return:
	 *   true if english, false if morse
	 */
	static bool isEnglish( const string& userInput );


	/* Translates english to morse
	 *  Parameters:
	 *   const string& english - the english string to translate
	 *  Return:
	 *   the converted english in morse as type string
	 */
	static string toMorse( const string& english );


	/* Translates english to morse
	 *  Parameters:
	 *   const string& mosre - the morse string to translate
	 *  Return:
	 *   the converted morse in english as type string
	 */
	static string toEnglish( const string& morse );


	//Contructor and Deconstructor:
	MEMsg();
	~MEMsg();
};

#endif