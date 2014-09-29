// Ashkan Hosseini
// CS335
// 09/06/2012

package cs335_chart;

import java.util.Random;
import java.util.Scanner;

public class Chart
{
    public static void main(String[] args)
	{
		//Get user input
		System.out.print("Enter chart size between 1 and 10: ");
		Scanner input = new Scanner( System.in );
		int chartSize = input.nextInt();
		
		//Check user input
		if(chartSize < 1 || chartSize > 10)
		{
			System.out.println("Error: You did not enter a number between 1 and 10");
			return;
		}
		
		//Create random number generator
		Random randomNumbers = new Random();
		
		//Display top row
		for (int j = 0; j < chartSize; j++)
		{
			System.out.print("+-----");
		}
		System.out.println("+");
		
		//Rows
		for(int i = 0; i < chartSize; i++)
		{
			//Columns
			System.out.print("|");
			for(int j = 0; j < chartSize; j++)
			{
				if(i == j)
				{
					System.out.print("CS335|");
				}
				else
				{
					//Get random number of digits and create a string to store them
					int numberOfDigits = 1 + randomNumbers.nextInt(5);
					String number = "";
					
					//For each digit get random number between 1 and 9
					for(int k = 0; k < numberOfDigits; k++)
					{
						int digit = 1 + randomNumbers.nextInt(9);
						number += digit;
					}
					
					//Determine how to output number based on number of digits
					if(numberOfDigits == 1)
					{
						System.out.print(" (" + number + ") |");
					}
					else if(numberOfDigits == 2)
					{
						System.out.print(" " + number + "* |");
					}
					else if(numberOfDigits == 3)
					{
						System.out.print(" " + number + " |");
					}
					else if(numberOfDigits == 4)
					{
						System.out.print("$" + number + "|");
					}
					else if(numberOfDigits == 5)
					{
						System.out.print(number + "|");
					}
				}
			}
			
			//Bottom of each row
			System.out.println("");
			for(int j = 0; j < chartSize; j++)
			{
				System.out.print("+-----");
			}
			System.out.println("+");
		}
    }
}


/* Example output:
+-----+-----+-----+-----+-----+
|CS335|$3954| 858 | 93* |55647|
+-----+-----+-----+-----+-----+
| 619 |CS335|$1156|81789| 658 |
+-----+-----+-----+-----+-----+
| 16* | 777 |CS335|$4766|$1625|
+-----+-----+-----+-----+-----+
|97773|74251| 631 |CS335|44283|
+-----+-----+-----+-----+-----+
|$7162| 287 | (5) | (5) |CS335|
+-----+-----+-----+-----+-----+
 */