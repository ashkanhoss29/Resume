* An emulation of Windows XP Minesweeper program *

There are five java files in this program: Minesweeper, Window, Square, SquareIdentity, and SquareDisplayState.

-Minesweeper:
This file contains the main method. Not much goes on here except that the look and feel of the program is set and an instance of Window class is created, sending it a default of 9x9 tiles and 10 mines.

-Window:
The core of the program lies here. Almost all of the GUI and logic is placed here. This includes the main program window that contains the mine counter, timer, reset button, playing field, and top menus. It also contains child windows that pop up such as the options menu and restart dialog box. The Window class extends MouseAdapter for knowing when the user clicks a button/square and implements ActionListener for knowing when the user clicks on a menu and updating the timer.

-Square:
This class is used as a container for the squares that should be displayed in the playing area of Window. One instance of this class equates to one square on the playing area. It contains the button that acts as the GUI side and it also contains states and identities for the square (for example is there a mine on this square, has this square been pressed yet, etc). Square also has the ability to load images and store them on the button so we get it to look similar to the xp version of Minesweeper.

-SquareIdentity
In this file is an enum that helps with giving each square an identity. The three identities that a square can possess are: EMPTY, MINE, and NUMBER. When a square is created and when mines are placed, the identity of that square is set appropriately.

-SquareDisplayState
Similar to SquareIdentity, this file contains an enum. This enum helps keep track of each squares current state: has it been clicked, does it have a flag on it?