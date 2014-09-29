/*
 * Bonus 2
 *
 * Ashkan Hosseini
 * CS 375
 * 04/30/2014
 *
 */

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Index for a square ( 1 <= i,j <= n )
struct square {

	square(int i, int j)
	{
		this->i = i;
		this->j = j;
	}

	int i;
	int j;
};

// Prototypes
void N_Queens(int n, vector<square> &FS);

/*
 * This is a driver for Bonus 2
*/
int main()
{
	// Test N_Queens
	vector<square> FS;
	FS.push_back(square(1,2));
	FS.push_back(square(2,4));

	N_Queens(4, FS);


	// Keep console open
	char hold;
	cin >> hold;

	// End
	return 0;
}

/*
 * Produce a CNF formula representing the n-queens problem
 *  with additional requirement that no queen is placed in any
 *  of the entries in FS
 */
void N_Queens(int n, vector<square> &FS)
{
	int clauseCount = 0;
	stringstream out; // hold result for output until after we know clause count

	cout << "p cnf " << n*n;

	// At least 1 queen per row
	for(int i = 0; i < n; i++) // For each row
	{
		for(int j = 1; j <= n; j++) // For each column in row
		{
				out << i*n+j << " ";
		}

		if(i != n)
			out << " 0";

		out << endl;
		clauseCount++;
	}

	// At least 1 queen per column
	for(int j = 1; j <= n; j++) // For each row
	{
		for(int i = 0; i < n; i++) // For each row in column
		{
				out << i*n+j << " ";
		}

		if(j != n-1)
			out << " 0";

		out << endl;
		clauseCount++;
	}

	// At most 1 queen per row
	for(int i = 0; i < n; i++) // For each row
	{
		for(int j = 1; j <= n; j++) // For each column in row
		{
			// Check current i,j with i,j+k
			for(int k = j+1; k <= n; k++)
			{
				out << "-" << i*n+j << " -" << i*n+k << " 0" << endl;
				clauseCount++;
			}
		}
	}

	// At most 1 queen per row
	for(int j = 1; j <= n; j++) // For each row
	{
		for(int i = 0; i < n; i++) // For each column in row
		{
			// Check current j,i with j+k,i
			for(int k = i+1; k < n; k++)
			{
				out << "-" << i*n+j << " -" << k*n+j << " 0" << endl;
				clauseCount++;
			}
		}
	}

	// At most 1 queen on diagonal (check right diagonal)
	for(int i = 0; i < n; i++) // For each row
	{
		for(int j = 1; j <= n; j++) // For each column in row
		{
			for(int k = 1; i+k < n && j+k <= n; k++)
			{
				out << "-" << i*n+j << " -" << (i+k)*n+(j+k) << " 0" << endl;
				clauseCount++;
			}
		}
	}

	// At most 1 queen on diagonal (check left diagonal)
	for(int i = 0; i < n; i++) // For each row
	{
		for(int j = 1; j <= n; j++) // For each column in row
		{
			for(int k = 1; i+k < n && j-k > 0; k++)
			{
				out << "-" << i*n+j << " -" << (i+k)*n+(j-k) << " 0" << endl;
				clauseCount++;
			}
		}
	}

	// No queen placed on FS entries
	for(int i = 0; i < FS.size(); i++)
	{
		out << " -" << (FS.at(i).i-1)*10 + FS.at(i).j << " ";
	}

	if(FS.size() > 0)
		out << " 0";

	// Write to output
	cout << " " << clauseCount << endl;
	cout << out.str();
}