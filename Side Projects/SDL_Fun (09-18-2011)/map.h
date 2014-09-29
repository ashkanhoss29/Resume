#ifndef MAP_H
#define MAP_H

#include <SDL.h>
#include "sprite.h"
#include <vector>


/**Holds map information, particularly all the objects/sprites for the map (rooms, tables, etc)**/
class Map
{
public:
	Map();
	~Map();

	/**Add a sprite to our collection of sprites**/
	void addSprite( Sprite* sprite );
	void addSprite( Sprite* sprite, int mapPositionX, int mapPositionY );
	void addSprite( std::string filename );
	void addSprite( std::string filename, int mapPositionX, int mapPositionY );
	void addSprite( std::string filename, int mapPositionX, int mapPositionY, bool collidable );
	
	/**Run Map**/
	void run_map();
	void apply_images( SDL_Surface* screen );

	/**Check for collisions**/
	int checkCollision( Sprite* collidingSprite ); //return val is index of collision. -1 is no collision.
	bool checkCollision( Sprite* collidingSprite, int index );
	bool checkCollision( int index1, int index2 );


	/**Set Methods**/
	void setPosition( int positionX, int positionY );
	void setPositionX( int positionX );
	void setPositionY( int positionY );
	void setSpeed( int speed );
	void setSpriteMapPosition( int index, int x, int y );
	
	/**Get Methods**/
	Sprite* getSprite( int index );
	int getPositionX();
	int getPositionY();
	int getSpeed();

private:
	std::vector< Sprite* > sprites;
	int numberOfSprites;
	int positionX;
	int positionY;
	int speed;

};

#endif