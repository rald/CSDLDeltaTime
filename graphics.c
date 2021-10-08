#include "graphics.h"



SDL_Surface *Graphics_LoadImage( char *filename ) {

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



void Graphics_ApplySurface( int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *clip ) {
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

