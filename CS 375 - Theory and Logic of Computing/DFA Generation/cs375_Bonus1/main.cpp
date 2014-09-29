/*
 * String DFA Generator
 *
 * Ashkan Hosseini
 * CS 375
 * 03/30/2014
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

// Helps contain information for a state
struct state {
	string current;
	int trans_G;
	int trans_A;
	int trans_T;
	int trans_C;
};

// Prototypes
string generateRandString(int length);
void constructDFA(vector<state> &states, string input);
char randomLetter();
int findTransition(string s1, string s2);
int match(vector<state> &states, string test);
void printDFA(vector<state> &states);

/*
 * The primary function for this program is to generate a DFA for a given search string.
 * The generated DFA is then used on a longer string to find substrings of the search string.
*/
int main()
{
	srand(time(NULL)); // for random string generation

	vector<state> states;
	string input = "";

	/*
	 * Below are multiple tests for testing DFA generation and string matching
	*/

	// Test 100-letter random string
	cout << "Test 100-letter:" << endl << endl;
	string Test1 = generateRandString(100);
	cout << Test1 << endl;
	for(int i = 3; i <= 10; i++)
	{
		string w = "";
		for(int j = 100-i; j < 100; j++)
		{
			w = w + Test1.at(j);
		}
		
		cout << "w" << i << " = " << w;

		constructDFA(states, w);
		cout << " : " << match(states, Test1) << " found" << endl;
	}


	// Test DFA generation
	cout << endl << endl << "Test GACGA DFA Generation:" << endl << endl;
	string DFA_testString1 = "GACGA";
	constructDFA(states, DFA_testString1);
	printDFA(states);

	cout << endl << endl << "Test AAGCATTTAAGCA DFA Generation:" << endl << endl;
	string DFA_testString2 = "AAGCATTTAAGCA";
	constructDFA(states, DFA_testString2);
	printDFA(states);


	// Asymptotic Analysis Comparison
	// Note that runtime is longer than result time. This is due to string generation time
	cout << endl << endl << "Asymptotic Analysis Comparison:" << endl << endl;
	cout << "100,000 Time Test" << endl;
	time_t addedTimes = 0;
	for(int i = 0; i < 10; i++)
	{
		string timeTest = generateRandString(100000);

		string w = "";
		for(int j = 100000-50; j < 100000; j++)
		{
			w = w + timeTest.at(j);
		}

		time_t timeDiff = time(NULL);
		constructDFA(states, w);
		match(states, timeTest);
		addedTimes = addedTimes + (time(NULL) - timeDiff);
	}
	cout << "Avarage time = " << addedTimes << endl;
	
	cout << endl << "200,000 Time Test" << endl;
	addedTimes = 0;
	for(int i = 0; i < 10; i++)
	{
		string timeTest = generateRandString(200000);

		string w = "";
		for(int j = 200000-50; j < 200000; j++)
		{
			w = w + timeTest.at(j);
		}

		time_t timeDiff = time(NULL);
		constructDFA(states, w);
		match(states, timeTest);
		addedTimes = addedTimes + (time(NULL) - timeDiff);
	}
	cout << "Avarage time = " << addedTimes << endl;


	// Keep console open!
	cin >> input;


	// End
	return 0;
}

string generateRandString(int length)
{
	string randomString = "";

	for(int i = 0; i < length; i++)
	{
		randomString = randomString + randomLetter();
	}

	return randomString;
}

void constructDFA(vector<state> &states, string input)
{
	states.clear();

	for(int i = 0; i < input.length(); i++)
	{
		state s;

		// add current state string
		if(i == 0)
			s.current = "";
		else
			s.current = states.at(i-1).current + input.at(i-1);
		
		// find transitions
		if(input.at(i) == 'G')
		{
			s.trans_G = i+1;
		}
		else
		{
			s.trans_G = findTransition(s.current, s.current+'G');
		}

		if(input.at(i) == 'A')
		{
			s.trans_A = i+1;
		}
		else
		{
			s.trans_A = findTransition(s.current, s.current+'A');
		}
		
		if(input.at(i) == 'T')
		{
			s.trans_T = i+1;
		}
		else
		{
			s.trans_T = findTransition(s.current, s.current+'T');
		}

		if(input.at(i) == 'C')
		{
			s.trans_C = i+1;
		}
		else
		{
			s.trans_C = findTransition(s.current, s.current+'C');
		}

		// add state to states
		states.push_back(s);
	}
}

char randomLetter()
{
	int randInt = rand() % 4 + 1;

	if(randInt == 1)
		return 'G';
	else if(randInt == 2)
		return 'A';
	else if(randInt == 3)
		return 'T';
	else
		return 'C';
}

int findTransition(string s1, string s2)
{
	for(int i = 0; i < s1.length(); i++)
	{
		if(s1.at((s1.length()-1) - i) == s2.at(s2.length()-1))
		{
			if((s1.length()-1)-i == 0)
				return 1;

			int counter = 0;
			while(s1.at((s1.length()-1)-(i+counter)) == s2.at((s2.length()-1)-counter) && (counter+i != s1.length()))
			{
				if(s1.length()-1 == i+counter)
					return counter+1;

				counter++;
			}
		}
	}

	return 0;
}

int match(vector<state> &states, string test)
{
	int state = 0;
	int foundCnt = 0;

	for(int i = 0; i < test.length(); i++)
	{
		if(test.at(i) == 'G')
			state = states.at(state).trans_G;
		else if(test.at(i) == 'A')
			state = states.at(state).trans_A;
		else if(test.at(i) == 'T')
			state = states.at(state).trans_T;
		else if(test.at(i) == 'C')
			state = states.at(state).trans_C;

		if(state == states.size())
		{
			foundCnt++;
			state = 0;
		}
	}

	return foundCnt;
}

void printDFA(vector<state> &states)
{
	cout << " G  A  T  C" << endl;
	cout << "___________" << endl;

	for(int i = 0; i < states.size(); i++)
	{
		cout << " " << states.at(i).trans_G << "  " 
			<< states.at(i).trans_A << "  " 
			<< states.at(i).trans_T << "  " 
			<< states.at(i).trans_C << "  " 
			<<  states.at(i).current << endl;
	}
}