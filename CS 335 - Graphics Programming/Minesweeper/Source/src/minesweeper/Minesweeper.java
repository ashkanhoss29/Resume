//Ashkan Hosseini
//CS335
//10/01/2012

package minesweeper;

import java.io.IOException;
import javax.swing.*;

public class Minesweeper
{
	public static void main(String args[])
	{
		try
		{
			UIManager.setLookAndFeel("com.sun.java.swing.plaf.windows.WindowsLookAndFeel");
			Window window = new Window(9, 9, 10);
		}
		catch(ClassNotFoundException | InstantiationException | IllegalAccessException | UnsupportedLookAndFeelException ex)
		{
			ex.printStackTrace();
		}
		catch(IOException ex)
		{
			ex.printStackTrace();
		}
	}
}