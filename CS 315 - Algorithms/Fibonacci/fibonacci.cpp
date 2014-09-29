// Ashkan Hosseini
// CS315
// 09/04/2012

/*
 * The nth number (N_FIB) in the modified Fibonacci sequence will be output.
 *
 * This program will compute a modified version of the Fibonacci 
 * sequence fib=(fib-1)+3(fib-2) where fib-1 is the last number 
 * in the sequence relative to fib (and similarly fib-2 is two before fib).
 * This number will be output to the console/terminal in decimal representation
 */

#include <iostream>
#include <vector>

// ==== SET THIS VALUE TO OUTPUT THE NTH VALUE IN THE SEQUANCE =====
#define N_FIB 273

using namespace std;

void fib2(int n); //part of the modified Fibonacci algorithm lies here
void computeFib(vector<char> &fib, vector<char> &fibMinus1, vector<char> &fibMinus2); //computes fib=(fib-1)+3(fib-2)
void binaryToDecimal(vector<int> &decimal, vector<char> &binary); //converts binary representation to decimal representation
void addDecimal(vector<int> &operand1, vector<int> &operand2); //adds to numbers in decimal representation

int main()
{
	//Compute and then output the nth number (N_FIB) in our sequence
	fib2(N_FIB);
	
	//Keep command line open
	int temp;
	cin >> temp;

	return 0;
}

void fib2(int n)
{
	if(n == 0 || n == 1)
	{
		//0 and 1 in the sequence is itself, so output n
		cout << n;
		return;
	}
	else if(n > 1)
	{
		//We need to keep track of the previous two numbers in the sequence
		// so create three numbers, one for the current one being computed
		// and two for the previous two in the sequence.
		//These numbers are stored in vectors of char variables that are used
		// to represent the number in binary.
		vector<char> numbers[3];
		numbers[0].push_back(1);
		numbers[1].push_back(1);
		numbers[2].push_back(0);

		//Indexes to the different numbers we need to store.
		// fib is the number in the sequence currently being computed
		// fibMinus1 (or fib-1) is the last computed number in the sequence
		// fibMinus2 (or fib-2) is the number in the sequence before fibMinus1
		int fib = 2;
		int fibMinus1 = 1;
		int fibMinus2 = 0;

		//Since we start with computing the third number in the sequence, only loop n-2 times.
		while(n > 2)
		{
			//Compute fib=(fib-1)+3(fib-2)
			computeFib(numbers[fib], numbers[fibMinus1], numbers[fibMinus2]);
			
			//Set fib to fib-1, fib-1 to fib-2, and fib-2 to fib
			int temp = fibMinus2;
			fibMinus2 = fibMinus1;
			fibMinus1 = fib;
			fib = temp;
			
			n--;
		}

		//Convert our binary representation to decimal
		vector<int> decimal;
		binaryToDecimal(decimal, numbers[fibMinus1]);

		//Output our result: the number in the sequence as requested by the user
		for(int i = decimal.size()-1; i >= 0; i--)
		{
			cout << decimal.at(i);
		}
	}
	else
	{
		//Error if n is invalid
		cout << "Error\n";
		return;
	}
}

//Compute fib = fibMinus1 + 3*fibMinus2
void computeFib(vector<char> &fib, vector<char> &fibMinus1, vector<char> &fibMinus2)
{
	//Assign or "add" fibMinus1 to fib
	for(int i = 0; i < fibMinus1.size(); i++)
	{
		fib.at(i) = fibMinus1.at(i);
	}

	//Now add fibMinus2 to fib three times (3*fibMinus2)
	for(int ct = 0; ct < 3; ct++)
	{
		//The following binary addition algorithm works as follows
		// Look at the current index in the sequence of digits
		// Add these digits and the carry (initially set to 0)
		//  If the result is 0, then set the resulting digit at this index to 0
		//  If the result is 1, then set the result digit at this index to 1
		//  If the result is 2, set the result digit at this index to 0 and carry to 1
		//  If the result is 3, set the result digit at this index to 1 and carry to 1
		int carry = 0;
		for(int i = 0; i < fib.size(); i++)
		{
			int amount = (int)fib.at(i) + (int)fibMinus2.at(i) + carry;
			carry = 0;

			if(amount == 1)
			{
				fib.at(i) = 1;
			}
			else if(amount == 2)
			{
				fib.at(i) = 0;
				carry = 1;
			}
			else if(amount == 3)
			{
				fib.at(i) = 1;
				carry = 1;
			}
		}

		//If at the end of addition we have carry, append a digit to our numbers
		// Set this digit for the current sequence number (fib) to 1 and the other two to 0
		if(carry == 1)
		{
			fib.push_back(1);
			fibMinus1.push_back(0);
			fibMinus2.push_back(0);
		}
	}
}

//Convert binary representation to decimal
void binaryToDecimal(vector<int> &decimal, vector<char> &binary)
{
	//Make sure the decimal vector has only one digit and is set to 0
	decimal.clear();
	decimal.push_back(0);

	//Create a decimal vector that holds the decimal representation for each binary place/digit
	vector<int> powersOfTwo;
	powersOfTwo.push_back(1);

	//Step through each binary digit and add the decimal representation if set to 1
	for(int i = 0; i < binary.size(); i++)
	{
		if(binary.at(i) == 1)
		{
			//Add 2^i to the decimal number since that bit is set
			addDecimal(decimal, powersOfTwo);
		}

		//Compute 2^(i+1)
		addDecimal(powersOfTwo, powersOfTwo);
	}
}

//Adds two decimal numbers and store the result in operand1
void addDecimal(vector<int> &operand1, vector<int> &operand2)
{
	//Make sure each operand is the same size
	// if not add zeroes to the front of the smaller number
	if(operand1.size() < operand2.size())
	{
		int sizeDiff = operand2.size() - operand1.size();
		while(sizeDiff != 0)
		{
			operand1.push_back(0);
			sizeDiff--;
		}
	}
	else if(operand2.size() < operand1.size())
	{
		int sizeDiff = operand1.size() - operand2.size();
		while(sizeDiff != 0)
		{
			operand2.push_back(0);
			sizeDiff--;
		}
	}

	//Below is the decimal addition algorithm, it works as follows:
	// Add the digits of the two numbers and the carry at the current index
	//  If the result is greater than 9 then subtract 10 and add 1 to carry
	// Add the result to the resulting digit (the current digit for operand1)
	// If there is a carry after the last digits are added then append operand1 with 1
	int carry = 0;
	for(int i = 0; i < operand1.size(); i++)
	{
		int temp = operand1.at(i) + operand2.at(i) + carry;
		carry = 0;

		if(temp > 9) 
		{
			temp = temp - 10;
			carry = 1;
		}

		operand1.at(i) = temp;
	}

	if(carry == 1)
	{
		operand1.push_back(1);
	}
}