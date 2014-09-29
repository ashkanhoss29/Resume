import java.util.Random;

/*
 * Solve the N_Queen problem using brute force and heuristic methods
 */

public class N_Queens
{
	private static final int size = 9;
	private static final int sizeSquared = size*size;
	private static int board[][] = new int[size][size];
	private static int elimValues[][] = new int[size][size];
	
	public static void main(String[] args)
	{
		long timeStart = 0;
		long timeElapsed = 0;
		
		//Exhaustive
		timeStart = System.currentTimeMillis();
		exhaustive(0);
		timeElapsed = System.currentTimeMillis()-timeStart;
		System.out.println("Exhaustive time: " + timeElapsed + "ms");
		drawBoard();
		
    /*
		//Random
		clearBoard();
		timeStart = System.currentTimeMillis();
		random();
		timeElapsed = System.currentTimeMillis()-timeStart;
		System.out.println("\nRandom time: " + timeElapsed + "ms");
		drawBoard();
    */
		
		//Heuristic
		clearBoard();
		timeStart = System.currentTimeMillis();
		createElimValues();
		heuristic(0);
		timeElapsed = System.currentTimeMillis()-timeStart;
		System.out.println("\nHeuristic time: " + timeElapsed + "ms");
		drawBoard();
	}
	
	private static boolean exhaustive(int x)
	{
    //note: 1 means queen, 2 means marked square
    
		//Place queen in next row at first column
		// If space not avaliable, try next column
		//  If no spaces avaliable, go back one row and try the next column there
		
		//Try each column at row x
		for(int i = 0; i < size; i++)
		{
			//Place queen if there is no mark set here
			if(board[x][i] != 2)
			{
				markSquares(x, i); //place queen and set its marks
				
				//If we're at the end or next step is true, return true
				if( (x == size-1) || exhaustive(x+1) )
				{
					return true;
				}
				
				//If we could not place at the next rows then clear this queen
				board[x][i] = 0;
				remarkSquares();
			}
		}
		
		//If we reach here it means we couldn't place a queen in any column at row x
		return false;
	}
	
	private static void random()
	{
		int queenCounter = 0; //how many queens we've added
		
		Random randomGenerator = new Random();
		int randomX = 0; //random x coordinate to place queen
		int randomY = 0; //random y coordinate to place queen
		
		//Place all eight queens randomly until a solution is found
		do
		{
			clearBoard(); //clear board to redo random placement
			queenCounter = 0;
			
			//Randomly place eight queens
			while(queenCounter < size)
			{
				randomX = randomGenerator.nextInt(size); //random number 0-7
				randomY = randomGenerator.nextInt(size);
			
				//Make sure the queen we're setting isn't already set there
				if(board[randomX][randomY] != 1)
				{
					//Set queen
					board[randomX][randomY] = 1;
					queenCounter++;
				}
			}
		} while(!remarkSquares()); //remarkSquear will tell us if queens collide
	}
	
	private static boolean heuristic(int x)
	{
		//For the current row, order columns based on values
		// Try placing a queen from smallest to largest value
		//  If a queen is placed, call next row and attempt placement
		//  If no spaces left to place, return false so the previous row will iterate

		int valuesTried[] = new int[size]; //1 means tried, 0 means not tried
		int amountTried = 0;
		int smallestFound, smallestI = 0;

		while (amountTried < size)
		{
			smallestFound = sizeSquared;
			for (int i = 0; i < size; i++)
			{
				if (board[x][i] == 2)
				{
					valuesTried[i] = 1;
					amountTried++;
				}
				else if ((elimValues[x][i] < smallestFound) && (valuesTried[i] != 1))
				{
					smallestFound = elimValues[x][i];
					smallestI = i;
				}
			}

			if (smallestFound < sizeSquared)
			{
				valuesTried[smallestI] = 1;
				markSquares(x, smallestI);

				if ((x == size-1) || heuristic(x + 1))
				{
					return true;
				}

				board[x][smallestI] = 0;
				remarkSquares();
			}
		}
		
		//If we reach here it means we couldn't place a queen in any column at row x
		return false;
	}
	
	//Mark the corresponding squares of a queen placed at x,y
	private static boolean markSquares(int x, int y)
	{
		board[x][y] = 0; //removes queen if there and re-adds at the end
		
		//Mark horizontals and verticals:
		for(int i = 0; i < size; i++)
		{
			if((board[x][i] == 1) || (board[i][y] == 1)) return false;
			board[x][i] = 2;
			board[i][y] = 2;
		}
		
		//Mark diagonals
		for(int i = 1; i < size; i++)
		{
			if( ((x-i) > -1) && ((y-i) > -1) )
			{
				if(board[x-i][y-i] == 1) return false;
				board[x-i][y-i] = 2;
			}
			
			if( ((x-i) > -1) && ((y+i) < size) )
			{
				if(board[x-i][y+i] == 1) return false;
				board[x-i][y+i] = 2;
			}
			
			if( ((x+i) < size) && ((y-i) > -1) )
			{
				if(board[x+i][y-i] == 1) return false;
				board[x+i][y-i] = 2;
			}
			
			if( ((x+i) < size) && ((y+i) < size) )
			{
				if(board[x+i][y+i] == 1) return false;
				board[x+i][y+i] = 2;
			}
		}
		
		board[x][y] = 1;
		
		return true;
	}
	
	//Used if we remove a queen and need to remove it's marks as well (exhaustive)
	//Used to check if any queen is on top of a mark set by another queen (random)
	private static boolean remarkSquares()
	{
		//First demark all squares, keeping queens
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				if(board[i][j] == 2)
				{
					board[i][j] = 0;
				}
			}
		}
		
		//Mark squares for each queen on the board
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				if(board[i][j] == 1)
				{
					//Returning false means a queen is on top of a marked square
					if(!markSquares(i, j)) return false;
				}
			}
		}
		
		return true;
	}
	
	private static void clearBoard()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				board[i][j] = 0;
			}
		}
	}
	
	private static void createElimValues()
	{
		for(int i = 0; i < size; i++)
		{
			for(int j = 0; j < size; j++)
			{
				int tempI = i;
				int tempJ = j;
				int smallest = 0;
				
				if(i > 3) tempI = (size-1)-i;
				if(j > 3) tempJ = (size-1)-j;
				
				if(tempI <= tempJ)
					smallest = tempI;
				else 
					smallest = tempJ;
				
				elimValues[i][j] = smallest*2 + 22;
				
				//System.out.print(elimValues[i][j] + " ");
			}
			//System.out.println("");
		}
	}
	
	private static void drawBoard()
	{	
		//Rows
		for(int i = 0; i < size; i++)
		{
			System.out.print("|");
			
			//Columns
			for(int j = 0; j < size; j++)
			{
				if(board[i][j] == 0)
				{
					System.out.print(" |");
				}
				else if(board[i][j] == 1)
				{
					System.out.print("Q|");
				}
				else if(board[i][j] == 2)
				{
					System.out.print("X|");
				}
			}
			System.out.println("");
		}
	}
}