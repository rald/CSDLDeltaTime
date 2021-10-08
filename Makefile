game: main.o graphics.o timer.o sprite.o
	gcc main.o graphics.o timer.o sprite.o -o game -lm -lSDL -lSDL_gfx -lSDL_image

main.o: main.c
	gcc -c main.c

graphics.o: graphics.c
	gcc -c graphics.c

timer.o: timer.c
	gcc -c timer.c

sprite.o: sprite.c
	gcc -c sprite.c

clean:
	rm main.o graphics.o timer.o sprite.o game

