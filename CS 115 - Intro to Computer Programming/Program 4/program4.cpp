// Name: Ashkan Hosseini
// Section: 002
// Date: 11/19/09
// Email: ashkan.hosseini@uky.edu

//***Program Purpose***:
/*  The purpose of this program is to simulate the game of Roulette.
	This program will first ask the user what number between 0 and 36 they want to be on.
	Then the user is asked how much of their stake they want to bet (this is the players "wager").  Players start with a stake of $100.
	Lastly the program will give the user the choice of betting on Even/Odd, Red/Black, and/or High/Low.  The user may choose not to bet on any of these three.
	The program will generate random numbers for where the ball landed and calculate winnings based on that and the users input
	This program will also display an animation simulating the role of the ball

	Winnings description:
		Number landed on (0-36):
			If the users chosen number is the same as the number the ball laned on, the user will win five times their wager.
			If the users input is not the same as the number the ball landed on, nothing happens
		Even/Odd/No. If the player chooses no, this step is skipped:
			If the player chooses even and the ball lands on even or the player chooses odd and the ball lands on odd, then the player wins two times their wager
			If neither of the above happens, than the player will lose the amount of their wager.
		Red/Black/No. If the player chooses no, this step is skipped:
			If the player chooses red and the ball lands on red or if the player chooses black and the ball lands on black, the user wins triple their wager
			If the above does not happen, the user will lose twice their wager
		High/Low/No. (this process is the same as Even/Odd)
			If the randomly generated number is more than 18 and the user guessed high, or if randomly generated number is less than or equal to 18 and user guessed low, than user wins their wager
			If the above is not true than the user loses their wager.
*/



//***Libraries***:
#include <iostream> //used for output and input from and to the console
#include <iomanip> //to be able to make values output 
#include <cstdlib> //used for the random number generator. Will use functions srand() and rand()
#include <ctime> //to be able to use function time() for the seed of function srand().
#include <cctype> //to be able to convert lower case characters to upper case by useing function toupper()
#include <string> //used for variables with type string
#include "graphics.h" //used to delay output of random number to console with function delay().  Also used to draw an animation.
using namespace std;



// ***Function Prototypes and Definitions***:

//Will generate a random number in the range specified. Parameter range is the range in which a random number will be generated:
int getrandnum();

//Asks the user what number between 0 and 36 they want to bet on.  Will check if users entered number is in range before returning the value as type int
int getnumbertobeton();

//Winnings() function: calculates the users winnings based on the different inputs of the user and the randomly generated numbers:
/*  Parameter definitions:
		wager - the amount the user is betting on
		random_num - the number randomly generated in the range 0-36
		number_bet - the number the user input in the range 0-36
		even_odd - character the user input, (E, O, or N) for their choice of even/odd
		red_black - character the user input, (R, B, or N) for their choice red/black
		high_low - character the user input (H, L, or N) for their choice of high/low
*/
float winnings(float wager, int random_num, int number_bet, char even_odd, char red_black, char high_low);

//Displays the number generated in the range 0-36. Parameter random_num is the random number generated in the range 0-36:
//Will also draw an animation:
void displayspin(int random_num);

//Tells the user their choices and gets their choice.
/*  Parameteres:
		ch1 - depending on which choices are being presented to the user, this will either be 'E', 'R', or 'H'
		ch2 - depending on which choice is being presented to the user, this will either be 'O', 'B', or 'L'
		fill - changes to either "even/odd/no bet", "red/black/no bet", or "high/low/no bet"
*/
char choice(char ch1, char ch2, string fill);

//Will ask the user what their wager is. Parameter stake used to check whether the users input doesn't exceed how much they have:
float getamounttobet(float stake);




