/*
@author  j.n.magee 14/11/96
*/
//Ported to Swing and modified for CS485 -- Spring2013
//provided by instructors

package concurrency.carpark;

import javax.swing.*;
import java.awt.*;
import concurrency.display.*;
import java.io.File;
import java.net.URL;
import java.util.Random;
import javax.imageio.ImageIO;

/*********************CARPARK CONTROL*****************************/

class CarParkControl {

    protected volatile int spaces; //number of spaces currently occupied
    protected final int capacity; //capacity of parking garage (never changes)
    protected volatile int queueSpaces;
    protected final int queueCapacity;

    CarParkControl(int n) {
        capacity = spaces = n;
        queueCapacity = queueSpaces = 3;
    }

    synchronized void arrive() throws InterruptedException {
        while (spaces==0) wait();
        --spaces;
        notifyAll();
    }
    
    synchronized void queue() throws InterruptedException {
        while(queueSpaces==0 || spaces==capacity) wait();
        --queueSpaces;
        notifyAll();
    }

    synchronized void depart() throws InterruptedException{
        while (queueSpaces==queueCapacity) wait();
        ++spaces;
        ++queueSpaces;
        notifyAll();
     }
}



/*******************ARRIVALS************************/

class Arrivals1 implements Runnable {

    DisplayCarPark carpark;

    Arrivals1(DisplayCarPark c) {
        carpark = c;
    }

    public void run() {
      try {
        while(true) {
           ThreadPanel.rotate(330);
           carpark.arrive1();
           ThreadPanel.rotate(30);
        }
      } catch (InterruptedException e){}
    }
}

class Arrivals2 implements Runnable {

    DisplayCarPark carpark;

    Arrivals2(DisplayCarPark c) {
        carpark = c;
    }

    public void run() {
      try {
        while(true) {
           ThreadPanel.rotate(330);
           carpark.arrive2();
           ThreadPanel.rotate(30);
        }
      } catch (InterruptedException e){}
    }
}

/********************QUEUE*******************************/
class Queue implements Runnable {
    
    DisplayCarPark carpark;

    Queue(DisplayCarPark c) {
        carpark = c;
    }

    public void run() {
      try {
        while(true) {
            ThreadPanel.rotate(330);
            carpark.queue();
            ThreadPanel.rotate(30);

        }
      } catch (InterruptedException e){}
    }
}

/********************DEPARTURES*******************************/

class Departures implements Runnable {

    DisplayCarPark carpark;

    Departures(DisplayCarPark c) {
        carpark = c;
    }

    public void run() {
      try {
        while(true) {
            ThreadPanel.rotate(180);
            carpark.depart();
            ThreadPanel.rotate(180);

        }
      } catch (InterruptedException e){}
    }
}

/****************************APPLET**************************/
interface ImageLoader
{
    public Image LoadImage(URL urlBase, String strPath);
    public URL GetDocBase();
}

interface ValueUpdater
{
    public void updateArrivals1(int num);
    public void updateArrivals2(int num);
    public void updateGarage(int num);
    public void updateOccupied(int num);
    public void updateQueue(int num);
    public void updateDepartures(int num);
}

//public class CarPark extends JApplet {
public class CarPark extends JFrame implements ImageLoader, ValueUpdater {

    final static int Places = 16;
    public static void main(String args[])
    {
        CarPark pCarPark = new CarPark();
        pCarPark.InitializeAndShow();
        pCarPark.start();
    }

    JPanel mPanel;
    ThreadPanel arrivals;
    ThreadPanel arrivals2;
    ThreadPanel departures;
    ThreadPanel exitqueue;
    CarParkCanvas carDisplay;
    
    JLabel txtArrivals;
    JLabel txtArrivals2;
    JLabel txtGarage_Occupied;
    JLabel txtQueue;
    JLabel txtDepartures;
    int garage;
    int occupied;

    //public void init() {
    CarPark(){
 //       super.init();
        //InitializeClass();     // UnComment for Applet
        setDefaultCloseOperation (EXIT_ON_CLOSE);    // Comment for Applet
        Runtime.getRuntime().addShutdownHook(new Thread(new Runnable() {
            public void run() {
                //This runs when the application closes
                
                System.out.println("This is where things get written to the log");
            }
        }));
    }

