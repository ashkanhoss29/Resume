/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary: 
 *   Will read input from the user (keyboard input) and store
 *   it in a string. We then call the isEnglish method of MEMsg 
 *   to determine whether that input is Morse or English. 
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
	//Holds Morse to English translation information.
	static map<char, string> englishTranslation;
	//Holds English to Morse translation information
	static map<string, char> morseTranslation;


	//Initializes morseTranslation and englishTranslation
	static void initStaticVariables();


	/* Analys string to see if it is English
	 *  Parameters:
	 *   const string& userInput - the string to analys
	 *  Return:
	 *   true if English, false if Morse
	 */
	static bool isEnglish( const string& userInput );


	/* Translate English to Morse
	 *  Parameters:
	 *   const string& english - the English string to translate
	 *  Return:
	 *   the converted english in Morse as type string
	 */
	static string toMorse( const string& english );


	/* Translate Morse to English
	 *  Parameters:
	 *   const string& morse - the Morse string to translate
	 *  Return:
	 *   the converted morse in English as type string
	 */
	static string toEnglish( const string& morse );

	/* Writes a Morse message in binary to the file name by filename
	 *  Parameters:
	 *   string filename - the name of the file to save to
	 *   const& morse - Morse message to convert to binary
	*/
	static void send( string filename, const string& morse );

	/*
	 * Reads a file written by send() and converts it to Morse
	 *  Parameters:
	 *   string filename - the name of the file to read from
	 *  Return:
	 *   Convereted binary as Morse message
	*/
	static string receive( string filename );


	//Contructor and Deconstructor:
	MEMsg();
	~MEMsg();
};

#endif