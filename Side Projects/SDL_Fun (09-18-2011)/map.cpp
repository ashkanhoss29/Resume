#include "map.h"
#include <iostream>

/***Constructors***/
Map::Map()
{
	numberOfSprites = 0;
	positionX = 0;
	positionY = 0;
}

Map::~Map()
{
}


/***Add sprite functions. Will push back sprite to sprite vector***/
void Map::addSprite( Sprite* sprite )
{
	sprites.push_back( sprite );
	numberOfSprites++;
}

void Map::addSprite( Sprite* sprite, int mapPositionX, int mapPositionY )
{
	sprite->setMapPositionX(mapPositionX);
	sprite->setMapPositionY(mapPositionY);

	addSprite( sprite );
}

void Map::addSprite( std::string filename )
{
	Sprite* sprite = new Sprite();
	sprite->load_image( filename );
	sprite->colorkey_image( 0, 0, 0 );
	sprite->setCollisionBoxPercent( .3 );

	sprites.push_back( sprite );
	numberOfSprites++;
}

void Map::addSprite( std::string filename, int mapCenterPositionX, int mapCenterPositionY )
{
	addSprite( filename );

	sprites[numberOfSprites-1]->setMapCenterPositionX( mapCenterPositionX );
	sprites[numberOfSprites-1]->setMapCenterPositionY( mapCenterPositionY );

	sprites[numberOfSprites-1]->setScreenPositionX( this->positionX + sprites[numberOfSprites-1]->getMapPositionX() );
	sprites[numberOfSprites-1]->setScreenPositionY( this->positionY + sprites[numberOfSprites-1]->getMapPositionY() );
}

void Map::addSprite( std::string filename, int mapPositionX, int mapPositionY, bool collidable )
{
	addSprite( filename, mapPositionX, mapPositionY );

	sprites[numberOfSprites-1]->setCollidable( collidable );
}
/**Ends addSprite Functions**/


/***Paint/blit all images in sprites vector***/
void Map::apply_images( SDL_Surface* screen )
{
	for( int x=0; x < numberOfSprites; x++ )
	{
		sprites[x]->apply_surface( screen );
	}
}


//Check collision between collidingSprite and any (the first) sprite in map
int Map::checkCollision( Sprite* collidingSprite )
{
	for( int x=0; x < numberOfSprites; x++ )
	{
		if( sprites[x]->checkCollision( collidingSprite ) )
		{
			return x;
		}
	}

	return -1;
}

//Check collision between collidingSprite and a specific sprite in map
bool Map::checkCollision( Sprite* collidingSprite, int index )
{
	return collidingSprite->checkCollision( sprites[index] );
}

//Check collision between two sprites in map
bool Map::checkCollision( int index1, int index2 )
{
	return sprites[index1]->checkCollision( sprites[index2] );
}


/***Set position functions***/
void Map::setPosition( int positionX, int positionY )
{
	this->positionX = positionX;
	this->positionY = positionY;

	for( int x=0; x < numberOfSprites; x++ )
	{
		sprites[x]->setScreenPositionX( sprites[x]->getMapPositionX() + positionX );
		sprites[x]->setScreenPositionY( sprites[x]->getMapPositionY() + positionY );
	}
}

void Map::setPositionX( int positionX )
{
	this->positionX = positionX;

	for( int x=0; x < numberOfSprites; x++ )
	{
		sprites[x]->setScreenPositionX( sprites[x]->getMapPositionX() + positionX );
	}
}

void Map::setPositionY( int positionY )
{
	this->positionY = positionY;

	for( int x=0; x < numberOfSprites; x++ )
	{
		sprites[x]->setScreenPositionY( sprites[x]->getMapPositionY() + positionY );
	}
}
/**End setPosition functions**/


/***Set speed***/
void Map::setSpeed( int speed )
{
	this->speed = speed;
}


/***Move sprite relative to the map***/
void Map::setSpriteMapPosition( int index, int x, int y )
{
	sprites[index]->setMapPositionX( x );
	sprites[index]->setMapPositionY( y );

	sprites[index]->setScreenPositionX( this->positionX + x );
	sprites[index]->setScreenPositionY( this->positionY + y );
}



/***Get Functions***/
Sprite* Map::getSprite(int index)
{
	return sprites[index];
}

int Map::getPositionX()
{
	return positionX;
}

int Map::getPositionY()
{
	return positionY;
}

int Map::getSpeed()
{
	return speed;
}
/**End get functions**/