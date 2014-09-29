package cs335;

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.media.opengl.*;
import com.sun.opengl.util.FPSAnimator;

public class SimpleJOGL extends MouseAdapter implements KeyListener {

    static JFrame frame;
    GLRender listener = new GLRender();
    static FPSAnimator animator = null;
	Point lastMouse = new Point();
	int mouseButton;

    public SimpleJOGL() throws HeadlessException {
        frame = new JFrame("Racing Car Game");
        frame.setSize(600, 480);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        GLCapabilities glcaps = new GLCapabilities();
        GLCanvas canvas = new GLCanvas(glcaps);
        canvas.addGLEventListener(listener);
        canvas.addMouseListener(this);
		canvas.addMouseMotionListener(this);
		canvas.addKeyListener(this);
		frame.addKeyListener(this);
        frame.getContentPane().add(canvas, BorderLayout.CENTER);
        animator = new FPSAnimator(canvas, 10, true);
		
        centerWindow(frame);
    }

    private void centerWindow(Component frame) {
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        Dimension frameSize = frame.getSize();
        if (frameSize.width > screenSize.width) {
            frameSize.width = screenSize.width;
        }
        if (frameSize.height > screenSize.height) {
            frameSize.height = screenSize.height;
        }
        frame.setLocation((screenSize.width - frameSize.width) >> 1,
                (screenSize.height - frameSize.height) >> 1);
		listener.wWidth = screenSize.width-frameSize.width;
		listener.wHeight = screenSize.height-frameSize.width;

    }

    public static void main(String[] args) {
        final SimpleJOGL app = new SimpleJOGL();
        // Display
        SwingUtilities.invokeLater(new Runnable() {

            public void run() {
                frame.setVisible(true);
            }
        });
        // Start animator
        SwingUtilities.invokeLater(new Runnable() {

            public void run() {
                animator.start();
            }
        });
    }
    
    @Override
    public void mouseClicked(MouseEvent e)
    {
        //System.out.println("test");
    }
	
	@Override
	public void mousePressed(MouseEvent e)
	{
		lastMouse = e.getPoint();
		mouseButton = e.getButton();
	}
	
	@Override
	public void mouseReleased(MouseEvent e)
	{
		
	}
	
	@Override
	public void mouseDragged(MouseEvent e)
	{
		if(mouseButton == 1)
		{
				listener.RotateX((-((float)lastMouse.x-(float)e.getX()))/5);
				listener.RotateY((-((float)lastMouse.y-(float)e.getY()))/5);
				listener.look(
						(-((float)lastMouse.x-(float)e.getX()))/150, 
						(-((float)lastMouse.y-(float)e.getY()))/150);
		}
		else if(mouseButton == 3)
		{
				listener.scaleU = listener.scaleU + (-((float)lastMouse.y-(float)e.getY()))/5;
		}
		
		lastMouse = e.getPoint();
	}
    
    
    public void keyDown(int keyCode)
	{
	}

	public void keyDownOnce(int keyCode)
	{
	}

	public synchronized void keyPressed(KeyEvent e)
	{
		if(e.getKeyCode() == KeyEvent.VK_UP)
		{
			listener.physics.Accelerate();
		}
		if(e.getKeyCode() == KeyEvent.VK_DOWN)
		{
			listener.physics.Deaccelerate();
		}
		if(e.getKeyCode() == KeyEvent.VK_RIGHT)
		{
			listener.physics.TurnRight();
		}
		if(e.getKeyCode() == KeyEvent.VK_LEFT)
		{
			listener.physics.TurnLeft();
		}
		if(e.getKeyCode() == KeyEvent.VK_C)
		{
			if(listener.overviewMode)
                        {
                            listener.overviewMode = false;
                            listener.focalLength = 15.0f;
                        }
			else
                        {
                            listener.overviewMode = true;
                            listener.focalLength = 50.0f;
                        }
			listener.reset();
			listener.resetFirstPerson();
		}
		if(e.getKeyCode() == KeyEvent.VK_1)
		{
			listener.focalLength += 5.0f;
                        System.out.println(listener.focalLength);
		}
		if(e.getKeyCode() == KeyEvent.VK_2)
		{
			listener.focalLength -= 5.0f;
                                                System.out.println(listener.focalLength);

		}
		
		
		if(e.getKeyCode() == 82)
		{
			listener.reset();
			listener.resetFirstPerson();
			listener.lightOn = true;
			listener.blendOn = false;
			listener.replaceOn = false;
			//listener.overviewMode = true;
		}
		
		if(e.getKeyCode() == 87)
		{
			listener.move(true);
		}
		if(e.getKeyCode() == 83)
		{
			listener.move(false);
		}
		
		if(e.getKeyCode() == 44)
		{
			if(listener.blendOn) listener.blendOn = false;
			else listener.blendOn = true;
		}
		if(e.getKeyCode() == 46)
		{
			if(listener.replaceOn) listener.replaceOn = false;
			else listener.replaceOn = true;
		}
		
		if(e.getKeyCode() == 47)
		{
			if(listener.lightOn) listener.lightOn = false;
			else listener.lightOn = true;
		}
		
		if(e.getKeyCode() == 70)
		{
			if(listener.circleTrack) listener.circleTrack = false;
			else listener.circleTrack = true;
		}
		
//		System.out.println("key: " + e.getKeyCode());
	}

	public synchronized void keyReleased(KeyEvent e)
	{
            if (e.getKeyCode() == KeyEvent.VK_UP || e.getKeyCode() == KeyEvent.VK_DOWN) {
                listener.physics.Released();
            }
	}

	public void keyTyped(KeyEvent e)
	{
	}
}