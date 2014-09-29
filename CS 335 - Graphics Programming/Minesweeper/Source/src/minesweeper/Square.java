//Ashkan Hosseini
//CS335
//10/01/2012

package minesweeper;

import javax.swing.ImageIcon;
import javax.swing.JButton;

public class Square
{
	JButton button;
	SquareIdentity identity;
	SquareDisplayState state;
	boolean clicked;
	int number;
	private static ImageIcon newIcon, emptyIcon, flagIcon, mineIcon, numberIcons[];
	
	public Square()
	{
		button = new JButton();
		
		setNew();
		
		button.setFocusable(false);
		button.setEnabled(true);
		
		identity = SquareIdentity.EMPTY;
		state = SquareDisplayState.NEW;
		clicked = false;
		number = 0;
	}
	
	public static void createIcons()
	{
		newIcon = new ImageIcon("images\\new.jpg");
		emptyIcon = new ImageIcon("images\\empty.jpg");
		flagIcon = new ImageIcon("images\\flag.jpg");
		mineIcon = new ImageIcon("images\\mine.jpg");
		
		numberIcons = new ImageIcon[8];
		numberIcons[0] = new ImageIcon("images\\one.jpg");
		numberIcons[1] = new ImageIcon("images\\two.jpg");
		numberIcons[2] = new ImageIcon("images\\three.jpg");
		numberIcons[3] = new ImageIcon("images\\four.jpg");
		numberIcons[4] = new ImageIcon("images\\five.jpg");
		numberIcons[5] = new ImageIcon("images\\six.jpg");
		numberIcons[6] = new ImageIcon("images\\seven.jpg");
		numberIcons[7] = new ImageIcon("images\\eight.jpg");
	}
	
	public static void resizeIcons(int width, int height)
	{
		newIcon = new ImageIcon(newIcon.getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		emptyIcon = new ImageIcon(emptyIcon.getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		flagIcon = new ImageIcon(flagIcon.getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		mineIcon = new ImageIcon(mineIcon.getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		
		numberIcons[0] = new ImageIcon(numberIcons[0].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[1] = new ImageIcon(numberIcons[1].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[2] = new ImageIcon(numberIcons[2].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[3] = new ImageIcon(numberIcons[3].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[4] = new ImageIcon(numberIcons[4].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[5] = new ImageIcon(numberIcons[5].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[6] = new ImageIcon(numberIcons[6].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
		numberIcons[7] = new ImageIcon(numberIcons[7].getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
	}
	
	public void setFlag()
	{
		state = SquareDisplayState.FLAG;
		button.setIcon(flagIcon);
	}
	
	public void setMine()
	{
		button.setIcon(mineIcon);
	}
	
	public void setEmpty()
	{
		state = SquareDisplayState.EMPTY;
		button.setIcon(emptyIcon);
	}
	
	public void setNew()
	{
		state = SquareDisplayState.NEW;
		button.setIcon(newIcon);
	}
	
	public void setNumber()
	{
		button.setIcon(numberIcons[number-1]);
	}
}