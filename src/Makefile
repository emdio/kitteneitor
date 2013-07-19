CC=gcc
//Belka
//just in the event your compiler is not defaulted to:
//CFLAGS=-O3 -funroll-loops
CFLAGS=-O3 -mtune=generic -march=i686

kitteneitormake: board.c data.c eval.c main.c search.c utils.c
	$(CC) -o kitteneitor *.c $(CFLAGS)
