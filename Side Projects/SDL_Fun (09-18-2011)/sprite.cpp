#include "sprite.h"
#include <iostream>

Sprite::Sprite()
{
	image = NULL;
	height = 0;
	width = 0;
	screenPositionX = 0;
	screenPositionY = 0;
	screenCenterPositionX = 0;
	screenCenterPositionY = 0;
	mapPositionX = 0;
	mapPositionY = 0;
	mapCenterPositionX = 0;
	mapCenterPositionY = 0;
	speed = 0;
	collidable = false;
	collisionBoxPercent = 0;
}

Sprite::~Sprite()
{
	SDL_FreeSurface( image );
	std::cout << "Freed surface " << imageName << std::endl;
}



/**Load Image**/
SDL_Surface* Sprite::load_surface( std::string filename )
{
	SDL_Surface* loaded_image = NULL; //Temporary storage of the pointer to image
	//SDL_Surface* optimizedImage = NULL; //The optimized image that will be used

	loaded_image = IMG_Load( filename.c_str() ); //Load the image

	//Make sure nothing went wrong in loading the image
	if( loaded_image != NULL )
	{
		//Convert 24bit bmp to 32bit now, instead of doing it during runtime
		loaded_image = SDL_DisplayFormat( loaded_image ); //Create an optimized image

		height = loaded_image->h;
		width = loaded_image->w;
	}

	imageName = filename;

	return loaded_image;
}

/**Load this->image**/
void Sprite::load_image( std::string filename )
{
	std::cout << "calling load_image" << std::endl;
	this->image = load_surface( filename );
}



/**Colorkey**/
void Sprite::colorkey_image( int red, int green, int blue, SDL_Surface* surface )
{
	if( surface != NULL )
	{
		//Map color key
		Uint32 colorkey = SDL_MapRGB( surface->format, red, green, blue );

		//Set all pixels of color R 0, G 0xFF, B 0xFF to be transparent
		SDL_SetColorKey( surface, SDL_SRCCOLORKEY, colorkey );
	}
}

/**colorkey this->image**/
void Sprite::colorkey_image( int red, int green, int blue )
{
	colorkey_image( red, green, blue, this->image );
}



/**Apply Surface**/
void Sprite::apply_surface( SDL_Surface* screen, SDL_Surface* image )
{
	if( image != NULL)
	{
		SDL_Rect offset; //stores x and y since SDL_BlitSurface() only excepts SDL_Rect

		offset.x = screenPositionX; //assign x to offset
		offset.y = screenPositionY; //assign y to offset
		
		SDL_BlitSurface( image, NULL, screen, &offset );
		//std::cout << "applied surface" << std::endl;
	}
}

/**Apply this->image**/
void Sprite::apply_surface( SDL_Surface* screen )
{
	apply_surface( screen, this->image );
}



