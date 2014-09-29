// Ashkan Hosseini
// CS315
// 10/08/2012

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

/* Compilation and Running instructions *

   In order to compute a prime number of n-bits, set NUMBER_OF_BITS (line 19) to that size
   After setting NUMBER_OF_BITS to the desired value, compile this one source file with a standard c++ compiler
   Run the executable and the prime number and counter & time will be output to the console/terminal

*/
using namespace std;

#define NUMBER_OF_BITS 64

void createRandomBinary(vector<int> &x, int n);
bool primality(vector<int> &N);
void modexp(vector<int> &x, vector<int> &y, vector<int> &N, vector<int> &z);
void divide(vector<int> &x, vector<int> &y, vector<int> &q, vector<int> &r);
void multiply(vector<int> &x, vector<int> &y);
void subtract(vector<int> &x, vector<int> &y);
void add(vector<int> &x, vector<int> &y, bool lastCarry);
void equalizeLengths(vector<int> &operand1, vector<int> &operand2);
int equalityCheck(vector<int> &x, vector<int> &y);
void binaryToDecimal(vector<int> &binary);
void addDecimal(vector<int> &x, vector<int> &y);

// **** UPDATE: packed 30 bits in single vector entry ****
// One major addition made to this program is chunking 30 bits into a single vector entry
// By doing this, the execution time increased 10x
//  This has required making changes to how bits are accessed
//  Bits are no longer accessed by only traversing the vector, but requires using masks
//  At each index of the vector, we use masks to access individual bits at that index
//  ***This allows use of binary arithmetic as required by the assignment***

int main()
{
	//Stores our randomly generated number
	vector<int> number;

	srand(time(NULL));

	//Counters for number of iteration and CPU time
	int counter = 0;
	int time1, time2;

	//Generated random numbers and test for primality
	//Keep track of time and how many numbers tested
	time1 = clock();
	while(true)
	{
		counter++;

		//Create random number, then make sure it is odd number
		createRandomBinary(number, NUMBER_OF_BITS);
		if((number.at(0) & 0x00000001) != 0x00000001)
		{
			number.at(0) = number.at(0) | 0x00000001;
		}

		if(primality(number))
		{
			//If the number is prime, output in decimel and break loop
			binaryToDecimal(number);
			break;
		}

		number.clear();
	}
	time2 = clock();

	cout << "Counter: " << counter << endl;
	cout << "Time: " << time2-time1 << endl;

	//Keeps console open
	int stop;
	cin >> stop;
}

//Goes through each bit and randomly generates 1 or 0 for that bit
void createRandomBinary(vector<int> &x, int n)
{
	x.clear();
	
	int i = 0;
	int bitsAdded = 0;
	while(bitsAdded < n-1)
	{
		x.push_back(0);
		int bitPosition = 1;
		for(int j = 0; j < 30; j++)
		{
			if(bitsAdded < n-1)
			{
				int random = rand() % 2;
				if(random == 1)
				{
					x.at(i) = x.at(i) | bitPosition;
				}
			}
			else
			{
				x.at(i) = x.at(i) | bitPosition;
				bitsAdded++;
				break;
			}

			bitPosition = bitPosition << 1;
			bitsAdded++;
		}

		i++;
	}
}

bool primality(vector<int> &N)
{
	vector<int> a, one, nMinus1, z;

	//Let a = 3
	a.push_back(1);
	a.push_back(1);

	//Get N-1
	one.push_back(1);
	equalizeLengths(N, one);
	nMinus1 = N;
	subtract(nMinus1, one);

	//Compute a^(N-1) mod N
	modexp(a, nMinus1, N, z);

	//Check to see if a^(N-1) = 1 mod N
	if(z.at(0) != 1) return false;
	for(int i = 1; i < z.size(); i++)
	{
		if(z.at(i) != 0)
		{
			return false;
		}
	}

	return true;
}

// This is the modexp algorithm directly from class notes
void modexp(vector<int> &x, vector<int> &y, vector<int> &N, vector<int> &z)
{
	equalizeLengths(x, y);
	equalizeLengths(x, N);

	z.clear();
	z.push_back(0);
	equalizeLengths(x, z);

	vector<int> temp;
	vector<int> q;
	vector<int> r;

	bool yZero = true;
	for(int i = 0; i < y.size(); i++)
	{
		if(y.at(i) != 0)
		{
			yZero = false;
		}
	}
	if(yZero) return;

	z.at(0) = 1;

	for(int i = y.size()-1; i >= 0; i--)
	{
		int bitPosition = 0x20000000;
		for(int j = 29; j >= 0; j--)
		{
			multiply(z, z);
			divide(z, N, q, r);
			z = r;

			if((y.at(i) & bitPosition) == bitPosition)
			{
				multiply(z, x);
				divide(z, N, q, r);
				z = r;
			}

			bitPosition = bitPosition >> 1;
		}
	}
}

