//Ashkan Hosseini
//CS335
//10/01/2012

package minesweeper;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.util.Random;
import javax.swing.*;
import javax.swing.border.BevelBorder;

public class Window extends MouseAdapter implements ActionListener
{
	boolean gameStarted, gameOver;
	private int rows, columns, numberOfMines, counter, internalCounter, lastOption;
	private Timer timer;
	private Point framePos;
	private JFrame frame, optionFrame;
	private JMenuBar menuBar;
	private JMenu gameMenu;
	private JMenuItem newGameMenuItem, optionMenuItem, exitMenuItem;
	private JPanel topPanel, playingField;
	private JTextField counterTextField, timerTextField;
	private JButton restartButton, optionOkButton, optionCancelButton;
	private JRadioButton beginnerRadio, intermediateRadio, expertRadio, customRadio;
	private JTextField optionHeight, optionWidth, optionMines;
	private Square squares[][];
	private boolean leftMousePressed, leftMouseReleased, leftMouseExited;
	
	public Window(int startingRows, int startingColumns, int startingNumberOfMines) throws IOException
	{
		if(numberOfMines >= startingRows*startingColumns)
		{
			throw new IOException("Too many mines!");
		}
		
		framePos = new Point(0,0);
		rows = startingRows;
		columns = startingColumns;
		numberOfMines = startingNumberOfMines;
		lastOption = 0;
		
		optionOkButton = new JButton("OK");
		optionOkButton.setEnabled(true);
		
		optionCancelButton = new JButton("Cancel");
		optionCancelButton.setEnabled(true);
		
		createFrame();
	}
	
	private void createFrame()
	{
		gameStarted = false;
		gameOver = false;
		leftMousePressed = false;
		leftMouseReleased = false;
		leftMouseExited = false;
		
		frame = new JFrame("Minesweeper");
		frame.setLocation(framePos);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		
		createMenu();
		createTopPanel();
		createPlayingField();
		
		timer = new Timer(1000, this);
		timer.stop();
		
		frame.getContentPane().add(menuBar, BorderLayout.NORTH);
		frame.getContentPane().add(topPanel, BorderLayout.CENTER);
		frame.getContentPane().add(playingField, BorderLayout.SOUTH);
		frame.pack();
		frame.setVisible(true);
	}
	
	private void createMenu()
	{
		menuBar = new JMenuBar();
		
		gameMenu = new JMenu("Game");
		menuBar.add(gameMenu);
		
		newGameMenuItem = new JMenuItem("New Game     ");
		newGameMenuItem.setAccelerator(KeyStroke.getKeyStroke("F2"));
		newGameMenuItem.addActionListener(this);
		
		optionMenuItem = new JMenuItem("Options");
		optionMenuItem.setAccelerator(KeyStroke.getKeyStroke("F3"));
		optionMenuItem.addActionListener(this);
		
		exitMenuItem = new JMenuItem("Exit");
		exitMenuItem.addActionListener(this);
		
		gameMenu.add(newGameMenuItem);
		gameMenu.add(optionMenuItem);
		gameMenu.add(exitMenuItem);
	}
	
	private void createTopPanel()
	{
		topPanel = new JPanel();
		topPanel.setLayout(new GridLayout(0, 3));
		topPanel.setBorder(BorderFactory.createBevelBorder(BevelBorder.RAISED));
		topPanel.setPreferredSize(new Dimension(20, 35));
		
		counterTextField = new JTextField("0");
		counterTextField.setEditable(false);
		timerTextField = new JTextField("0");
		timerTextField.setEditable(false);
		
		restartButton = new JButton("Start");
		restartButton.setEnabled(true);
		restartButton.addMouseListener(this);
		
		topPanel.add(counterTextField);
		topPanel.add(restartButton);
		topPanel.add(timerTextField);
	}
	
