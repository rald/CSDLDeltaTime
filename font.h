#ifndef FONT_H
#define FONT_H

#include <SDL/SDL.h>

#include <stdarg.h>
#include <string.h>

#include "common.h"
#include "graphics.h"



typedef struct Font Font;

struct Font {
	int w,h;
	SDL_Surface *image;
};



Font *Font_New(SDL_Surface *image,int w,int h);
void Font_Print(SDL_Surface *surface,Font *font,int x,int y,char *fmt,...);

#endif
