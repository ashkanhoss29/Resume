#include "MEMsg.h"
#include <sstream> //Allows us to add multiple strings into one string
#include <cctype>  //For uppercase transformation
#include <fstream>
#include <iostream>

using namespace std;

map<char, string> MEMsg::englishTranslation;
map<string, char> MEMsg::morseTranslation;

void MEMsg::initStaticVariables()
{
	englishTranslation['A'] = ".-";
	englishTranslation['B'] = "-...";
	englishTranslation['C'] = "-.-.";
	englishTranslation['D'] = "-..";
	englishTranslation['E'] = ".";
	englishTranslation['F'] = "..-.";
	englishTranslation['G'] = "--.";
	englishTranslation['H'] = "....";
	englishTranslation['I'] = "..";
	englishTranslation['J'] = ".---";
	englishTranslation['K'] = "-.-";
	englishTranslation['L'] = ".-..";
	englishTranslation['M'] = "--";
	englishTranslation['N'] = "-.";
	englishTranslation['O'] = "---";
	englishTranslation['P'] = ".--.";
	englishTranslation['Q'] = "--.-";
	englishTranslation['R'] = ".-.";
	englishTranslation['S'] = "...";
	englishTranslation['T'] = "-";
	englishTranslation['U'] = "..-";
	englishTranslation['V'] = "...-";
	englishTranslation['W'] = ".--";
	englishTranslation['X'] = "-..-";
	englishTranslation['Y'] = "-.--";
	englishTranslation['Z'] = "--..";
	englishTranslation['0'] = "-----";
	englishTranslation['1'] = ".----";
	englishTranslation['2'] = "..---";
	englishTranslation['3'] = "...--";
	englishTranslation['4'] = "....-";
	englishTranslation['5'] = ".....";
	englishTranslation['6'] = "-....";
	englishTranslation['7'] = "--...";
	englishTranslation['8'] = "---..";
	englishTranslation['9'] = "----.";
	englishTranslation['.'] = ".-.-.-";
	englishTranslation[','] = "--..--";
	englishTranslation['?'] = "..--..";
	englishTranslation['\''] = ".----.";
	englishTranslation['!'] = "-.-.--";
	englishTranslation['/'] = "-..-.";
	englishTranslation['('] = "-.--.";
	englishTranslation[')'] = "-.--.-";
	englishTranslation['&'] = ".-...";
	englishTranslation[':'] = "---...";
	englishTranslation[';'] = "-.-.-.";
	englishTranslation['='] = "-...-";
	englishTranslation['+'] = ".-.-.";
	englishTranslation['-'] = "-....-";
	englishTranslation['_'] = "..--.-";
	englishTranslation['"'] = ".-..-.";
	englishTranslation['$'] = "...-..-";
	englishTranslation['@'] = ".--.-.";

	morseTranslation[".-"] = 'A';
	morseTranslation["-..."] = 'B';
	morseTranslation["-.-."] = 'C';
	morseTranslation["-.."] = 'D';
	morseTranslation["."] = 'E';
	morseTranslation["..-."] = 'F';
	morseTranslation["--."] = 'G';
	morseTranslation["...."] = 'H';
	morseTranslation[".."] = 'I';
	morseTranslation[".---"] = 'J';
	morseTranslation["-.-"] = 'K';
	morseTranslation[".-.."] = 'L';
	morseTranslation["--"] = 'M';
	morseTranslation["-."] = 'N';
	morseTranslation["---"] = 'O';
	morseTranslation[".--."] = 'P';
	morseTranslation["--.-"] = 'Q';
	morseTranslation[".-."] = 'R';
	morseTranslation["..."] = 'S';
	morseTranslation["-"] = 'T';
	morseTranslation["..-"] = 'U';
	morseTranslation["...-"] = 'V';
	morseTranslation[".--"] = 'W';
	morseTranslation["-..-"] = 'X';
	morseTranslation["-.--"] = 'Y';
	morseTranslation["--.."] = 'Z';
	morseTranslation["-----"] = '0';
	morseTranslation[".----"] = '1';
	morseTranslation["..---"] = '2';
	morseTranslation["...--"] = '3';
	morseTranslation["....-"] = '4';
	morseTranslation["....."] = '5';
	morseTranslation["-...."] = '6';
	morseTranslation["--..."] = '7';
	morseTranslation["---.."] = '8';
	morseTranslation["----."] = '9';
	morseTranslation[".-.-.-"] = '.';
	morseTranslation["--..--"] = ',';
	morseTranslation["..--.."] = '?';
	morseTranslation[".----."] = '\'';
	morseTranslation["-.-.--"] = '!';
	morseTranslation["-..-."] = '/';
	morseTranslation["-.--."] = '(';
	morseTranslation["-.--.-"] = ')';
	morseTranslation[".-..."] = '&';
	morseTranslation["---..."] = ':';
	morseTranslation["-.-.-."] = ';';
	morseTranslation["-...-"] = '=';
	morseTranslation[".-.-."] = '+';
	morseTranslation["-....-"] = '-';
	morseTranslation["..--.-"] = '_';
	morseTranslation[".-..-."] = '"';
	morseTranslation["...-..-"] = '$';
	morseTranslation[".--.-."] = '@';
}