	//Parameters; x: rows, y: columns, n: number of mines
	private void createPlayingField()
	{
		gameStarted = false;
		counter = numberOfMines;
		internalCounter = 0;
		
		counterTextField.setText(Integer.toString(numberOfMines));
		
		GridLayout layout = new GridLayout(rows, columns);
		playingField = new JPanel();
		playingField.setLayout(layout);
		
		Dimension size = new Dimension(24*(columns), 24*(rows));
		playingField.setPreferredSize(size);
		
		squares = new Square[rows][columns];
		Square.createIcons();
		Square.resizeIcons(24, 24);
		for(int i = 0; i < rows; i++)
		{
			for(int j = 0; j < columns; j++)
			{
				squares[i][j] = new Square();
				squares[i][j].button.addMouseListener(this);
				squares[i][j].button.addMouseMotionListener(this);
				playingField.add(squares[i][j].button);
			}
		}
	}
	
	private void createOptionFrame()
	{
		optionFrame = new JFrame("Options");
		optionFrame.setLocation(frame.getLocation());
		optionFrame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		optionFrame.setResizable(false);
		
		//JRadioButton beginnerRadio, intermediateRadio, expertRadio, customRadio;
		beginnerRadio = new JRadioButton("Beginner: 10 mines, 9x9 tiles");
		beginnerRadio.setSelected(true);
		intermediateRadio = new JRadioButton("Intermediate: 40 mines, 16x16 tiles");
		expertRadio = new JRadioButton("Advanced: 99 mines, 16x30 tiles");
		customRadio = new JRadioButton("Custom");
		
		ButtonGroup optionGroup = new ButtonGroup();
		optionGroup.add(beginnerRadio);
		optionGroup.add(intermediateRadio);
		optionGroup.add(expertRadio);
		optionGroup.add(customRadio);
		
		JPanel radioPanel = new JPanel(new GridLayout(0,1));
		radioPanel.add(beginnerRadio);
		radioPanel.add(intermediateRadio);
		radioPanel.add(expertRadio);
		radioPanel.add(customRadio);
		
		optionHeight = new JTextField(Integer.toString(rows));
		optionWidth = new JTextField(Integer.toString(columns));
		optionMines = new JTextField(Integer.toString(numberOfMines));
		
		JPanel textPanel = new JPanel(new GridLayout(3, 2));
		textPanel.add(new JLabel("Height: "));
		textPanel.add(optionHeight);
		textPanel.add(new JLabel("Width: "));
		textPanel.add(optionWidth);
		textPanel.add(new JLabel("Mines: "));
		textPanel.add(optionMines);
		
		optionOkButton.addMouseListener(this);
		optionCancelButton.addMouseListener(this);
		
		JPanel buttonPanel = new JPanel(new GridLayout(1,0));
		buttonPanel.add(optionOkButton);
		buttonPanel.add(optionCancelButton);
		
		optionFrame.getContentPane().add(radioPanel, BorderLayout.NORTH);
		optionFrame.getContentPane().add(textPanel, BorderLayout.CENTER);
		optionFrame.getContentPane().add(buttonPanel, BorderLayout.SOUTH);
		
		frame.getContentPane().add(menuBar, BorderLayout.NORTH);
		frame.getContentPane().add(topPanel, BorderLayout.CENTER);
		frame.getContentPane().add(playingField, BorderLayout.SOUTH);
		
		
		optionFrame.pack();
		optionFrame.setVisible(true);
	}
	
	private void placeMines(int startX, int startY)
	{
		Random random = new Random();
		int minesPlaced = 0;
		while(minesPlaced < numberOfMines)
		{
			//random x
			//random y
			int randX = random.nextInt(rows);
			int randY = random.nextInt(columns);
			
			//don't put mine where player first clicks
			if((randX > startX + 1 || randX < startX - 1) || (randY > startY + 1 || randY < startY - 1))
			{
				//if x, y don't already have mine then add and increment
				if(squares[randX][randY].identity != SquareIdentity.MINE)
				{
					squares[randX][randY].identity = SquareIdentity.MINE;
					incrementSurroundingSquares(randX, randY);
					minesPlaced++;
				}
			}
		}
	}
	
