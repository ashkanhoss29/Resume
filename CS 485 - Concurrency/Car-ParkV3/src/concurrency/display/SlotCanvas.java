/*
@author  j.n.magee 20/04/98
*/
package concurrency.display;

import java.awt.*;
import java.applet.*;


/**************************************************************/

public class SlotCanvas extends Canvas {
    String title_;
    int slots_;
    String[] buf_;
    private final static String empty = "  ";

    Font f1 = new Font("Helvetica",Font.ITALIC+Font.BOLD,24);
    Font f2 = new Font("TimesRoman",Font.BOLD,36);

    public SlotCanvas(String title, Color c, int slots) {
        super();
        title_=title;
        slots_=slots;
        buf_ = new String[slots_];
        for(int i=0; i<buf_.length;i++) buf_[i]=empty;
        setSize(20+50*slots_,120);
        setBackground(c);
  	}

  	private int index(String s){
  	    for(int i=0;i<buf_.length;i++)
  	       if(buf_[i].equals(s)) return i;
  	    return -1;
  	}

  	public synchronized void enter(String name){
  	    int i = index(empty);
  	    if (i>=0) buf_[i] = name;
  	    repaint();
  	}

    public synchronized void leave(String name) {
        int i = index(name);
        if (i>=0) buf_[i]=empty;
        repaint();
    }


    public void paint(Graphics g) {
        update(g);
    }

    Image offscreen;
    Dimension offscreensize;
    Graphics2D offgraphics;

    public synchronized void update(Graphics g){
        Dimension d = getSize();
	    if ((offscreen == null) || (d.width != offscreensize.width)
	                            || (d.height != offscreensize.height)) {
	        offscreen = createImage(d.width, d.height);
	        offscreensize = d;
	        offgraphics = (Graphics2D)offscreen.getGraphics();
                offgraphics.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
                offgraphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
                offgraphics.setRenderingHint(RenderingHints.KEY_RENDERING,RenderingHints.VALUE_RENDER_QUALITY);
	        offgraphics.setFont(getFont());
	    }

	    offgraphics.setColor(getBackground());
	    offgraphics.fillRect(0, 0, d.width, d.height);

         // Display the title
        offgraphics.setColor(Color.black);
        offgraphics.setFont(f1);
        FontMetrics fm = offgraphics.getFontMetrics();
        int w = fm.stringWidth(title_);
        int h = fm.getHeight();
        int x = (getSize().width - w)/2;
        int y = h;
        offgraphics.drawString(title_, x, y);
        offgraphics.drawLine(x,y+3,x+w,y+3);
        // Buffer Boxes
        y = getSize().height/2 - 15;
        offgraphics.setColor(Color.white);
        offgraphics.fillRect(10,y,50*slots_,50);
        offgraphics.setColor(Color.black);
        offgraphics.setFont(f2);
        for(int i=0; i<slots_; i++) {
            offgraphics.drawRect(10+50*i,y,50,50);
            if (buf_[i]!=null)
                offgraphics.drawString(buf_[i],20+50*i,y+35);
        }
        g.drawImage(offscreen, 0, 0, null);
     }
}


/**************************************************************/
