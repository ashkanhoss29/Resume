#include <iostream>
#include <string> //Used for storing image name/location
#include <sstream>
#include "SDL.h" //Graphics, Audio, Input, etc
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "sprite.h"
#include "map.h"
#include "animation.h"
#include "Exploration.h"


/***Globals***/
/*SDL Screen attributes*/
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;
const int SCREEN_BPP = 32; //Bits per pixel

/**Surfaces are images to display or screens to display images in**/
SDL_Surface* screen = NULL;
TTF_Font* font = NULL;
//SDL_Surface* smiley = NULL;


/***Prototypes***/
bool init_sdl();
void clean_up();
SDL_Surface* load_image( std::string filename );
void colorkey( SDL_Surface* image );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );


/*Main*/
int main( int argc, char* args[] )
{
	bool quit = false;

	//SDL_Color textColor = { 255, 255, 255 };
	//Uint32 startTime = 0;
	//bool runningTime = true;
	//SDL_Surface* displayTime = NULL;


	//SDL_Event event;

	//Init SDL and create screen
	if( init_sdl() == false )
	{
		return 1;
	}

	/*
	Animation* player = new Animation();
	player->addImage( "character.jpg" );
	player->colorkey_frame( 255, 255, 255, player->getNumberOfFrames()-1 );
	player->addImage( "character2.jpg" );
	player->colorkey_frame( 255, 255, 255, player->getNumberOfFrames()-1 );
	player->setCollisionBoxPercent( .3 );
	player->setScreenCenterPositionX( (int)SCREEN_WIDTH/2 );
	player->setScreenCenterPositionY( (int)SCREEN_HEIGHT/2 );
	player->setCollidable( true );
	

	Map map;
	map.setPosition( 0, 0 );
	map.setSpeed( 3 );
	map.addSprite( "background2.jpg", 700, 400 );
	int xPos = 0;
	int yPos = 0;
	for( int x=0; x < 3; x++ )
	{
		for ( int y=0; y < 2; y++ )
		{
			map.addSprite( "beholder2.png", xPos, yPos, true );
			std::cout << "Created sprite at: " << xPos << ", " << yPos << std::endl;
			yPos = yPos + 700;
		}
		xPos = xPos + 700;
		yPos = 0;
	}
	//map.addSprite( "beholder2.png", 0, 0, true );
	//map.addSprite( "beholder2.png", 0, 600, true );
	//map.addSprite( "beholder2.png", 600, 0, true );
	//map.addSprite( "beholder2.png", 600, 600, true );
	map.setSpriteMapPosition( 0, 0, 0 );
	*/

	/*
	bool keysHeld[323] = {false};

	bool colliding = false;
	int collidingMapSprite;
	int collisionVelocity = 2;

	int frameCounter = 0;
	int frameToUse = 0;

	startTime = SDL_GetTicks();
	*/

	Exploration* exploration = new Exploration();
	exploration->init( SCREEN_WIDTH, SCREEN_HEIGHT, screen );

	/***Main Loop***/
	while( quit == false )
	{
		if( !exploration->run() )
		{
			quit = true;
		}

		/****Check for collision****/
		/*
		if( (collidingMapSprite = map.checkCollision( player )) != -1 )
		{
			colliding = true;

			int xDifference = map.getSprite( collidingMapSprite )->getScreenCenterPositionX() - player->getScreenCenterPositionX();
			int yDifference = map.getSprite( collidingMapSprite )->getScreenCenterPositionY() - player->getScreenCenterPositionY();


			if( xDifference < 0 )
				map.setPositionX( map.getPositionX() + (xDifference/xDifference * -collisionVelocity) );
			else if( xDifference > 0 )
				map.setPositionX( map.getPositionX() + (xDifference/xDifference) * collisionVelocity );

			if( yDifference < 0 )
				map.setPositionY( map.getPositionY() + (yDifference/yDifference * -collisionVelocity) );
			else if( yDifference > 0 )
				map.setPositionY( map.getPositionY() + (yDifference/yDifference) * collisionVelocity );
		}
		else
		{
			colliding = false;
		}

		/**Loop until all events in event queue have been processed**
		while( SDL_PollEvent( &event ) )
		{
			//If the user clicks on X
			if( event.type == SDL_QUIT )
			{
				//Setting this will exit the main loop
				quit = true;
			}
			else if( event.type == SDL_KEYDOWN )
			{
				if( event.key.keysym.sym == SDLK_s )
				{
					if( runningTime )
					{
						//Stop timer
						runningTime = false;
						startTime = 0;
					}
					else
					{
						//Start timer
						runningTime = true;
						startTime = SDL_GetTicks();
					}
				}

				keysHeld[event.key.keysym.sym] = true;
			}
			else if( event.type == SDL_KEYUP )
			{
				keysHeld[event.key.keysym.sym] = false;
			}

		}
		
		/**Do the following when the respective key is pressed down**
		if( keysHeld[SDLK_ESCAPE] )
		{
			//TODO: Bring up or close Pause menu
		}
		if( keysHeld[SDLK_LEFT] )
		{
			map.setPositionX( map.getPositionX() + map.getSpeed() );
			//background->setScreenPositionX( background->getScreenPositionX() - background->getSpeed() );
		}
		if( keysHeld[SDLK_RIGHT] )
		{
			map.setPositionX( map.getPositionX() - map.getSpeed() );
			//background->setScreenPositionX( background->getScreenPositionX() + background->getSpeed() );
		}
		if( keysHeld[SDLK_UP] )
		{
			map.setPositionY( map.getPositionY() + map.getSpeed() );
			//background->setScreenPositionY( background->getScreenPositionY() - background->getSpeed() );
		}
		if( keysHeld[ SDLK_DOWN] )
		{
			map.setPositionY( map.getPositionY() - map.getSpeed() );
			//background->setScreenPositionY( background->getScreenPositionY() + background->getSpeed() );
		}


		/****Draw Sprites****
		SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		map.apply_images( screen );
		if( !colliding )
		{
			
		}
		//beholder->apply_surface( screen );
		//staticBeholder->apply_surface( screen );

		frameCounter++;
		if( frameCounter == 30 )
		{
			frameCounter = 0;
			if( frameToUse == 0 )
			{
				frameToUse = 1;
			}
			else
			{
				frameToUse = 0;
			}
		}

		player->apply_frame( screen, frameToUse );


		if( runningTime )
		{
			std::stringstream time;
			time << "Timer: " << SDL_GetTicks() - startTime;
			displayTime = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
			apply_surface( 50, 50, displayTime, screen );
			SDL_FreeSurface( displayTime );
		}
		*/

		/****Flip buffer?****/
		if( SDL_Flip( screen ) == -1 )
		{
			return 1;
		}

	}


	/**Out of Main Loop means game is shutting down. Clean everything up**/
	clean_up();
	return 0;
}


