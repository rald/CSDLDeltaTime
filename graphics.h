#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

SDL_Surface *Graphics_LoadImage( char *filename );
void Graphics_ApplySurface( int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip );

#endif
