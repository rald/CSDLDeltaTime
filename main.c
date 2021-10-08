#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_image.h>

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>


#include "timer.h"



#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

#define FIXED_TIME_STEP (1.0/30.0)


bool quit = false;
SDL_Surface *screen;
SDL_Event event;
SDL_Surface *box;
SDL_Rect boxClip[4];
int boxNumFrames=4;
int boxFrame=0;
Timer *timer;
double dt=0;
double fps=0;
double accumulator=0;
int mousex=0,mousey=0;
bool mousedown=false;
int x=(SCREEN_WIDTH-32)/2,y=(SCREEN_HEIGHT-32)/2;
int xi=0,yi=0;

SDL_Surface *loadImage( char *filename ) {

	//The image that's loaded
	SDL_Surface *loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface *optimizedImage = NULL;

	//Load the image
	loadedImage = IMG_Load( filename );

	//If the image loaded
	if( loadedImage != NULL ) {
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );

		//If the image was optimized just fine
		if( optimizedImage != NULL ) {
			//Map the color key
			Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0xFF, 0x00, 0xFF );

			//Set all pixels of color R 0xFF, G 0x00, B 0xFF to be transparent
			SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey );
		}
	}

	//Return the optimized image
	return optimizedImage;
}



void applySurface( int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip ) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


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

	boxFrame=(boxFrame+1) % boxNumFrames;

	x+=xi;
	y+=yi;

	if(x<0) xi=fabs(xi);
	if(x>SCREEN_WIDTH-32) xi=-fabs(xi);
	if(y<0) yi=fabs(yi);
	if(y>SCREEN_HEIGHT-32) yi=-fabs(yi);

}

bool inrect(int x,int y,int rx1,int ry1,int rx2,int ry2) {
	return x>=rx1 && x<=rx2 && y>=ry1 && y<=ry2 ? true : false;
}

int main(int argc,char **argv) {

	srand(time(NULL));

	xi=(rand()%2?1:-1)*5;
	yi=(rand()%2?1:-1)*5;

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

	rectangleColor(screen,100,100,200,200,0x00FF00FF);
	circleColor(screen,200,200,50,0x0000FFFF);

	box=loadImage("box.png");

	for(int i=0;i<4;i++) {
		boxClip[i].x=i*32;
		boxClip[i].y=0;
		boxClip[i].w=32;
		boxClip[i].h=32;
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
			}
		}

    //Fill the screen black
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0x00, 0x00, 0x00 ) );

		applySurface(x,y,box,screen,&boxClip[boxFrame]);

		rectangleColor(screen,10,10,40,40,0xFFFFFFFF);
		rectangleColor(screen,50,10,80,40,0xFFFFFFFF);
		rectangleColor(screen,90,10,120,40,0xFFFFFFFF);

		if(mousedown) {

			if(inrect(mousex,mousey,10,10,40,40)) { dt/=2; boxColor(screen,10,10,40,40,0xFFFFFFFF); }

			if(inrect(mousex,mousey,50,10,80,40)) { dt*=2; boxColor(screen,50,10,80,40,0xFFFFFFFF); }

			if(inrect(mousex,mousey,90,10,120,40)) { dt=0; boxColor(screen,90,10,120,40,0xFFFFFFFF); }

		}

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