//***Functions***:
int main()
{
	float stake = 100.0; //used to store how much the user has at the beginning and end of the game.  Players start with $100.
	float wager, total_won; //wager is used to store the amount the player bets.  total_won will store the amount won based on the function winnings()
	int number_bet; //stores the value the user inputs for where the they think the ball will land
	int random_num; //these are used to store the four different randomly generated numbers
	char even_odd, red_black, high_low; //stores the choice of the user for the three different choices
	char again = 'y'; //stores the users choice for whether or not they want to play again

	cout << fixed << showpoint << setprecision(2); //sets output to show two decimal places
	srand(time(NULL)); //seed for randomly generated numbers based on the time

	//If variable again is equal to 'y' and the players stake is more than 0, this loop will execute (player starts off with 'y' and stake of 100):
	while (again == 'y' && stake > 0)
	{
		cout << "Spin and Win!!! Roulette!!!" << endl;
		cout << "Your stake is $" << stake << endl << endl;

		number_bet = getnumbertobeton(); //asks and stores the users choice of a number between 0 and 36 by calling the function getnumbertobeton()
		random_num = getrandnum(); //will generate and store a random number between 0 and 36 by calling the function getrandnum() 
		wager = getamounttobet(stake); //asks and stores the users wager by calling function getamounttobet()

		even_odd = choice('E', 'O', "even/odd/no bet"); //asks and stores the choice of the user for E/O into variable even_odd by calling function choice()
		red_black = choice('R', 'B', "red/black/no bet");//similar to even_odd, but for red/black
		high_low = choice('H', 'L', "high/low/no bet");//similar to even_odd, but for high/low
		cout << endl;

		displayspin(random_num); //call to displayspin() functions
		total_won = winnings(wager, random_num, number_bet, even_odd, red_black, high_low); //will make variable total_won equal to how much the player won based on calculations of function winnings()
		stake = stake + total_won; //variable stake will be added to variable total_won to determine how much the user has after winnings

		cout << "Now you have $" << stake << endl << endl; //lets the player know how much they have after winnings


		delay(1200); //will delay the closing of the graphics window by 1.2 seconds
		closegraph(); //will close the graphics window


		//Will determine whether or not the user has enough money.  If user has 0 or less, then the game is over!:
		if (stake > 0)
		{
			cout << "Do you want to play again? (y/n) ";
			cin >> again;
		}
		else
		{
			cout << "Game Over! You are out of money!" << endl;
		}
	}

	cout << "You left the game with $" << stake << endl;


	return 0;
}




int getrandnum()
{
	return rand() % 37; //generates a random number in the range 0-36:
}



int getnumbertobeton()
{
	int bet; //variable to store users input for the number they want to bet on
	bool ct = false; //used to determine whether the user has input a number in the range 0-36

	/* This loop will run until the variable bet has a value greater than or equal to zero and less than or equal to 36. */
	while (ct == false) 
	{
		cout << "What number do you want to bet on? (0-36) ";
		cin >> bet;

		if (bet >= 0 && bet <= 36)
		{
			ct = true; //when user inputs in range of 0-36, ct becomes true to end the loop
		}
		else
		{
			cout << "You have entered an invalid number!" << endl;
		}
	}

	return bet;
}



float winnings(float wager, int random_num, int number_bet, char even_odd, char red_black, char high_low)
{ 
	//variables to store how much the user won based on where the ball landed and the users guess:
	float ball = 0;
	float E_O = 0;
	float R_B = 0;
	float H_L = 0;
	//variable to store the total amount won:
	float total_won;


	//***The following if statements will calculate winnings***:
	//if statement will only run if the users input and generated numbers are the same:
	if (random_num == number_bet)
	{
		ball = wager * 5; //if the if statement does run (meaning the user guessed correct) than their wager is multiplied by 5
		cout << "You win 5 times your bet! You hit the number! $" << ball << endl;
	}


	//If the user inputs 'N' than the if statement does not run (meaning they don't want to bet:
	if (even_odd != 'N')
	{
		//If users inputs correctly, their wager is doubled:
		if ((even_odd == 'E' && random_num % 2 == 0) || (even_odd == 'O' && random_num % 2 != 0))
		{
			E_O = wager * 2;
			cout << "You win two times your bet on odd/even!  $" << E_O << endl;
		}
		//If user does not input correctly, they will lose their wager:
		else
		{
			E_O = wager - wager * 2;
			cout << "You lost your bet on odd/even!  $" << wager << endl;
		}
	}


	//If the user inputs 'N' than the if statement does not run (meaning they don't want to bet:
	if (red_black != 'N')
	{
		//If users input is correct, they win triple their wager:
		if ((red_black == 'R' && random_num % 3 == 0) || (red_black == 'B' && random_num % 3 != 0))
		{
			R_B = wager * 3;
			cout << "You won three times your bet on red/black!  $" << R_B << endl;
		}
		//if users input is incorrect, they lose twice their wager:
		else
		{
			R_B = wager - wager * 3;
			cout << "You lost your bet on red/black!  $" << wager * 2 << endl;
		}
	}


	//If the user inputs 'N' than the if statement does not run (meaning they don't want to bet:
	if (high_low != 'N')
	{
		//If user inputs correctly, they win their wager:
		if ((high_low == 'H' && random_num > 18) || (high_low == 'L' && random_num <= 18))
		{
			H_L = wager;
			cout << "You won your bet on high/low!  $" << wager << endl;
		}
		//If user inputs incorrectly, they lose their wager:
		else
		{
			H_L = wager - wager * 2;
			cout << "You lost your bet on high/low!  $" << wager << endl;
		}
	}


	//The winnings are added up and stored in total, which will be the returned value of this function:
	total_won = ball + E_O + R_B + H_L;
	return total_won;
}



