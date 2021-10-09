#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "graphics.h"
#include "sprite.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32


bool quit = false;
SDL_Surface *screen;
SDL_Event event;

int main(int argc,char **argv) {

	srand(time(NULL));

  //Initialize all SDL subsystems
  if( SDL_Init( SDL_INIT_VIDEO ) == -1 ) {
		return false;
  }

  //Set up the screen
  screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

  //If there was an error in setting up the screen
  if( screen == NULL ) {
		return false;
  }

	SDL_Surface *fontSpriteSheet=Graphics_LoadImage("font1.png");

	int fontWidth=8;
	int fontHeight=8;

	SDL_Surface *font = SDL_CreateRGBSurface(
			SDL_SWSURFACE | SDL_SRCCOLORKEY | SDL_SRCALPHA,
			fontWidth*256, fontHeight,
			screen->format->BitsPerPixel,
			screen->format->Rmask,
			screen->format->Gmask,
			screen->format->Bmask,
			screen->format->Amask);

	SDL_Rect clips[256];

	for(int j=0;j<16;j++) {
		for(int i=0;i<16;i++) {
			int k=i+j*16;
			clips[k].x=i*fontWidth;
			clips[k].y=j*fontHeight;
			clips[k].w=fontWidth;
			clips[k].h=fontHeight;
		}
	}

	SDL_FillRect( font, NULL, SDL_MapRGB( screen->format, 0xFF, 0x00, 0xFF ) );

	for(int i=0;i<256;i++) {
		Graphics_ApplySurface(i*fontWidth,0,fontSpriteSheet,font,&clips[i]);
	}

	SDL_SaveBMP(font,"font2.bmp");

	//While the user hasn't quit
	while( !quit ) {

		//While there's event to handle
		while( SDL_PollEvent( &event ) ) {
			//If the user has Xed out the window
			switch( event.type ) {
				case SDL_QUIT:
					//Quit the program
					quit = true;
					break;
			}
		}

    //Fill the screen black
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		Graphics_ApplySurface(0,0,font,screen,NULL);

		//Update the screen
		if( SDL_Flip( screen ) == -1 ) {
			return 1;
		}

	}

	SDL_FreeSurface(fontSpriteSheet);
	SDL_FreeSurface(font);

	SDL_Quit();

	return 0;
}
