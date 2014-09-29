package bouncingball;

import java.awt.Graphics;
import javax.swing.JPanel;
import java.util.Random;

/**
 * @author Ashkan
 */
public class Circle extends JPanel
{
	int x, y, vx, vy;
	
	public Circle()
	{
		Random random = new Random(System.currentTimeMillis());
		x = random.nextInt(50);
		y = random.nextInt(50);
		vx = random.nextInt(10)-5;
		vy = random.nextInt(10)-5;
	}
	
  @Override
	public void paintComponent(Graphics g)
	{
		super.paintComponent(g);
		
		if(x+vx > 495 || x+vx < -5) vx = -1*vx;
		if(y+vy > 495 || y+vy < -5) vy = -1*vy;
		
		x = x + vx;
		y = y + vy;
		
		g.drawOval(x, y, 10, 10);
	}
}
