/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary: 
 *   Will read input from the user (keyboard input) and store
 *   it in a string. We then call the isEnglish method of MEMsg 
 *   to determine whether that input is Morse or English. 
 *   Once we have figured out whether the input is morse or 
 *   english, we will translate and print out the result. Then
 *   call send() and receive to write/read from file in binary
 *   and output what you wrote/read when verbose is set to true
 *
 * Associated files:
 *   driver.cc
 *   testDriver.cc
 *   MEMsg.h
 *   MEMsg.cc
 *   makefile
 *
 * Build:
 *   makefile
 *   target: runme
 *     Will use driver.cc and act similar to morseD2
 *     except now it calls send() and receive() with
 *     verbose set to false
 *   target: testme
 *     Uses testDriver.cc that only takes one morse
 *     input and writes/sends to file message.bin
 *     with parameter verbose of send() and receive()
 *     set to true
 *
 * Caveats:
 *   If anything other than dot, dash, space, or # is found, it is
 *   considered english. Otherwise it is morse. The caveat is that
 *   if we find for example ------- it is still considered morse.
 *   For send() receive(), error such as --- h .. is only handled
 *   by replacing h with a space and not some sort of error message
 *
 * Acknowledgement:
 *   cplusplus.com to learn about the different stl containers and other syntax
 *
 *
 */

#ifndef MEMSG_H
#define MEMSG_H

#include <string>
#include <map>

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
	static std::map<char, std::string> englishTranslation;
	//Holds English to Morse translation information
	static std::map<std::string, char> morseTranslation;


	//Initializes morseTranslation and englishTranslation
	static void initStaticVariables();


	/* Analys string to see if it is English
	 *  Parameters:
	 *   const string& userInput - the string to analys
	 *  Return:
	 *   true if English, false if Morse
	 */
	static bool isEnglish( const std::string& userInput );


	/* Translate English to Morse
	 *  Parameters:
	 *   const string& english - the English string to translate
	 *  Return:
	 *   the converted english in Morse as type string
	 */
	static std::string toMorse( const std::string& english );


	/* Translate Morse to English
	 *  Parameters:
	 *   const string& morse - the Morse string to translate
	 *  Return:
	 *   the converted morse in English as type string
	 */
	static std::string toEnglish( const std::string& morse );

	/* Writes a Morse message in binary to the file name by filename
	 *  Parameters:
	 *   string filename - the name of the file to save to
	 *   const& morse - Morse message to convert to binary
	 *   bool verbose - Whether to output contents of file in hex and binary
	*/
	static void send( std::string filename, const std::string& morse, bool verbose );

	/*
	 * Reads a file written by send() and converts it to Morse
	 *  Parameters:
	 *   string filename - the name of the file to read from
	 *   bool verbose - Whether to output contents of file in hex and binary
	 *  Return:
	 *   Convereted binary as Morse message
	*/
	static std::string receive( std::string filename, bool verbose );


	//Contructor and Deconstructor:
	MEMsg();
	~MEMsg();
};

#endif
