#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


#include "common.h"
#include "graphics.h"
#include "timer.h"
#include "sprite.h"
#include "font.h"



#define SCREEN_BPP 32

#define NUM_SPRITES 100

#define FIXED_TIME_STEP (1.0/30.0)


bool quit = false;
SDL_Surface *screen;
SDL_Event event;
SDL_Surface *boxSpriteSheet;
SDL_Surface *fontSpriteSheet;
SDL_Surface *icon;
Timer *timer;
double dt=0;
double fps=0;
double accumulator=0;
int mousex=0,mousey=0;
bool mousedown=false;
bool slowDownKeyDown=false;
bool speedUpKeyDown=false;
bool stopKeyDown=false;
Sprite *sprite[NUM_SPRITES];
Font *font;

double getDeltaTime(double *fps,double timerInSeconds) {
	static double framesPerSecond=0;
	static double previousTime=0;
	static double oldTime=0;
	double currentTime=timerInSeconds;
	double elapsedTime=currentTime-oldTime;

	oldTime=currentTime;
	framesPerSecond++;

	if(currentTime-previousTime>1) {
		previousTime=currentTime;
		*fps=framesPerSecond;
		framesPerSecond=0;
	}

	return elapsedTime;
}


void update() {
	for(int i=0;i<NUM_SPRITES;i++) {
		if(sprite[i]->animate) {
			Sprite_Update(sprite[i]);

			sprite[i]->x+=sprite[i]->vx;
			sprite[i]->y+=sprite[i]->vy;

			if(sprite[i]->x<0) sprite[i]->vx=fabs(sprite[i]->vx);
			if(sprite[i]->x>SCREEN_WIDTH-32) sprite[i]->vx=-fabs(sprite[i]->vx);
			if(sprite[i]->y<0) sprite[i]->vy=fabs(sprite[i]->vy);
			if(sprite[i]->y>SCREEN_HEIGHT-32) sprite[i]->vy=-fabs(sprite[i]->vy);
		}
	}
}

bool inrect(int x,int y,int rx1,int ry1,int rx2,int ry2) {
	return x>=rx1 && x<=rx2 && y>=ry1 && y<=ry2 ? true : false;
}

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

	SDL_WM_SetCaption("CSDLDeltaTime",NULL);

	icon=Graphics_LoadImage("assets/icon.png");

	SDL_WM_SetIcon(icon,NULL);

	boxSpriteSheet=Graphics_LoadImage("assets/box.png");

	fontSpriteSheet=Graphics_LoadImage("assets/font.png");

	font=Font_New(fontSpriteSheet,32,32);

	for(int i=0;i<NUM_SPRITES;i++) {

		int x=rand()%(SCREEN_WIDTH-32);
		int y=rand()%(SCREEN_HEIGHT-32);

		int vx=(rand()%2?1:-1)*(rand()%10+1);
		int vy=(rand()%2?1:-1)*(rand()%10+1);

		sprite[i]=Sprite_New(boxSpriteSheet,x,y,32,32,vx,vy,true);

		sprite[i]->currentFrame=rand()%sprite[i]->numFrames;

	}

	timer=Timer_New();
	Timer_Start(timer);

	//While the user hasn't quit
	while( !quit ) {

		dt=getDeltaTime(&fps,Timer_GetTicks(timer)/1000.0);


		//While there's event to handle
		while( SDL_PollEvent( &event ) ) {
			//If the user has Xed out the window
			switch( event.type ) {
				case SDL_QUIT:
					//Quit the program
					quit = true;
					break;
				case SDL_MOUSEMOTION:
					mousex=event.motion.x;
					mousey=event.motion.y;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mousedown=true;
					break;
				case SDL_MOUSEBUTTONUP:
					mousedown=false;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_1: slowDownKeyDown=true; break;
						case SDLK_2: speedUpKeyDown=true; break;
						case SDLK_3: stopKeyDown=true; break;
					}
					break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) {
						case SDLK_1: slowDownKeyDown=false; break;
						case SDLK_2: speedUpKeyDown=false; break;
						case SDLK_3: stopKeyDown=false; break;
					}
					break;
			}
		}

    //Fill the screen black
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		for(int i=0;i<NUM_SPRITES;i++) {
			Sprite_Draw(screen,sprite[i]);
		}

		rectangleColor(screen,10,10,40,40,0xFFFFFFFF);
		rectangleColor(screen,50,10,80,40,0xFFFFFFFF);
		rectangleColor(screen,90,10,120,40,0xFFFFFFFF);

		if(mousedown) {

			if(inrect(mousex,mousey,10,10,40,40)) { dt/=4; boxColor(screen,10,10,40,40,0xFFFFFFFF); }

			if(inrect(mousex,mousey,50,10,80,40)) { dt*=4; boxColor(screen,50,10,80,40,0xFFFFFFFF); }

			if(inrect(mousex,mousey,90,10,120,40)) { dt=0; boxColor(screen,90,10,120,40,0xFFFFFFFF); }

		}

		if(slowDownKeyDown) { dt/=4; boxColor(screen,10,10,40,40,0xFFFFFFFF); }

		if(speedUpKeyDown) { dt*=4; boxColor(screen,50,10,80,40,0xFFFFFFFF); }

		if(stopKeyDown) { dt=0; boxColor(screen,90,10,120,40,0xFFFFFFFF); }

		Font_Print(screen,font,256,16,"%s","Hello World");


		if(dt>FIXED_TIME_STEP) dt=FIXED_TIME_STEP;

		accumulator+=dt;

		while(accumulator>=FIXED_TIME_STEP) {
			update();
			accumulator-=FIXED_TIME_STEP;
		}

		//Update the screen
		if( SDL_Flip( screen ) == -1 ) {
			return 1;
		}

	}

	SDL_Quit();

	return 0;
}
