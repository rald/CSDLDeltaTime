#include "font.h"



Font *Font_New(SDL_Surface *image,int w,int h) {
	Font *font=malloc(sizeof(*font));
	if(font) {
		font->image=image;
		font->w=w;
		font->h=h;
	}
	return font;
}



void Font_Print(SDL_Surface *surface,Font *font,int x,int y,char *fmt,...) {

	int cx=x,cy=y;

	char *buf=NULL;
	int len=0;

	va_list args;
	va_start(args,fmt);
	len=vsnprintf(NULL,0,fmt,args);
	buf=calloc(len+1,sizeof(*buf));
	vsprintf(buf,fmt,args);
	va_end(args);

	for(int i=0;i<strlen(buf);i++) {
		SDL_Rect clip = {buf[i]%16*font->w,buf[i]/16*font->h,font->w,font->h};
		Graphics_ApplySurface(cx,cy,font->image,surface,&clip);
		cx+=font->w;
		if(cx>=SCREEN_WIDTH) {
			cx=0;
			cy+=font->h;
			if(cy>=SCREEN_HEIGHT) {
				break;
			}
		}
	}

	free(buf);
	buf=NULL;

}
