//Ashkan Hosseini
//CS315
//9/21/2012

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

void createRandomBinary(vector<char> &x, int n);
void divide(vector<char> &x, vector<char> &y, vector<char> &q, vector<char> &r);
int equalityCheck(vector<char> &x, vector<char> &y);
void shift(vector<char> &x);
void subtract(vector<char> &x, vector<char> &y);
void add(vector<char> &x, vector<char> &y, bool lastCarry);
void equalizeLengths(vector<char> &operand1, vector<char> &operand2);
void binaryToDecimal(vector<char> &decimal, vector<char> &binary);
void addDecimal(vector<char> &x, vector<char> &y);

int main()
{
	vector<char> x, y, q, r, decimal;

	srand(time(NULL));

	//Tests:

	//Create random numbers and perform subtraction
	cout << "Subtraction Test" << endl;
	for(int i = 4; i < 10; i++)
	{
		createRandomBinary(x, i);
		createRandomBinary(y, i);

		cout << "x: ";
		binaryToDecimal(decimal, x);
		cout << "y: ";
		binaryToDecimal(decimal, y);

		//Subtract from the bigger number
		if(equalityCheck(x, y) == 1)
		{
			cout << "x-y: ";
			subtract(x, y);
			binaryToDecimal(decimal, x);
		}
		else
		{
			cout << "y-x: ";
			subtract(y, x);
			binaryToDecimal(decimal, y);
		}

		cout << endl;
	}

	//Create random numbers and perform division
	// ***** WARNING: Test WILL NOT check division by 0!!!!!! ******
	// ***** Division by 0 WILL return incorrect result *****
	cout << "Division Test" << endl;
	for(int i = 4; i < 10; i++)
	{
		createRandomBinary(x, i);
		createRandomBinary(y, i);

		cout << "x: ";
		binaryToDecimal(decimal, x);
		cout << "y: ";
		binaryToDecimal(decimal, y);

		//Divide x by y
		divide(x, y, q, r);
		cout << "x/y quotient: ";
		binaryToDecimal(decimal, q);
		cout << "x/y remainder: ";
		binaryToDecimal(decimal, r);

		cout << endl;
	}

	char wait;
	cin >> wait;
}

void createRandomBinary(vector<char> &x, int n)
{
	x.clear();

	//Create random n-bit number
	for(int i = 0; i < n; i++)
	{
		x.push_back(rand() % 2); //push random digit (0 or 1)
	}
}

//x/y
void divide(vector<char> &x, vector<char> &y, vector<char> &q, vector<char> &r)
{
	equalizeLengths(x, y);

	//Set q = r = 0
	q.clear();
	r.clear();
	q.push_back(0);
	r.push_back(0);

	//Create vector to hold value of 1
	vector<char> one;
	one.push_back(1);

	//Main divide algorithm
	for(int i = x.size()-1; i >= 0; i--)
	{
		shift(q); // q = 2q
		shift(r); // r = 2r

		if(x.at(i) == 1) add(r, one, true); // r = r+1

		if(equalityCheck(r, y) != 2)
		{
			subtract(r, y); // r = r-y
			add(q, one, true);  // q = q+1
		}
	}
}

//0 if equal
//1 if x > y
//2 if x < y
int equalityCheck(vector<char> &x, vector<char> &y)
{
	equalizeLengths(x, y);

	//Go through bits from most significant to least
	//Compare bits at i, if one bit is greater that number is greater
	for(int i = x.size()-1; i >= 0; i--)
	{
		if(x.at(i) > y.at(i)) return 1;
		if(x.at(i) < y.at(i)) return 2;
	}

	return 0;
}

void shift(vector<char> &x)
{
	int hold = 0;
	int temp = 0;

	for(int i = 0; i < x.size(); i++)
	{
		//Assign previous digit (hold) to current digit and set current digit to hold
		temp = x.at(i);
		x.at(i) = hold;
		hold = temp;
	}

	//Append the value of the previous digit if that digit was greater than 0
	if(hold > 0)
	{
		x.push_back(hold);
	}
}

//x-y
//Assumes x >= y > 0
void subtract(vector<char> &x, vector<char> &y)
{
	equalizeLengths(x, y);

	//A-B = A+inverse(B)+1 and drop most significant digit
	//The source of this is from CS380 which I am taking alongside CS315

	//Inverse of y
	vector<char> iy;
	for(int i = 0; i < y.size(); i++)
	{
		if(y.at(i) == 0) iy.push_back(1);
		else iy.push_back(0);
	}

	//iy = iy+1
	vector<char> one;
	one.push_back(1);
	add(iy, one, true);

	//x = x+iy
	add(x, iy, false);
}

//Adds y to x
void add(vector<char> &x, vector<char> &y, bool lastCarry)
{
	//The following binary addition algorithm works as follows
	// Look at the current index in the sequence of digits
	// Add these digits and the carry (initially set to 0)
	//  If the result is 0, then set the resulting digit at this index to 0
	//  If the result is 1, then set the result digit at this index to 1
	//  If the result is 2, set the result digit at this index to 0 and carry to 1
	//  If the result is 3, set the result digit at this index to 1 and carry to 1
	
	equalizeLengths(x, y);

	int carry = 0;

	for(int i = 0; i < x.size(); i++)
	{
		int amount = (int)x.at(i) + (int)y.at(i) + carry;
		carry = 0;

		if(amount == 1)
		{
			x.at(i) = 1;
		}
		else if(amount == 2)
		{
			x.at(i) = 0;
			carry = 1;
		}
		else if(amount == 3)
		{
			x.at(i) = 1;
			carry = 1;
		}
	}

	//If at the end of addition we have carry, append a digit to our numbers
	// Set this digit for the current sequence number (operand1) to 1 and the other to 0
	if(carry == 1 && lastCarry)
	{
		x.push_back(1);
		y.push_back(0);
	}
}

void equalizeLengths(vector<char> &operand1, vector<char> &operand2)
{
	//Make sure the lengths of both operands are the same
	//If one is smaller, add zeroes to its end
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

	for(int i = decimal.size()-1; i >= 0; i--)
	{
		cout << (int)decimal.at(i);
	}
	cout << endl;
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