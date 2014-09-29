#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Sprite
{
public:
	Sprite();
	~Sprite();


	SDL_Surface* load_surface( std::string filename );
	void load_image( std::string filename );
	void colorkey_image( int red, int green, int blue, SDL_Surface* image );
	void colorkey_image( int red, int green, int blue );
	void apply_surface( SDL_Surface* screen, SDL_Surface* surface );
	void apply_surface( SDL_Surface* screen );
	bool checkCollision( Sprite* collidingSprite );


	/**Set functions**/
	void setImage( SDL_Surface* image );
	void setScreenPositionX( int screenPositionX );
	void setScreenPositionY( int screenPositionY );
	void setScreenCenterPositionX( int centerPositionX );
	void setScreenCenterPositionY( int centerPositionY );
	void setMapPositionX( int mapPositionX );
	void setMapPositionY( int mapPositionY );
	void setMapCenterPositionX( int centerPositionX );
	void setMapCenterPositionY( int centerPositionY );
	void setSpeed( int speed );
	void setCollidable( bool collidable );
	void setCollisionBoxPercent( float percent );

	/**Get functions**/
	SDL_Surface* getImage();
	int getHeight();
	int getWidth();
	int getScreenPositionX();
	int getScreenPositionY();
	int getScreenCenterPositionX();
	int getScreenCenterPositionY();

	int getRelativeHeight();
	int getRelativeWidth();
	int getMapPositionX();
	int getMapPositionY();
	int getMapCenterPositionX();
	int getMapCenterPositionY();

	int getSpeed();
	bool isCollidable();
	float getCollisionBoxPercent();
	int getCollisionBoxX();
	int getCollisionBoxY();
	int getCollisionBoxWidth();
	int getCollisionBoxHeight();


private:
	SDL_Surface *image;
	std::string imageName;
	int height;
	int width;
	int screenPositionX;
	int screenPositionY;
	int screenCenterPositionX;
	int screenCenterPositionY;

	int relativeHeight;
	int relativeWidth;
	int mapPositionX;
	int mapPositionY;
	int mapCenterPositionX;
	int mapCenterPositionY;

	int speed;

	bool collidable;
	float collisionBoxPercent;
	int collisionBoxWidthDiff;
	int collisionBoxHeightDiff;
	int collisionBoxX;
	int collisionBoxY;
	int collisionBoxWidth;
	int collisionBoxHeight;
};

#endif