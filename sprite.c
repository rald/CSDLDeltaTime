#include "sprite.h"

Sprite *Sprite_New(SDL_Surface *image,int x,int y,int w,int h,int vx,int vy) {
	Sprite *sprite=malloc(sizeof(*sprite));

	if(sprite) {
		sprite->x=x;
		sprite->y=y;
		sprite->w=w;
		sprite->h=h;
		sprite->vx=vx;
		sprite->vy=vy;
		sprite->numFrames=image->w/w;
		sprite->currentFrame=0;
		sprite->image=image;
		sprite->clips=malloc(sizeof(*sprite->clips)*sprite->numFrames);
		for(int i=0;i<sprite->numFrames;i++) {
			sprite->clips[i].x=i*w;
			sprite->clips[i].y=0;
			sprite->clips[i].w=w;
			sprite->clips[i].h=h;
		}
	}

	return sprite;
}

void Sprite_Draw(SDL_Surface *surface,Sprite *sprite) {
	Graphics_ApplySurface(sprite->x,sprite->y,sprite->image,surface,&sprite->clips[sprite->currentFrame]);
}
