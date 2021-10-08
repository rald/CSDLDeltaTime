game: main.o timer.o
	gcc main.o timer.o -o game -lm -lSDL -lSDL_gfx -lSDL_image

main.o: main.c
	gcc -c main.c

timer.o: timer.c
	gcc -c timer.c

clean:
	rm main.o timer.o game

