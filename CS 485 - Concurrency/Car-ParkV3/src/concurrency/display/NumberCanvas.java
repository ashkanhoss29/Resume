/********************************************************/
// j.n.magee 16.4.98
// j.n.magee revised 8.8.2004

package concurrency.display;

import java.awt.*;
import javax.swing.*;

public class NumberCanvas extends JPanel {
    int value_ = 0;
    String title_;

    Font f1 = new Font("Helvetica",Font.BOLD,36);
    Font f2 = new Font("Times",Font.ITALIC+Font.BOLD,24);

    public NumberCanvas(String title) {
        this(title,Color.cyan);
    }

    public NumberCanvas(String title,Color c) {
        super();
        title_=title;
        setBackground(c);
	}

    public void setcolor(Color c){
        setBackground(c);
        repaint();
    }

    public void setvalue(int newval){
        value_ = newval;
        repaint();
    }

    public void paint(Graphics g){

            Graphics2D g2 = (Graphics2D)g;
            g2.setRenderingHint(RenderingHints.KEY_TEXT_ANTIALIASING,RenderingHints.VALUE_TEXT_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,RenderingHints.VALUE_ANTIALIAS_ON);
            g2.setRenderingHint(RenderingHints.KEY_RENDERING,RenderingHints.VALUE_RENDER_QUALITY);
            
            g2.setColor(Color.black);

         // Display the title
            g2.setFont(f2);
            FontMetrics fm = g2.getFontMetrics();
            int w = fm.stringWidth(title_);
            int h = fm.getHeight();
            int x = (getSize().width - w)/2;
            int y = h;
            g2.drawString(title_, x, y);
            g2.drawLine(x,y+3,x+w,y+3);


         // Display the value
            g2.setFont(f1);
            fm = g2.getFontMetrics();
            String s1 = String.valueOf(value_);
            w = fm.stringWidth(s1);
            h = fm.getHeight();
            x = (getSize().width - w)/2;
            y = (getSize().height+ h)/2;
            g2.drawString(s1, x, y);
         }
}