//x/y
void divide(vector<int> &x, vector<int> &y, vector<int> &q, vector<int> &r)
{
	equalizeLengths(x, y);

	//Set q = r = 0
	q.clear();
	r.clear();
	q.push_back(0);
	r.push_back(0);

	//Create vector to hold value of 1
	vector<int> one;
	one.push_back(1);

	//Main divide algorithm
	for(int i = x.size()-1; i >= 0; i--)
	{
		int bitPosition = 0x20000000;
		for(int j = 29; j >= 0; j--)
		{
			add(q, q, true);
			add(r, r, true);

			if((x.at(i) & bitPosition) == bitPosition)
			{
				add(r, one, true);
			}

			if(equalityCheck(r, y) != 2)
			{
				subtract(r, y); // r = r-y
				add(q, one, true);  // q = q+1
			}

			bitPosition = bitPosition >> 1;
		}
	}
}

// x = x*y
void multiply(vector<int> &x, vector<int> &y)
{
	//Make sure z is 0
	vector<int> z;
	z.push_back(0);

	//Set the lengths of each number to the longest number
	equalizeLengths(x, y);
	equalizeLengths(x, z);

	//Below is the main multiplication algorithm (see Homework2.pdf for more info)
	for(int i = x.size()-1; i >= 0; i--)
	{
		int bitPosition = 0x20000000;
		for(int j = 29; j >= 0; j--)
		{
			add(z, z, true);

			if((y.at(i) & bitPosition) == bitPosition)
			{
				add(z, x, true);
			}

			bitPosition = bitPosition >> 1;
		}
	}

	x = z;
}

void subtract(vector<int> &x, vector<int> &y)
{
	equalizeLengths(x, y);

	//A-B = A+inverse(B)+1 and drop most significant digit
	//The source of this is from CS380 which I am taking alongside CS315

	//Inverse of y
	vector<int> iy;
	for(int i = 0; i < y.size(); i++)
	{
		iy.push_back(0);

		int bitPosition = 1;
		for(int j = 0; j < 30; j++)
		{
			if((y.at(i) & bitPosition) != bitPosition)
			{
				iy.at(i) = iy.at(i) | bitPosition;
			}

			bitPosition = bitPosition << 1;
		}
	}

	//iy = iy+1
	vector<int> one;
	one.push_back(1);
	add(iy, one, true);

	//x = x+iy
	add(x, iy, false);
}

//Adds y to x
void add(vector<int> &x, vector<int> &y, bool lastCarry)
{
	//The following binary addition algorithm works as follows
	// Look at the current index in the sequence of digits
	// Add these digits and the carry (initially set to 0)
	//  If the result is 0, then set the resulting digit at this index to 0
	//  If the result is 1, then set the result digit at this index to 1
	//  If the result is 2, set the result digit at this index to 0 and carry to 1
	//  If the result is 3, set the result digit at this index to 1 and carry to 1

	//UPDATE: I know pack bits into sets of 30
	
	equalizeLengths(x, y);

	int carry = 0;
	for(int i = 0; i < x.size(); i++)
	{
		int temp = x.at(i) + y.at(i) + carry;
		carry = 0;

		if(temp > 0x3fffffff) 
		{
			//carry = (temp & 0xc0000000) >> 30;
			carry = 1;
			temp = temp & 0x3fffffff;
		}
		x.at(i) = temp;
	}

	if(carry == 1 && lastCarry)
	{
		x.push_back(carry);
		y.push_back(0);
	}
}

void equalizeLengths(vector<int> &operand1, vector<int> &operand2)
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

//0 if equal
//1 if x > y
//2 if x < y
int equalityCheck(vector<int> &x, vector<int> &y)
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

//Convert binary representation to decimal
void binaryToDecimal(vector<int> &binary)
{
	//Make sure the decimal vector has only one digit and is set to 0
	vector<int> decimal;
	decimal.push_back(0);

	//Create a decimal vector that holds the decimal representation for each binary place/digit
	vector<int> powersOfTwo;
	powersOfTwo.push_back(1);

	//Step through each binary digit and add the decimal representation if set to 1
	for(int i = 0; i < binary.size(); i++)
	{
		int bitPosition = 1;
		for(int j = 0; j < 30; j++)
		{
			if((binary.at(i) & bitPosition) == bitPosition)
			{
				addDecimal(decimal, powersOfTwo);
			}

			addDecimal(powersOfTwo, powersOfTwo);
			bitPosition = bitPosition << 1;
		}
	}

	for(int i = decimal.size()-1; i >= 0; i--)
	{
		cout << decimal.at(i);
	}
	cout << endl;
}

//Adds two decimal numbers and store the result in operand1
void addDecimal(vector<int> &x, vector<int> &y)
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