void displayspin(int random_num)
{
	int ct = 0; //used in loop statement for output to console
	int ctr = 0; //used in loop for graphics
	int x = 10; //initial x coordinate of ball in graphics
	int y = 10; //initial y coordinate of ball in graphics


	cout << "Spinning " ;

	//This loop statement will output to the console five decimals, each 100 milliseconds apart:
	while (ct < 5)
	{
		delay(100); //delays execution by 100 milliseconds
		cout << ".";
		ct++;
	}
	//Each delay() will delay execuation by amount inside paranthesis:
	delay(300);
	cout << " 1.";
	delay(450);
	cout << " 2.";
	delay(650);
	cout << " 3..";
	delay(900);
	cout << " 4.";
	delay(1100);
	cout << "And the number is " << random_num << endl << endl;


	//The following will output an animation by using the graphics.h library:
	initwindow(800, 600, "Roulette!");
	circle(400, 300, 120);

	//Animation:
	while(ctr < 8)
	{
		x = x + 50; //moves x coordinate 50 units each time loop runs
		y = y + 20; //moves y coordinate 20 units each time loop runs
		circle(x, y, 10); //draw a circle representing the roulette ball
		ctr++;
		delay(100); //delays each output by 100 milliseconds
	}

	//Outputs the number the ball landed on to the graphics window:
	bgiout << random_num << endl; 
	outstreamxy(100, 100);
}



char choice(char ch1, char ch2, string fill)
{
	char user_choice; //stores users input as type char
	bool ct = false; //used to check whether users input is correct type or not

	//Loop will run until user inputs the correct type:
	while (ct == false)
	{
		cout << "Do you want to bet on " << fill << "? (" << ch1 << "/" << ch2 << "/N) ";
		cin >> user_choice;
		user_choice = toupper(user_choice);

		//If statement will check to see if the user has input the correct type of value:
		if (user_choice == ch1 || user_choice == ch2 || user_choice == 'N')
		{
			ct = true; //when the user enters the correct type of char, ct becomes true to end the loop
		}
		else
		{
			cout << "Invalid type!" << endl;
		}
	}

	return user_choice;
}



float getamounttobet(float stake)
{
	float amount; //variable to hold how much the user wants to bet
	bool ct = false;

	//Loop gets users input and checks to see whether that input is valid or not.  If not valid, will run again:
	while (ct == false)
	{
		cout << "How much do you want to bet? ";
		cin >> amount;

		if (amount <= 0) //if user enteres value less than or equal to zero, the following will output:
		{
			cout << "Please enter a number greater than zero" << endl;
		}
		else if (amount > stake) //If user enters an number less than amount of variable stake, the following will output: 
		{
			cout << "That is too much!" << endl;
		}
		else
		{
			ct = true; //If the input of the user is valid, variable ct will change to true to end the loop
		}
	}

	return amount;
}