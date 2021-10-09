#ifndef SPRITE_H
#define SPRITE_H

#include <stdbool.h>

#include "graphics.h"



typedef struct Sprite Sprite;

struct Sprite {
	int x,y;
	int w,h;
	int vx,vy;
	int numFrames;
	int currentFrame;
	SDL_Surface *image;
	SDL_Rect *clips;
	bool animate;
};

Sprite *Sprite_New(SDL_Surface *image,int x,int y,int w,int h,int vx,int vy,bool animate);
void Sprite_Draw(SDL_Surface *surface,Sprite *sprite);
void Sprite_Update(Sprite *sprite);

#endif
