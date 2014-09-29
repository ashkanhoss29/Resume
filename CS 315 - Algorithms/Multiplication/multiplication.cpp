// Ashkan Hosseini
// CS315
// 09/12/2012

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

/* An implementation of the iterative multiplication algorithm */

/* Compilation and Running instructions *

   In order to compute the product of two n-bit numbers, set N (line 18) to that size
   After setting N to the desired value, compile this one source file with a standard c++ compiler
   Run the executable and the counter and product will be output to the console/terminal

*/

#define N 60

using namespace std;

void createRandomBinary(vector<char> &x, int n);
void multiply(vector<char> &x, vector<char> &y, vector<char> &z);
void shift(vector<char> &x);
void add(vector<char> &x, vector<char> &y);
void equalizeLengths(vector<char> &operand1, vector<char> &operand2);
void binaryToDecimal(vector<char> &decimal, vector<char> &binary);
void addDecimal(vector<char> &x, vector<char> &y);

int counter = 0; //stores the number of elementary operations performed in computing the product

int main()
{
	vector<char> x; //operand 1
	vector<char> y; //operand 2
	vector<char> z; //result
	vector<char> decimal; //result in decimal format

	//Create two random numbers
	srand(time(NULL));
	createRandomBinary(x, N);
	createRandomBinary(y, N);

	//Compute the product
	multiply(x, y, z);
	
	//Output the result in decimal for easy read
	cout << "operations count: " << counter << endl;
	binaryToDecimal(decimal, z);
	for(int i = decimal.size()-1; i >= 0; i--)
	{
		cout << (int)decimal.at(i);
	}

	//Keeps console open
	int stop;
	cin >> stop;
}


void createRandomBinary(vector<char> &x, int n)
{
	//Create random n-bit number
	for(int i = 0; i < n; i++)
	{
		x.push_back(rand() % 2); //push random digit (0 or 1)
	}
}

void multiply(vector<char> &x, vector<char> &y, vector<char> &z)
{
	//Make sure z is 0
	z.clear();      counter++;
	z.push_back(0); counter++;

	//Set the lengths of each number to the longest number
	equalizeLengths(x, y);
	equalizeLengths(x, z);

	//Below is the main multiplication algorithm (see Homework2.pdf for more info)
	counter++; //x.size()-1
	counter++; //i assignment
	for(int i = x.size()-1; i >= 0; i--)
	{
		counter++; //i >= 0
		counter++; //i--

		//Multiply z by 2 (shift because we are base 2)
		shift(z);

		//If y at i is 1 we do 2z+x, otherwise stick with 2z
		if(y.at(i) == 1)
		{
			counter++; //y.at(i) == 1

			add(z, x);
		}
	}
}

void shift(vector<char> &x)
{
	int hold = 0; counter++;
	int temp = 0; counter++;

	counter++; //i assignment
	for(int i = 0; i < x.size(); i++)
	{
		counter++; //i < x.size()
		counter++; //i++

		//Assign previous digit (hold) to current digit and set current digit to hold
		temp = x.at(i); counter++;
		x.at(i) = hold; counter++;
		hold = temp;    counter++;
	}

	//Append the value of the previous digit if that digit was greater than 0
	if(hold > 0)
	{
		counter++; //hold > 0

		x.push_back(hold); counter++;
	}
}

//Adds y to x
void add(vector<char> &x, vector<char> &y)
{
	//The following binary addition algorithm works as follows
	// Look at the current index in the sequence of digits
	// Add these digits and the carry (initially set to 0)
	//  If the result is 0, then set the resulting digit at this index to 0
	//  If the result is 1, then set the result digit at this index to 1
	//  If the result is 2, set the result digit at this index to 0 and carry to 1
	//  If the result is 3, set the result digit at this index to 1 and carry to 1
	
	equalizeLengths(x, y);

	int carry = 0; counter++;

	counter++; //i assignment
	for(int i = 0; i < x.size(); i++)
	{
		counter++; //i < x.siz()
		counter++; //i++

		int amount = (int)x.at(i) + (int)y.at(i) + carry; counter++;
		carry = 0; counter++;

		if(amount == 1)
		{
			counter++; //amount == 1
			x.at(i) = 1; counter++;
		}
		else if(amount == 2)
		{
			counter++;
			x.at(i) = 0; counter++;
			carry = 1;   counter++;
		}
		else if(amount == 3)
		{
			counter++;
			x.at(i) = 1; counter++;
			carry = 1;   counter++;
		}
	}

	//If at the end of addition we have carry, append a digit to our numbers
	// Set this digit for the current sequence number (operand1) to 1 and the other to 0
	if(carry == 1)
	{
		counter++;
		x.push_back(1); counter++;
		y.push_back(0); counter++;
	}
}

void equalizeLengths(vector<char> &operand1, vector<char> &operand2)
{
	//Make sure the lengths of both operands are the same
	//If one is smaller, add zeroes to its end
	if(operand1.size() < operand2.size())
	{
		counter++; //size comparison

		int sizeDiff = operand2.size() - operand1.size(); counter++;
		while(sizeDiff != 0)
		{
			counter++; //sizeDiff != 0

			operand1.push_back(0); counter++;
			sizeDiff--;            counter++;
		}
	}
	else if(operand2.size() < operand1.size())
	{
		counter++;

		int sizeDiff = operand1.size() - operand2.size(); counter++;
		while(sizeDiff != 0)
		{
			counter++;

			operand2.push_back(0); counter++;
			sizeDiff--;            counter++;
		}
	}
}

//Convert binary representation to decimal
void binaryToDecimal(vector<char> &decimal, vector<char> &binary)
{
	//Make sure the decimal vector has only one digit and is set to 0
	decimal.clear();
	decimal.push_back(0);

	//Create a decimal vector that holds the decimal representation for each binary place/digit
	vector<char> powersOfTwo;
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
void addDecimal(vector<char> &x, vector<char> &y)
{
	equalizeLengths(x, y);

	//Below is the decimal addition algorithm, it works as follows:
	// Add the digits of the two numbers and the carry at the current index
	//  If the result is greater than 9 then subtract 10 and add 1 to carry
	// Add the result to the resulting digit (the current digit for operand1)
	// If there is a carry after the last digits are added then append operand1 with 1
	int carry = 0;
	for(int i = 0; i < x.size(); i++)
	{
		int temp = x.at(i) + y.at(i) + carry;
		carry = 0;

		if(temp > 9) 
		{
			temp = temp - 10;
			carry = 1;
		}

		x.at(i) = temp;
	}

	if(carry == 1)
	{
		x.push_back(1);
	}
}