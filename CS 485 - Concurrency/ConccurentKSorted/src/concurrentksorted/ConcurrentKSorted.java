/* Taylor Spencer
 * Ashkan Hosseini
 * 02/14/2013
 */

/*
* Sample solution for HW3, Spr 2013
* Command-line arguments:  k(=blocksize) [infile]
* Infile format:
*   N
*   <list of N integers>
* Output:  # of sorted and unsorted blocks.
* This version checks blocks sequentially
*/
package concurrentksorted;

import java.io.*;
import java.util.Scanner;

public class ConcurrentKSorted
{
	private static int sortedCount = 0;
	private static int unsortedCount = 0;
	private static int blockCount = 0;
	// XXX this is not needed for sequential version...
	private static final Object foo = new Object(); // for locking purposes only

	public static void main(String[] args) throws Exception
	{
		if(args.length < 1)
		{
			System.out.println("Usage: java AlmostKSorted_k <K>  [<inputfilename>]");
			System.exit(3);
		}
		
		int blockSize = Integer.parseInt(args[0]);
		String fileName = ((args.length > 1) ? args[1] : "AlmostKSorted_k.in");
		
		Scanner in = new Scanner(new File(fileName)); // may throw
		
		int n = in.nextInt();    // read the first integer to get array size (n)
		int input[] = new int[n]; // allocate an array to hold them
		
		// now read in n integers
		for(int i = 0; i < n; i++)
		{
			input[i] = in.nextInt();  // may throw!
		}
		
		int start = 0;
		int end = blockSize;
		
		//Step through each block and check if its sorted or unsorted
		while(end <= n)
		{
			//Each call to checkBlock is put in a seperate thread
			checkBlock(start, end, input);

			start = end;
			end += blockSize;
			
			blockCount++;
		}
		
		//Check if there is a trailing block
		if(start != n)
		{
			checkBlock(start, n, input);
			
			blockCount++;
		}
		
		//Check to see that all threads/blocks have finished
		boolean finished = false;
		while(!finished)
		{
			//Make sure access to sortedCount and unsortedCount is atomic
			synchronized(foo)
			{
				finished = blockCount == sortedCount+unsortedCount;
			}
		}

		System.out.println("Sorted: " + sortedCount);
		System.out.println("UnSorted: " + unsortedCount);
	}

	private static void checkBlock(final int start, final int end, final int data[])
	{
		new Thread()
		{
			public void run()
			{
				boolean sorted = true;
				for(int i = start; i + 1 < end; i += 1)
				{
					if(data[i] > data[i + 1])
					{
						sorted = false;
						break;
					}
				}
				
				//Make sure the counters are atomic (can only be incremented by one thread at a time)
				synchronized(foo)
				{
					if(sorted)
					{
						sortedCount++;
					}
					else
					{
						unsortedCount++;
					}
				}
			}
		}.start();
	}
}