bool MEMsg::isEnglish( const string& userInput )
{

	//temp to hold single char from string for analysis
	char tempChar = ' ';

	//Loop through each char of userInput and analys for english
	for(unsigned int index = 0; index < userInput.length(); index++)
	{
		tempChar = userInput.at(index);
		
		//If char is anything but dot, dash, space, or #, it's considered english
		if(tempChar == '.' || tempChar == '-' || tempChar == ' ' || tempChar == '#')
		{
			//Continue
		}
		else
		{
			//char was not morse, return true (english)
			return true;
		}
	}

	//no english found, return false (morse)
	return false;
}

string MEMsg::toMorse( const string& English )
{
	//stub implementation
	string returnValue;

	if(English.size() < 1)
	{
		return " ";
	}
	else
	{
		stringstream addStrings; //store each character translation
		map<char, string>::iterator it;

		//Go through each english character and translate
		for(unsigned int counter = 0; counter < English.size(); counter++)
		{
			char upperChar = toupper(English.at(counter));
			//Add the translation to the stringstream
			//cout << englishTranslation.find(toupper(English.at(counter)))->second << " ";
			if(!(toupper(English.at(counter)) == ' '))
			{
				it = englishTranslation.find(upperChar);
				if(it != englishTranslation.end())
				{
					addStrings << it->second;
				}
				else
				{
					addStrings << "<error>" << English.at(counter) << "</error>";
				}
			}
			
			//addStrings << ' ';

			if(counter+1 < English.size())
			{
				if(toupper(English.at(counter+1)) == ' ')
				{
					addStrings << '#';
				}
				else if(!(upperChar == ' '))
				{
					addStrings << ' ';
				}
			}
		}

		//Convert the stringstream (all of the strings) into one string
		returnValue = addStrings.str();
	}

	//Return our completed translation (or space if nothing in Morse)
	return returnValue;
}

string MEMsg::toEnglish( const string& Morse )
{
	stringstream englishChars;
	stringstream oneString;
	string tempString;
	map<string, char>::iterator it;

	for(unsigned int counter = 0; counter < Morse.size(); counter++)
	{
		if(Morse.at(counter) == '.' || Morse.at(counter) == '-')
		{
			oneString << Morse.at(counter);
		}
		else if(Morse.at(counter) == '#')
		{
			englishChars << ' ';
		}

		if((counter+1 < Morse.size()) && oneString.str() != "")
		{
			if(Morse.at(counter+1) == '#' || Morse.at(counter+1) == ' ')
			{
				//Examine previous findings
				it = morseTranslation.find(oneString.str());

				if(it != morseTranslation.end())
				{
					englishChars << it->second;
				}
				else
				{
					englishChars << "<error>" << oneString.str() << "</error>";
				}

				oneString.str("");
			}
		}
		else if(oneString.str() != "")
		{
			//Examine previous findings
			it = morseTranslation.find(oneString.str());

			if(it != morseTranslation.end())
			{
				englishChars << it->second;
			}
			else
			{
				englishChars << "<error>" << oneString.str() << "</error>";
			}

			oneString.str("");
		}
	}

	tempString = englishChars.str();
	return tempString;
}