/**Collision Detection**/
bool Sprite::checkCollision( Sprite* collidingSprite )
{
	if( collidable )
	{
		//If top left corner of collidingSprite is inside this sprite return true:
		if( collidingSprite->getCollisionBoxX() > this->getCollisionBoxX() && 
			collidingSprite->getCollisionBoxY() > this->getCollisionBoxY() )
		{
			if( collidingSprite->getCollisionBoxX() < this->getCollisionBoxX()+this->getCollisionBoxWidth() &&
				collidingSprite->getCollisionBoxY() < this->getCollisionBoxY()+this->getCollisionBoxHeight() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//If top right corner of collidingSprite is inside this sprite return true:
		else if( collidingSprite->getCollisionBoxX()+collidingSprite->getCollisionBoxWidth() > this->getCollisionBoxX() && 
			collidingSprite->getCollisionBoxY() > this->getCollisionBoxY() )
		{
			if( collidingSprite->getCollisionBoxX()+collidingSprite->getCollisionBoxWidth() < this->getCollisionBoxX()+this->getCollisionBoxWidth() &&
				collidingSprite->getCollisionBoxY() < this->getCollisionBoxY()+this->getCollisionBoxHeight() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//If bottom left corner of collidingSprite is inside this sprite return true:
		else if( collidingSprite->getCollisionBoxX() > this->getCollisionBoxX() && 
			collidingSprite->getCollisionBoxY()+collidingSprite->getCollisionBoxHeight() > this->getCollisionBoxY() )
		{
			if( collidingSprite->getCollisionBoxX() < this->getCollisionBoxX()+this->getCollisionBoxWidth() &&
				collidingSprite->getCollisionBoxY()+collidingSprite->getCollisionBoxHeight() < this->getCollisionBoxY()+this->getCollisionBoxHeight() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		//If bottom right corner of colliding sprite is inside this sprite return true:
		else if( collidingSprite->getCollisionBoxX()+collidingSprite->getCollisionBoxWidth() > this->getCollisionBoxX() &&
			collidingSprite->getCollisionBoxY()+collidingSprite->getCollisionBoxHeight() > this->getCollisionBoxY() )
		{
			if( collidingSprite->getCollisionBoxX()+collidingSprite->getCollisionBoxWidth() < this->getCollisionBoxX()+this->getCollisionBoxWidth() &&
				collidingSprite->getCollisionBoxY()+collidingSprite->getCollisionBoxHeight() < this->getCollisionBoxY()+this->getCollisionBoxHeight() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}



/*****Set functions*****/

/**Screen Position**/
void Sprite::setScreenPositionX( int screenPositionX )
{
	this->screenPositionX = screenPositionX;

	screenCenterPositionX = (int)((width / 2) + screenPositionX);

	collisionBoxX = screenPositionX + collisionBoxWidthDiff;
	collisionBoxWidth = width - collisionBoxWidthDiff;
}

void Sprite::setScreenPositionY( int screenPositionY )
{
	this->screenPositionY = screenPositionY;

	screenCenterPositionY = (int)((height / 2) + screenPositionY);

	collisionBoxY = screenPositionX + collisionBoxHeightDiff;
	collisionBoxHeight = height - collisionBoxHeightDiff;
}

void Sprite::setScreenCenterPositionX( int centerPositionX )
{
	screenCenterPositionX = centerPositionX;

	screenPositionX = (int)(centerPositionX - (width / 2));
}

void Sprite::setScreenCenterPositionY( int centerPositionY )
{
	screenCenterPositionY = centerPositionY;

	screenPositionY = (int)(centerPositionY - (width / 2));
}

/**Map Position**/
void Sprite::setMapPositionX( int mapPositionX )
{
	this->mapPositionX = mapPositionX;

	mapCenterPositionX = (int)((width / 2) + mapPositionX);
}

void Sprite::setMapPositionY( int mapPositionY )
{
	this->mapPositionY = mapPositionY;

	mapCenterPositionY = (int)((height / 2) + mapPositionY);
}

void Sprite::setMapCenterPositionX( int centerPositionX )
{
	mapCenterPositionX = centerPositionX;

	mapPositionX = (int)(centerPositionX - (width / 2));
}

void Sprite::setMapCenterPositionY( int centerPositionY )
{
	mapCenterPositionY = centerPositionY;

	mapPositionY = (int)(centerPositionY - (height / 2));
}

/**Speed**/
void Sprite::setSpeed( int speed )
{
	this->speed = speed;
}

/**Collidable**/
void Sprite::setCollidable( bool collidable )
{
	this->collidable = collidable;
}

void Sprite::setCollisionBoxPercent( float percent )
{
	collisionBoxWidthDiff = (int)(width * percent);
	collisionBoxHeightDiff = (int)(height * percent);
	collisionBoxPercent = percent;
}



/*****Get functions*****/

SDL_Surface* Sprite::getImage()
{
	return image;
}

int Sprite::getHeight()
{
	return height;
}

int Sprite::getWidth()
{
	return width;
}

/**Screen Position**/
int Sprite::getScreenPositionX()
{
	return screenPositionX;
}

int Sprite::getScreenPositionY()
{
	return screenPositionY;
}

int Sprite::getScreenCenterPositionX()
{
	return screenCenterPositionX;
}

int Sprite::getScreenCenterPositionY()
{
	return screenCenterPositionY;
}


/**Relative Height/Width**/
int Sprite::getRelativeHeight()
{
	return relativeHeight;
}

int Sprite::getRelativeWidth()
{
	return relativeWidth;
}

/**Map Position**/
int Sprite::getMapPositionX()
{
	return mapPositionX;
}

int Sprite::getMapPositionY()
{
	return mapPositionY;
}

int Sprite::getMapCenterPositionX()
{
	return mapCenterPositionX;
}

int Sprite::getMapCenterPositionY()
{
	return mapCenterPositionY;
}

/**Speed**/
int Sprite::getSpeed()
{
	return speed;
}

/**Collidable**/
bool Sprite::isCollidable()
{
	return collidable;
}

float Sprite::getCollisionBoxPercent()
{
	return collisionBoxPercent;
}

int Sprite::getCollisionBoxX()
{
	//std::cout << imageName << " - screenPositionX: " << screenPositionX << "  ";
	//std::cout << "getCollisionBoxX: " << screenPositionX + collisionBoxWidthDiff << std::endl;
	//return (int)(screenPositionX + ( screenPositionX * 0.3 ));
	return screenPositionX + collisionBoxWidthDiff;
}

int Sprite::getCollisionBoxY()
{
	//std::cout <<  imageName << " - screenPositionY: " << screenPositionY << "  ";
	//std::cout << "getCollisionY: " << screenPositionY + collisionBoxHeightDiff << std::endl;
	//return (int)(screenPositionY + ( screenPositionY * 0.3 ));
	return screenPositionY + collisionBoxHeightDiff;
}

int Sprite::getCollisionBoxWidth()
{
	//std::cout <<  imageName << " - width: " << width + screenPositionX << "  ";
	//std::cout << "getCollisionBoxWidth(): " << (width + screenPositionX) - collisionBoxWidthDiff << std::endl;
	//return (int)(( width + screenPositionX ) - ( ( width + screenPositionX ) * ( 0.3 ) ));
	return width - collisionBoxWidthDiff;
}

int Sprite::getCollisionBoxHeight()
{
	//std::cout <<  imageName << " - height: " << height + screenPositionY << "  ";
	//std::cout << "getCollisionBoxHeight: " << (height + screenPositionY) - collisionBoxHeightDiff << std::endl;
	//return (int)(( height + screenPositionY ) - ( ( height + screenPositionY ) * ( 0.3 ) ));
	return height - collisionBoxHeightDiff;
}