    public void InitializeAndShow()
    {
        mPanel = new JPanel();
        this.setContentPane(mPanel);
        setSize(1200, 600);
        
        // TODO - set correct initial values
        garage = 16;
        occupied = 0;
        
         // Set up Display
        arrivals = new ThreadPanel("Arrivals",Color.blue);
        arrivals2 = new ThreadPanel("Arrivals2",Color.green);
        carDisplay = new CarParkCanvas("CarPark",Places,this);
        exitqueue = new ThreadPanel("Exit Queue", Color.orange);
        departures = new ThreadPanel("Departures",Color.yellow);
        
        txtArrivals = new JLabel("aa");
        txtArrivals.setPreferredSize(new Dimension(90, 30));
        txtArrivals.setSize(txtArrivals.getPreferredSize());
        txtArrivals2 = new JLabel("bb");
        txtArrivals2.setPreferredSize(new Dimension(90, 30));
        txtArrivals2.setSize(txtArrivals2.getPreferredSize());
        txtGarage_Occupied = new JLabel("cc");
        txtGarage_Occupied.setPreferredSize(new Dimension(160, 30));
        txtGarage_Occupied.setSize(txtGarage_Occupied.getPreferredSize());
        txtQueue = new JLabel("dd");
        txtQueue.setPreferredSize(new Dimension(100, 30));
        txtQueue.setSize(txtQueue.getPreferredSize());
        txtDepartures = new JLabel("ee");
        txtDepartures.setPreferredSize(new Dimension(100, 30));
        txtDepartures.setSize(txtDepartures.getPreferredSize());
        
        GridBagLayout gridbag = new GridBagLayout();
        mPanel.setLayout(gridbag);
        GridBagConstraints gc = new GridBagConstraints();
        gc.anchor = GridBagConstraints.NORTH;
        gc.gridwidth = 1;
        gc.gridheight = 1;
        
        JPanel holder = getHolder(arrivals, txtArrivals);
        gridbag.setConstraints(holder, gc);
        mPanel.add(holder);
        
        holder = getHolder(arrivals2, txtArrivals2);
        gridbag.setConstraints(holder, gc);
        mPanel.add(holder);
        
        holder = getHolder(carDisplay, txtGarage_Occupied);
        gridbag.setConstraints(holder, gc);
        mPanel.add(holder);
        
        holder = getHolder(exitqueue, txtQueue);
        gridbag.setConstraints(holder, gc);
        mPanel.add(holder);
        
        holder = getHolder(departures, txtDepartures);
        gridbag.setConstraints(holder, gc);
        mPanel.add(holder);
        mPanel.setBackground(Color.lightGray);

        LoadImages();

        // TODO set correct initial values
        updateArrivals1(0);
        updateArrivals2(0);
        updateGarage(16);
        updateOccupied(0);
        updateQueue(0);
        updateDepartures(0);
        
        setVisible(true);
    }
    
    JPanel getHolder(JPanel main, JLabel secondary)
    {
        JPanel ret = new JPanel();
        
        GridBagLayout gridbag = new GridBagLayout();
        ret.setLayout(gridbag);
        
        GridBagConstraints gc = new GridBagConstraints();
        gc.anchor = GridBagConstraints.NORTH;
        gc.gridwidth = 1;
        gc.gridheight = 1;
        gc.gridx = 0;
        gc.gridy = 0;
        
        gridbag.setConstraints(main, gc);
        ret.add(main);
        
        gc.gridy = 1;
        gridbag.setConstraints(secondary, gc);
        ret.add(secondary);
        
        return ret;
    }

    public void LoadImages()
    {
        //      Image imgLight = LoadImage(GetDocBase(), "image/street-light.png");
        //Image imgLight = LoadImage(null, "image/street-light.png");
        Image imgProgress = LoadImage(null, "image/progressImage.png");
        Image imgRun = LoadImage(null, "image/Check.png");
        Image imgPause = LoadImage(null, "image/Ex.png");

        arrivals.setRunPauseImages(imgRun, imgPause);
        arrivals.initializeCanvas();
        arrivals.setProgressImage(imgProgress);
        arrivals2.setRunPauseImages(imgRun, imgPause);
        arrivals2.initializeCanvas();
        arrivals2.setProgressImage(imgProgress);
        
        exitqueue.setRunPauseImages(imgRun, imgPause);
        exitqueue.initializeCanvas();
        exitqueue.setProgressImage(imgProgress);
        
        departures.setRunPauseImages(imgRun, imgPause);
        departures.initializeCanvas();
        departures.setProgressImage(imgProgress);
    }

    public Image LoadImage(URL urlBase, String strPath)
    {
        try
        {
            if(urlBase != null)
            {
                URL url = new URL(urlBase, strPath);
                return ImageIO.read(url);
            }
            else
                return ImageIO.read(new File(strPath));
        }
        catch(Exception e)
        {}
        return null;
    }

    public URL GetDocBase()
    {
        return null;
    }

