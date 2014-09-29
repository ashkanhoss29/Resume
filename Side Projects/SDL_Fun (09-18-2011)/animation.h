#ifndef ANIMATION_H
#define ANIMATION_H

#include "sprite.h"
#include <vector>

class Animation: public Sprite
{
public:
	Animation();
	~Animation();

	void addImage( std::string filename ); //add a single frame
	void apply_frame( SDL_Surface* screen, int index );
	void apply_frames( SDL_Surface* screen );
	void colorkey_frame( int red, int green, int blue, int index );

	int getNumberOfFrames();
	int getLastFrameApplied();

private:
	std::vector< SDL_Surface* > frames; //frames of animation
	int numberOfFrames;
	int lastFrameApplied;
};

#endif