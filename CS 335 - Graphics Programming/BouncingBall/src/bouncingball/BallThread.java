package bouncingball;

import java.awt.Dimension;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import javax.swing.JFrame;

/**
 * @author Ashkan
 */
public class BallThread extends MouseAdapter implements Runnable
{
	JFrame frame;
	Circle myCircle;
	boolean running;
	int counter;
	
	public BallThread()
	{
		//later make this random
		myCircle = new Circle();
		myCircle.setPreferredSize(new Dimension(500, 500));
		myCircle.addMouseListener(this);
		frame = new JFrame();
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().add(myCircle);
		frame.pack();
		frame.setVisible(true);
		counter = 0;
		running = false;
	}
	
	@Override
	public void run()
	{
		while(true)
		{
			if(getRunning())
			{
				myCircle.repaint();

				try
				{
					Thread.sleep(10);
				}
				catch(Exception e)
				{
				}
			}
		}
	}
	
	private synchronized void startRunning()
	{
		running = true;
	}
	
	private synchronized boolean getRunning()
	{
		return running;
	}
	
	@Override
	public void mousePressed(MouseEvent e)
	{
		startRunning();
	}
}
