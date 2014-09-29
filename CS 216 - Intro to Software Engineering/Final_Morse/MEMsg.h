/*
 * author: ashkan.hosseini@uky.edu (Ashkan Hosseini)
 *
 * File Summary: 
 *   Will read input from the user (keyboard input) and store
 *   it in a string. We then call the isEnglish method of MEMsg 
 *   to determine whether that input is Morse or English. 
 *   Once we have figured out whether the input is morse or 
 *   english, we will translate and output the result.
 *
 * Associated files:
 *   driver.cc
 *   MEMsg.h
 *   MEMsg.cc
 *   makefile
 *   mytests
 *   testdata.txt
 *
 * Build:
 *   makefile
 *   target: runme
 *
 * Caveats:
 *   There is some odd bits with figuring out when to output # for space
 *   when translating from english to morse. For example if you do space
 *   space space, you will get two #. However I think we shouldn't get anything
 *
 * Acknowledgement:
 *   cplusplus.com to learn about maps and toupper function
 *
 * Key Notes and Reflection:
 *   The largest problem with this assignment was to figure out how to break
 *   up the english and morse inputs for examinations. I originally had planned
 *   to transform the string to cstring, tokenize, translate, then add everything
 *   into one string. However I found that if I just look at character by character
 *   it is more efficient. Because of this decision however, there are some parts
 *   in the code that are probably not needed or are repeated or I could have done better.
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
	static void initTranslation();


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


	//Contructor and Deconstructor:
	MEMsg();
	~MEMsg();
};

#endif
