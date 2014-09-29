#include <iostream>
#include "animation.h"

Animation::Animation()
{
	numberOfFrames = 0;
	lastFrameApplied = 0;
}

Animation::~Animation()
{
	for( int x=0; x < numberOfFrames; x++ )
	{
		SDL_FreeSurface( frames[x] );
		frames[x] = NULL;
		std::cout << "Freed frame " << x << std::endl;
	}
}

void Animation::addImage( std::string filename )
{
	SDL_Surface* surface = NULL;
	surface = load_surface( filename );
	colorkey_image( 255, 255, 255, surface );

	frames.push_back( surface );
	numberOfFrames++;
}

void Animation::apply_frame( SDL_Surface* screen, int index)
{
	apply_surface( screen, frames[index] );
	lastFrameApplied = index;
}

void Animation::apply_frames( SDL_Surface* screen )
{
	for( int x=0; x < numberOfFrames; x++ )
	{
		apply_frame( screen, x );
	}
}

void Animation::colorkey_frame( int red, int green, int blue, int index )
{
	colorkey_image( red, green, blue, frames[index] );
}


int Animation::getNumberOfFrames()
{
	return numberOfFrames;
}

int Animation::getLastFrameApplied()
{
	return lastFrameApplied;
}