/***Function Definitions***/

/*Initialize SDL subsystems and an SDL screen*/
bool init_sdl()
{
	//Initialize all SDL subsystems (graphics, sound, etc.)
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false; //If SDL_Init() returns -1 we have an error, so exit
	}

	//Set up a screen using software memory and test for correct init
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
	if( screen == NULL )
	{
		return false; //Exit if we failed to init screen properly
	}

	if( TTF_Init() == -1 )
	{
		std::cout << "TTF_Init() error" << std::endl;
		return false;
	}
	else
	{
		font = TTF_OpenFont( "Benegraphic.ttf", 28 );
		if( font == NULL)
		{
			std::cout << "TTF_OpenFont is NULL" << std::endl;
			return false;
		}
	}

	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	int init = IMG_Init( flags );
	if( (init & flags) != flags )
	{
		return false;
	}

	SDL_WM_SetCaption( "Event test", NULL ); //Set window caption

	return true;
}


void clean_up()
{
	//Quite SDL
	SDL_Quit();
}


/*Load and optimize an image*/
//string filename - Name of the image file to load
SDL_Surface* load_image( std::string filename )
{
	SDL_Surface* loadedImage = NULL; //Temporary storage of the pointer to image
	SDL_Surface* optimizedImage = NULL; //The optimized image that will be used

	loadedImage = IMG_Load( filename.c_str() ); //Load the image

	//Make sure nothing went wrong in loading the image
	if( loadedImage != NULL )
	{
		//Convert 24bit bmp to 32bit now, instead of doing it during runtime
		optimizedImage = SDL_DisplayFormat( loadedImage ); //Create an optimized image

		SDL_FreeSurface( loadedImage ); //Free the old image
	}

	return optimizedImage;
}


void colorkey( SDL_Surface* image )
{
	if( image != NULL )
	{
		//Map color key
		Uint32 colorkey = SDL_MapRGB( image->format, 159, 43, 43 );

		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( image, SDL_SRCCOLORKEY, colorkey );
	}
}


/*Blit source to destination*/
//x and y - coordinates for source surface
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	SDL_Rect offset; //stores x and y since SDL_BlitSurface() only excepts SDL_Rect

	offset.x = x; //assign x to offset
	offset.y = y; //assign y to offset

	SDL_BlitSurface( source, NULL, destination, &offset );
}