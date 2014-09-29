#ifndef EXPLORATION_H
#define EXPLORATION_H

#include <string> //Used for storing image name/location
#include <sstream>
#include "SDL.h" //Graphics, Audio, Input, etc
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "map.h"
#include "animation.h"

//This class will hold and run the exploration view
class Exploration
{
public:
	Exploration();
	~Exploration();

	bool init( int SCREEN_WIDTH, int SCREEN_HEIGHT, SDL_Surface* screen );
	bool run();

private:
	bool processKeyInput();
	void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );

	SDL_Event event;
	SDL_Surface* screen;

	//SDL_Color textColor;
	TTF_Font* font;
	Uint32 startTime;
	bool runningTime;
	SDL_Surface* displayTime;

	Animation* player;
	Map* map;

	bool keysHeld[323];
	bool colliding;
	int collidingMapSprite;
	int collisionVelocity;

	int frameCounter;
	int frameToUse;

};

#endif