#include "MEMsg.h"
#include <sstream> //Allows us to add multiple strings into one string
#include <cctype>  //For uppercase transformation

map<char, string> MEMsg::englishTranslation;
map<string, char> MEMsg::morseTranslation;

void MEMsg::initTranslation()
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

string MEMsg::toMorse( const string& English)
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

string MEMsg::toEnglish( const string& Morse)
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

MEMsg::MEMsg()
{
}

MEMsg::~MEMsg()
{
}