    ///////////////////////////////////////////////////
    // Implementation of ValueUpdater    
    public void updateArrivals1(int num)
    {
        txtArrivals.setText("Arrivals: " + Integer.toString(num));
    }
    public void updateArrivals2(int num)
    {
        txtArrivals2.setText("Arrivals2: " + Integer.toString(num));
    }
    public void updateOccupied(int num)
    {
        occupied = num;
        updateGarageOccupied();
    }
    public void updateGarage(int num)
    {
        garage = num;
        updateGarageOccupied();
    }
    public void updateQueue(int num)
    {
        txtQueue.setText("Exit Queue: " + Integer.toString(num));
    }
    public void updateDepartures(int num)
    {
        txtDepartures.setText("Departures: " + Integer.toString(num));
    }
    // END        
    private void updateGarageOccupied()
    {
        txtGarage_Occupied.setText("Garage: " + Integer.toString(garage) + "        " +
                                   "Occupied: " + Integer.toString(occupied));
    }
    
    public void start() {
        DisplayCarPark c = new DisplayCarPark(this, carDisplay,Places);
        arrivals.start(new Arrivals1(c));
        departures.start(new Departures(c));
        
        // TODO start arrivals2 thread
        arrivals2.start(new Arrivals2(c));
        
        // TODO start exit queue thread
        exitqueue.start(new Queue(c));
    }


    public void stop() {
        arrivals.stop();
        departures.stop();
        arrivals2.stop();
        exitqueue.stop();
    }

}

/**************************************************************/

class DisplayCarPark extends CarParkControl 
{
    ValueUpdater valueUpdater;
    CarParkCanvas disp;
    boolean occupied[];
    
    //volatile counters for all of the GUI text boxes
    private volatile int arrivals1 = 0;
    private volatile int arrivals2 = 0;
    private volatile int queue = 0;
    private volatile int departures = 0;
    private volatile int occupiedSpaces = 0;

    DisplayCarPark(ValueUpdater updater, CarParkCanvas disp,int size) {
        super(size);
        
        valueUpdater = updater;
        this.disp = disp;
        occupied = new boolean[size];
        for (int i=0; i<size; i++) occupied[i]=false;
    }

    private void display() {
        disp.setvalue(spaces,occupied);
    }

    // TODO - add methods for arrive2 and exit queue
    
    @Override
    synchronized public void arrive() throws InterruptedException {
        super.arrive();
        
        // TODO update arrivals1
        //valueUpdater.updateArrivals1(arrivals1);
        
        occupied[place(false)]=true;
        display();
        Thread.sleep(4);
    }
    
    synchronized public void arrive1() throws InterruptedException {
        arrive();
        arrivals1++;
        occupiedSpaces++;
        valueUpdater.updateOccupied(occupiedSpaces);
        valueUpdater.updateArrivals1(arrivals1);
    }
    
    synchronized public void arrive2() throws InterruptedException { 
        arrive();
        arrivals2++;
        occupiedSpaces++;
        valueUpdater.updateOccupied(occupiedSpaces);
        valueUpdater.updateArrivals2(arrivals2);
    }
    
    @Override
    synchronized public void queue() throws InterruptedException {
        super.queue();
        queue++;
        valueUpdater.updateQueue(queue);
    }

    @Override
    synchronized public void depart() throws InterruptedException {
        super.depart();
        occupied[place(true)]=false;
        
        // TODO update arrivals1
        departures++;
        occupiedSpaces--;
        queue--;
        valueUpdater.updateQueue(queue);
        valueUpdater.updateOccupied(occupiedSpaces);
        valueUpdater.updateDepartures(departures);
        
        display();
    }

    private int place(boolean v) {
        int start = ((int)(Math.random() * 1000))% capacity;
        for (int i =0; i<capacity; i++) {
            int j = (start + i) % capacity;
            if(occupied[j] == v) return j;
        }
        return 0; //should never happen
    }

 }

/**************************************************************/

class CarParkCanvas extends  JPanel {

    private static Random randGen = new Random();

    String title;
    int m_iNumCars;
    int slots;
    int spaces;
    boolean occupied[];
    int carImageForSlot[];       // index of the car for slot
    Image carImages[];           // array of car images
    ImageLoader imgLoader;
    Image  car;
    Image parkLot;

    Font f1 = new Font("Helvetica",Font.ITALIC+Font.BOLD,24);
    Font f2 = new Font("TimesRoman",Font.BOLD,36);