void MEMsg::send( string filename, const string& morse, bool verbose )
{
	//Contains mapping for Morse to binary
	//Should be in header as static, but not worth the trouble
	unsigned char bitMasks[4][3];

	bitMasks[0][0] = 0x40;
	bitMasks[0][1] = 0x80;
	bitMasks[0][2] = 0xC0;

	bitMasks[1][0] = 0x10;
	bitMasks[1][1] = 0x20;
	bitMasks[1][2] = 0x30;

	bitMasks[2][0] = 0x4;
	bitMasks[2][1] = 0x8;
	bitMasks[2][2] = 0xC;

	bitMasks[3][0] = 0x1;
	bitMasks[3][1] = 0x2;
	bitMasks[3][2] = 0x3;


	int numberOfBytes = morse.size()/4+1;
	if(morse.size()%4 != 0)
	{
		numberOfBytes++;
	}
	unsigned char* bytesToWrite = new unsigned char[numberOfBytes];
	int byteCounter = 1;
	string verboseBinary = "";

	bytesToWrite[0] = morse.size(); //Store how many bytes we used
	
	//Set each char/byte in the array to 0 (or 0000 0000 in bit terms)
	for(int i = 1; i < numberOfBytes; i++)
	{
		bytesToWrite[i] = 0x0;
	}

	//Go through the string. For each 1 in string, set bit at index
	for(unsigned int i = 0; i < morse.size(); i++)
	{
		//If our index reaches end of byte, use next byte
		if( (i%4 == 0) && (i != 0) )
		{
			byteCounter += 1;
		}
		
		//Assign the correspoding value for . - and # to our bytes
		if(morse.at(i) == '.')
		{
			bytesToWrite[byteCounter] |= bitMasks[i%4][0]; //set bit: 00 00 00 01
			if(verbose == true)
			{
				//cout << hex << (int)bitMasks[i%4][0] << " ";
				verboseBinary += "01";
			}
		}
		else if(morse.at(i) == '-')
		{
			bytesToWrite[byteCounter] |= bitMasks[i%4][1]; //set bit: 00 00 00 10
			if(verbose == true)
			{
				//cout << hex << (int)bitMasks[i%4][1] << " ";
				verboseBinary += "10";
			}
		}
		else if(morse.at(i) == '#')
		{
			bytesToWrite[byteCounter] |= bitMasks[i%4][2]; //set bit: 00 00 00 11
			if(verbose == true)
			{
				//cout << hex << (int)bitMasks[i%4][2] << " ";
				verboseBinary += "11";
			}
		}
		else
		{
			if(verbose == true)
			{
				verboseBinary += "00";
			}
		}
	}

	if(verbose)
	{
		cout << verboseBinary << endl;
		for(int i = 1; i < numberOfBytes; i++)
		{
			cout << "0x" << hex << (int)bytesToWrite[i] << " ";
		}
		cout << endl;
	}

	//Write our bytes to a file with name filename
	ofstream fout( filename.c_str(), ios::out | ios::binary );
	fout.write( (const char*)bytesToWrite, numberOfBytes );
	fout.close();
	delete[] bytesToWrite;
	bytesToWrite = NULL;
}


string MEMsg::receive( string filename, bool verbose )
{
	//See comment in send()
	unsigned char bitMasks[4][3];

	bitMasks[0][0] = 0x40;
	bitMasks[0][1] = 0x80;
	bitMasks[0][2] = 0xC0;

	bitMasks[1][0] = 0x10;
	bitMasks[1][1] = 0x20;
	bitMasks[1][2] = 0x30;

	bitMasks[2][0] = 0x4;
	bitMasks[2][1] = 0x8;
	bitMasks[2][2] = 0xC;

	bitMasks[3][0] = 0x1;
	bitMasks[3][1] = 0x2;
	bitMasks[3][2] = 0x3;

	unsigned char* bytesToRead = new unsigned char[512]; //holds bytes to read
	string morseMessage = ""; //holds the translated morse message for return
	string verboseBinary = "";

	//Open file with name filename, read bytes, and store in bytesToRead
	ifstream fin( filename.c_str(), ios::in | ios::binary );
	fin.read( (char*)bytesToRead, 512 );
	fin.close();

	unsigned int numberOfBits = 2*bytesToRead[0]; //amount of bits stored in the file
	unsigned int numberOfBytes = bytesToRead[0]; //number of bytes to read
	unsigned int bitsRead = 0; //keep track of number of bits read

	//Read binary two bit and convert to Morse
	for ( unsigned int i = 1; i <= numberOfBytes; i++)
	{
		//Outputs hex value of the bytes if verbose=true
		if(verbose)
		{
			cout << "0x" << hex << (int)bytesToRead[i] << " ";
		}

		for(int ctr = 0; ctr < 4; ctr++)
		{
			//Once we have read all the bits, exit the loops so we don't get trailing 00's
			if(bitsRead == numberOfBits)
			{
				break;
			}

			//Convert the bits to Morse based on mapping in bitMasks
			if((bytesToRead[i] & bitMasks[ctr][0]) && !(bytesToRead[i] & bitMasks[ctr][1]))
			{
				morseMessage += ".";
				verboseBinary += "01";
			}
			else if(bytesToRead[i] & bitMasks[ctr][1] && !(bytesToRead[i] & bitMasks[ctr][0]))
			{
				morseMessage += "-";
				verboseBinary += "10";
			}
			else if(bytesToRead[i] & bitMasks[ctr][2])
			{
				morseMessage += "#";
				verboseBinary += "11";
			}
			else
			{
				morseMessage += " ";
				verboseBinary += "00";
			}
			bitsRead += 2;
		}

		//Outputs binary representation that was in the file
		if(bitsRead == numberOfBits)
		{
			break;
		}
	}

	if(verbose)
	{
		cout << endl << verboseBinary << endl;
	}

	delete[] bytesToRead;
	bytesToRead = NULL;

	return morseMessage;
}


MEMsg::MEMsg()
{
}

MEMsg::~MEMsg()
{
}
