#include "Exploration.h"
#include <iostream>

Exploration::Exploration()
{
	screen = NULL;
	//SDL_Color textColor = { 255, 255, 255 };
	//this->textColor = textColor;
	font = NULL;
	startTime = 0;
	runningTime = true;
	displayTime = NULL;

	player = new Animation();
	map = new Map();
}

Exploration::~Exploration()
{
}

bool Exploration::init( int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Surface* screen )
{
	if( screen != NULL )
	{
		this->screen = screen;
	}
	else
	{
		return false;
	}

	this->font = TTF_OpenFont( "Benegraphic.ttf", 28 );

	player = new Animation();
	player->addImage( "character.jpg" );
	player->colorkey_frame( 255, 255, 255, player->getNumberOfFrames()-1 );
	player->addImage( "character2.jpg" );
	player->colorkey_frame( 255, 255, 255, player->getNumberOfFrames()-1 );
	player->setCollisionBoxPercent( (float)0.3 );
	player->setScreenCenterPositionX( (int)SCREEN_WIDTH/2 );
	player->setScreenCenterPositionY( (int)SCREEN_HEIGHT/2 );
	player->setCollidable( true );
	

	map->setPosition( 0, 0 );
	map->setSpeed( 3 );
	map->addSprite( "background2.jpg", 700, 400 );
	int xPos = 0;
	int yPos = 0;
	for( int x=0; x < 3; x++ )
	{
		for ( int y=0; y < 2; y++ )
		{
			map->addSprite( "beholder2.png", xPos, yPos, true );
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
	map->setSpriteMapPosition( 0, 0, 0 );


	bool keysHeld[323] = {false};

	colliding = false;
	collidingMapSprite;
	collisionVelocity = 2;

	frameCounter = 0;
	frameToUse = 0;

	startTime = SDL_GetTicks();

	return true;
}

bool Exploration::run()
{
	if( !processKeyInput() )
	{
		return false;
	}

	/****Draw Sprites****/
	SDL_FillRect( screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	map->apply_images( screen );

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
		SDL_Color textColor = { 255, 255, 255 };
		std::stringstream time;
		time << "Timer: " << SDL_GetTicks() - startTime;
		displayTime = TTF_RenderText_Solid( font, time.str().c_str(), textColor );
		apply_surface( 50, 50, displayTime, screen );
		SDL_FreeSurface( displayTime );
	}

	return true;
}


bool Exploration::processKeyInput()
{
	/****Check for collision****/
	if( (collidingMapSprite = map->checkCollision( player )) != -1 )
	{
		colliding = true;

		int xDifference = map->getSprite( collidingMapSprite )->getScreenCenterPositionX() - player->getScreenCenterPositionX();
		int yDifference = map->getSprite( collidingMapSprite )->getScreenCenterPositionY() - player->getScreenCenterPositionY();


		if( xDifference < 0 )
			map->setPositionX( map->getPositionX() + (xDifference/xDifference * -collisionVelocity) );
		else if( xDifference > 0 )
			map->setPositionX( map->getPositionX() + (xDifference/xDifference) * collisionVelocity );

		if( yDifference < 0 )
			map->setPositionY( map->getPositionY() + (yDifference/yDifference * -collisionVelocity) );
		else if( yDifference > 0 )
			map->setPositionY( map->getPositionY() + (yDifference/yDifference) * collisionVelocity );
	}
	else
	{
		colliding = false;
	}

	/**Loop until all events in event queue have been processed**/
	while( SDL_PollEvent( &event ) )
	{
		//If the user clicks on X
		if( event.type == SDL_QUIT )
		{
			//Setting this will exit the main loop
			return false;
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

	/**Do the following when the respective key is pressed down**/
	if( keysHeld[SDLK_ESCAPE] )
	{
		//TODO: Bring up or close Pause menu
	}
	if( keysHeld[SDLK_LEFT] )
	{
		map->setPositionX( map->getPositionX() + map->getSpeed() );
		//background->setScreenPositionX( background->getScreenPositionX() - background->getSpeed() );
	}
	if( keysHeld[SDLK_RIGHT] )
	{
		map->setPositionX( map->getPositionX() - map->getSpeed() );
		//background->setScreenPositionX( background->getScreenPositionX() + background->getSpeed() );
	}
	if( keysHeld[SDLK_UP] )
	{
		map->setPositionY( map->getPositionY() + map->getSpeed() );
		//background->setScreenPositionY( background->getScreenPositionY() - background->getSpeed() );
	}
	if( keysHeld[ SDLK_DOWN] )
	{
		map->setPositionY( map->getPositionY() - map->getSpeed() );
		//background->setScreenPositionY( background->getScreenPositionY() + background->getSpeed() );
	}

	return true;
}


void Exploration::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	SDL_Rect offset; //stores x and y since SDL_BlitSurface() only excepts SDL_Rect

	offset.x = x; //assign x to offset
	offset.y = y; //assign y to offset

	SDL_BlitSurface( source, NULL, destination, &offset );
}