    CarParkCanvas(String title, int slots, ImageLoader imgLoader) {
        super();
        this.title=title;
        this.slots=slots;
        spaces = slots;
        this.imgLoader = imgLoader;
        this.occupied = new boolean[slots];
        this.carImageForSlot = new int[slots];
        this.m_iNumCars = 0;

        for (int i=0; i<slots; i++)
        {
            occupied[i] = false;
            carImageForSlot[i] = -1;
        }
        setPreferredSize(new Dimension(340, 228));
        setBackground(Color.cyan);

        MediaTracker mt;
        mt = new MediaTracker(this);
        parkLot = imgLoader.LoadImage(imgLoader.GetDocBase(), "image/park-lot-top_16.png");
        mt.addImage(parkLot, 0);
        LoadCarImages(mt, 1);

        try {
            mt.waitForID(0);
        } catch (java.lang.InterruptedException e) {
            System.out.println("Couldn't load car image");
        }
    }

    public int LoadCarImages(MediaTracker mt, int iStartIndex)
    {
        m_iNumCars = 7;
        carImages = new Image[m_iNumCars];
        for(int i = 0; i < m_iNumCars; ++i)
            carImages[i] = null;
        
        carImages[0] = LoadImage("image/blue-truck-top_small.png", imgLoader.GetDocBase());
        carImages[1] = LoadImage("image/blue-truck-top_orange_small.png", imgLoader.GetDocBase());
        carImages[2] = LoadImage("image/blue-truck-top_red_small.png", imgLoader.GetDocBase());
        carImages[3] = LoadImage("image/blue-truck-top_green_small.png", imgLoader.GetDocBase());
        carImages[4] = LoadImage("image/blue-truck-top_lightyellow_small.png", imgLoader.GetDocBase());
        carImages[5] = LoadImage("image/blue-truck-top_gray_small.png", imgLoader.GetDocBase());
        carImages[6] = LoadImage("image/blue-truck-top_bluegreen_small.png", imgLoader.GetDocBase());

        for(int i = 0; i < m_iNumCars; ++i)
        {
            mt.addImage(carImages[i], iStartIndex);
            ++iStartIndex;
        }
        return iStartIndex;
    }

    public Image LoadImage(String strPath, URL urlBase)
    {
        return imgLoader.LoadImage(urlBase, strPath);
    }

 // Crash problem
    public void OccupySlots(boolean occupiedSlots[])
    {
        occupied = occupiedSlots;
        for(int i = 0; i < slots; ++i)
        {
            if(!occupied[i])
            {
                if(carImageForSlot[i] >= 0)
                    System.out.println("Car just left: " + i);
                carImageForSlot[i] = -1;
            }
            else if(occupied[i] && carImageForSlot[i] < 0)
            {
                int iCarIndex = Math.abs(randGen.nextInt()) % m_iNumCars;
                carImageForSlot[i] = iCarIndex;
                System.out.println("new car: " + i + " image: " + iCarIndex);
            }
        }
    }

    public void setvalue(int spaces, boolean occupied[]){
        this.spaces = spaces;
        OccupySlots(occupied);
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
        int w = fm.stringWidth(title);
        int h = fm.getHeight();
        int x = (getSize().width - w)/2;
        int y = h;

        // CarPark Places
        offgraphics.drawImage(parkLot, 0, -8, this);

        // Top text
        offgraphics.drawString(title, x, y);
        offgraphics.drawLine(x,y+3,x+w,y+3);
   
        //arrival gate
        offgraphics.setColor(Color.black);
        offgraphics.fillRect(0,y+15,12,180);
        if (spaces==0)
             offgraphics.setColor(Color.red);
        else
             offgraphics.setColor(Color.white);
        offgraphics.fillRect(2,y+17,8,176);

        //departure gate
        offgraphics.setColor(Color.black);
        offgraphics.fillRect(331,y+15,12,180);
        if (spaces==slots)
             offgraphics.setColor(Color.red);
        else
             offgraphics.setColor(Color.white);
        offgraphics.fillRect(333,y+17,8,176);
        //Display Cars
       
        for (int i=0; i<slots; i++)
        {
            if (occupied[i])
            {
                int iCarImageIndex = carImageForSlot[i];
                if(iCarImageIndex < 0 && iCarImageIndex > slots)
                    iCarImageIndex = 0;
                Image carImage = carImages[iCarImageIndex];

                int iXPos = 0;
                int iYpos = 0;
                if(i < 8)
                {
                    iXPos = 8+40*i;
                    iYpos = y+10;

                    if(i >= 4)
                        iXPos += 5;
                }
                else
                {
                    iXPos = 8+40*(i-8);
                    iYpos = y+105;

                    if(i >= 12)
                        iXPos += 5;
                }

                System.out.println(iXPos + " " + iYpos + " " + i);
                offgraphics.drawImage(carImage,iXPos,iYpos,this);
            }
        }
        g.drawImage(offscreen, 0, 0, null);
    }
}


