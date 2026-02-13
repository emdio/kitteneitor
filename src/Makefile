CC=gcc
//Belka
/* Lavavej, a professional from Microsoft, told me: "I recommend against -funroll-loops. 
GCC's documentation says: 'This option makes code larger, and may or may not make it run faster.' 
If it were a strong win, it'd be implied by -O3. "   and
"I add -fomit-frame-pointer because I don't care about debugging."  */
//CFLAGS=-O3 -funroll-loops
CFLAGS=-O3 -fomit-frame-pointer

kitteneitormake: board.c data.c eval.c main.c search.c utils.c
	$(CC) -o kitteneitor *.c $(CFLAGS)
