objs=main.o graphics.o timer.o sprite.o font.o

game: $(objs)
	gcc $(objs) -o game -lm -lSDL -lSDL_gfx -lSDL_image

main.o: main.c

graphics.o: graphics.c

timer.o: timer.c

sprite.o: sprite.c

font.o: font.c

clean:
	rm $(objs) game