	//x and y should be zero indexed
	private void incrementSurroundingSquares(int x, int y)
	{
		if((x-1) >= 0)
		{
			if(squares[x-1][y].identity != SquareIdentity.MINE)
			{
				squares[x-1][y].identity = SquareIdentity.NUMBER;
				squares[x-1][y].number++;
			}
			
			if((y-1) >= 0)
			{
				if(squares[x-1][y-1].identity != SquareIdentity.MINE)
				{
					squares[x-1][y-1].identity = SquareIdentity.NUMBER;
					squares[x-1][y-1].number++;
				}
			}
			
			if((y+1) < squares[x].length)
			{
				if(squares[x-1][y+1].identity != SquareIdentity.MINE)
				{
					squares[x-1][y+1].identity = SquareIdentity.NUMBER;
					squares[x-1][y+1].number++;
				}
			}
		}
		
		if((x+1) < squares.length)
		{
			if(squares[x+1][y].identity != SquareIdentity.MINE)
			{
				squares[x+1][y].identity = SquareIdentity.NUMBER;
				squares[x+1][y].number++;
			}
			
			if((y-1) >= 0)
			{
				if(squares[x+1][y-1].identity != SquareIdentity.MINE)
				{
					squares[x+1][y-1].identity = SquareIdentity.NUMBER;
					squares[x+1][y-1].number++;
				}
			}
			
			if((y+1) < squares[x].length)
			{
				if(squares[x+1][y+1].identity != SquareIdentity.MINE)
				{
					squares[x+1][y+1].identity = SquareIdentity.NUMBER;
					squares[x+1][y+1].number++;
				}
			}
		}
		
		if((y - 1) >= 0)
		{
			if(squares[x][y - 1].identity != SquareIdentity.MINE)
			{
				squares[x][y - 1].identity = SquareIdentity.NUMBER;
				squares[x][y - 1].number++;
			}
		}

		if((y + 1) < squares[x].length)
		{
			if(squares[x][y + 1].identity != SquareIdentity.MINE)
			{
				squares[x][y + 1].identity = SquareIdentity.NUMBER;
				squares[x][y + 1].number++;
			}
		}
	}
	
	private void clearSquare(int x, int y)
	{
		SquareDisplayState squareState = squares[x][y].state;
		if(squares[x][y].clicked || squareState == SquareDisplayState.FLAG || gameOver)
		{
			return;
		}
		
		//squares[x][y].button.setEnabled(false);
		squares[x][y].setEmpty();
		squares[x][y].clicked = true;
		
		if(squares[x][y].identity == SquareIdentity.MINE)
		{
			timer.stop();
			gameOver = true;
			squares[x][y].setMine();
			revealMines();
			restartGame("You lost! Start a new game?");
			return;
		}
		
		internalCounter++;
		if(internalCounter == rows*columns-numberOfMines)
		{
			timer.stop();
			gameOver = true;
			counterTextField.setText("0");
			restartGame("You win! Start a new game?");
			return;
		}
		
		if(squares[x][y].identity == SquareIdentity.NUMBER)
		{
			squares[x][y].setNumber();
			return;
		}
		
		if((x-1) >= 0)
		{
			clearSquare(x-1, y);
			
			if((y-1) >= 0)
			{
				clearSquare(x-1, y-1);
			}
			
			if((y+1) < squares[x].length)
			{
				clearSquare(x-1, y+1);
			}
		}
		
		if((x+1) < squares.length)
		{
			clearSquare(x+1, y);
			
			if((y-1) >= 0)
			{
				clearSquare(x+1, y-1);
			}
			
			if((y+1) < squares[x].length)
			{
				clearSquare(x+1, y+1);
			}
		}
		
		if((y-1) >= 0)
		{
			clearSquare(x, y-1);
		}
		
		if((y+1) < squares[x].length)
		{
			clearSquare(x, y+1);
		}
	}
	
	private void revealMines()
	{
		for(int i = 0; i < squares.length; i++)
		{
			for(int j = 0; j < squares[i].length; j++)
			{
				if(squares[i][j].identity == SquareIdentity.MINE)
				{
					squares[i][j].setMine();
				}
			}
		}
	}
	
	private void restartGame(String message)
	{
		Object[] options =
		{
			"Restart", "Exit"
		};
		int selection = JOptionPane.showOptionDialog(frame,
				message,
				"Restart", JOptionPane.YES_NO_OPTION,
				JOptionPane.QUESTION_MESSAGE,
				null,
				options,
				options[0]);
		
		framePos = frame.getLocation();
		frame.dispose();
		
		if(selection == 0)
		{
			createFrame();
		}
	}
	
