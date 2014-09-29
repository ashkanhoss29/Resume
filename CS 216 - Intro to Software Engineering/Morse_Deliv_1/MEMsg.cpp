#include "MEMsg.h"

map<char, string> MEMsg::morseTranslation;

void MEMsg::initMorseTranslation()
{
	morseTranslation['A'] = ".-";
	morseTranslation['B'] = "-...";
	morseTranslation['C'] = "-.-.";
	morseTranslation['D'] = "-..";
	morseTranslation['E'] = ".";
	morseTranslation['F'] = "..-.";
	morseTranslation['G'] = "--.";
	morseTranslation['H'] = "....";
	morseTranslation['I'] = "..";
	morseTranslation['J'] = ".---";
	morseTranslation['K'] = "-.-";
	morseTranslation['L'] = ".-..";
	morseTranslation['M'] = "--";
	morseTranslation['N'] = "-.";
	morseTranslation['O'] = "---";
	morseTranslation['P'] = ".--.";
	morseTranslation['Q'] = "--.-";
	morseTranslation['R'] = ".-.";
	morseTranslation['S'] = "...";
	morseTranslation['T'] = "-";
	morseTranslation['U'] = "..-";
	morseTranslation['V'] = "...-";
	morseTranslation['W'] = ".--";
	morseTranslation['X'] = "-..-";
	morseTranslation['Y'] = "-.--";
	morseTranslation['Z'] = "--..";
	morseTranslation['0'] = "-----";
	morseTranslation['1'] = ".----";
	morseTranslation['2'] = "..---";
	morseTranslation['3'] = "...--";
	morseTranslation['4'] = "....-";
	morseTranslation['5'] = ".....";
	morseTranslation['6'] = "-....";
	morseTranslation['7'] = "--...";
	morseTranslation['8'] = "---..";
	morseTranslation['9'] = "----.";
	morseTranslation['.'] = ".-.-.-";
	morseTranslation[','] = "--..--";
	morseTranslation['?'] = "..--..";
	morseTranslation['\''] = ".----.";
	morseTranslation['!'] = "-.-.--";
	morseTranslation['/'] = "-..-.";
	morseTranslation['('] = "-.--.";
	morseTranslation[')'] = "-.--.-";
	morseTranslation['&'] = ".-...";
	morseTranslation[':'] = "---...";
	morseTranslation[';'] = "-.-.-.";
	morseTranslation['='] = "-...-";
	morseTranslation['+'] = ".-.-.";
	morseTranslation['-'] = "-....-";
	morseTranslation['_'] = "..--.-";
	morseTranslation['"'] = ".-..-.";
	morseTranslation['$'] = "...-..-";
	morseTranslation['@'] = ".--.-.";
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
	//stubb implementation
	return " ";
}

string MEMsg::toEnglish( const string& Morse)
{
	//stub implementation
	return " ";
}

MEMsg::MEMsg()
{
}

MEMsg::~MEMsg()
{
}