	@Override
	public void actionPerformed(ActionEvent e)
	{
		if(timer.equals(e.getSource()))
		{
			String time = timerTextField.getText();
			int curTime = Integer.parseInt(time);
			curTime++;
			timerTextField.setText(Integer.toString(curTime));
		}
		if(newGameMenuItem.equals(e.getSource()))
		{
			restartGame("Quit and start a new game?");
		}
		if(optionMenuItem.equals(e.getSource()))
		{
			createOptionFrame();
		}
		if(exitMenuItem.equals(e.getSource()))
		{
			frame.dispose();
		}
	}
	
	@Override
	public void mouseClicked(MouseEvent e)
	{
		if(restartButton.equals(e.getSource()))
		{
			restartGame("Quit and start a new game?");
		}
		else if(optionOkButton.equals(e.getSource()))
		{
			if(beginnerRadio.isSelected())
			{
				rows = 9;
				columns = 9;
				numberOfMines = 10;
			}
			else if(intermediateRadio.isSelected())
			{
				rows = 16;
				columns = 16;
				numberOfMines = 40;
			}
			else if(expertRadio.isSelected())
			{
				rows = 16;
				columns = 36;
				numberOfMines = 99;
			}
			else if(customRadio.isSelected())
			{
				int height, width, numMines;
				
				height = Integer.parseInt(optionHeight.getText());
				width = Integer.parseInt(optionWidth.getText());
				numMines = Integer.parseInt(optionMines.getText());
				
				if(height < 9 || height > 24 || width < 9 || width > 36 || numMines < 10 || numMines > 668)
				{
					JOptionPane.showMessageDialog(frame, "Parameters out of bounds!");
					return;
				}
				else
				{
					rows = height;
					columns = width;
					numberOfMines = numMines;
					
				}
			}
			
			optionFrame.dispose();
			framePos = frame.getLocation();
			frame.dispose();
			createFrame();
		}
		else if(optionCancelButton.equals(e.getSource()))
		{
			optionFrame.dispose();
		}
	}
	
	@Override
	public void mousePressed(MouseEvent e)
	{
		if(e.getButton() == MouseEvent.BUTTON1)
		{
			leftMousePressed = true;
			processMouseEvent(e);
		}
		
		if(e.getButton() == MouseEvent.BUTTON3)
		{
			processMouseEvent(e);
		}
	}
	
	@Override
	public void mouseReleased(MouseEvent e)
	{
		if(e.getButton() == MouseEvent.BUTTON1 && leftMousePressed && !leftMouseExited)
		{
			leftMouseReleased = true;
			processMouseEvent(e);
		}
		leftMousePressed = false;
		leftMouseExited = false;
	}
	
	@Override
	public void mouseExited(MouseEvent e)
	{
		if(leftMousePressed)
		{
			leftMouseExited = true;
			processMouseEvent(e);
		}
	}
	
	private void processMouseEvent(MouseEvent e)
	{
		for(int i = 0; i < squares.length; i++)
		{
			for(int j = 0; j < squares[i].length; j++)
			{
				if(squares[i][j].button.equals(e.getSource()))
				{
					//0 is left, 1 is middle, 2 is right
					if(leftMousePressed && (squares[i][j].state != SquareDisplayState.FLAG
							&& !squares[i][j].clicked))
					{
						squares[i][j].setEmpty();
						
						if(leftMouseExited)
						{
							squares[i][j].setNew();
						}
						if(leftMouseReleased)
						{
							leftMouseReleased = false;

							if(!gameStarted)
							{
								//start timer
								timerTextField.setText("1");
								timer.start();
								placeMines(i, j);
								gameStarted = true;
								gameOver = false;
							}
							clearSquare(i, j);
						}
					}
					if(e.getButton() == MouseEvent.BUTTON3)
					{
						if(leftMousePressed)
						{
							leftMousePressed = false;
							squares[i][j].setNew();
						}
						SquareDisplayState squareState = squares[i][j].state;
						switch(squareState)
						{
							case NEW:
								squares[i][j].setFlag();
								counter--;
								counterTextField.setText(Integer.toString(counter));
								break;
							case FLAG:
								squares[i][j].setNew();
								counter++;
								counterTextField.setText(Integer.toString(counter));
								break;
							//case QUESTION:
							//	squares[i][j].setNew();
							//	break;
						}
					}

					break;
				}
			}
		}
	}
}