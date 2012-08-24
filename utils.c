#include <stdio.h>

#include "defs.h"
#include "data.h"
#include "protos.h"

//#define NDEBUG
//#include <assert.h>

/*
 ****************************************************************************
 * Utilities *
 ****************************************************************************
 */

void
PrintBoard ()
{
  char pieceName[] = "PNBRQKpnbrqk";
  int i;
  for (i = 0; i < 64; i++)
    {
      if ((i & 7) == 0)
	{
	  printf ("   +---+---+---+---+---+---+---+---+\n");
	  if (i <= 56)
	    {
	      printf (" %d |", 8 - (((unsigned) i) >> 3));
	    }
	}

      if (piece[i] == EMPTY && ((((unsigned) i) >> 3) % 2 == 0 && i % 2 == 0))
	printf ("   |");
      else if (piece[i] == EMPTY
	       && ((((unsigned) i) >> 3) % 2 != 0 && i % 2 != 0))
	printf ("   |");
      else if (piece[i] == EMPTY)
	printf (" · |");
      else if (piece[i] == EPS_SQUARE)
	printf (" * |");
      else
	{
	  if (color[i] == WHITE)
	    printf (" %c |", pieceName[piece[i]]);
	  else
	    printf ("<%c>|", pieceName[piece[i] + 6]);
	}
      if ((i & 7) == 7)
	printf ("\n");
    }
  printf
    ("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
}


/* Returns the number of posible positions to a given depth. Based on the
 perft function on Danasah */
U64
perft (int depth)
{
  int i;
  int movecnt;			/* The number of available moves */
  U64 nodes = 0;

  if (!depth)
    return 1;

  MOVE moveBuf[200];		/* List of movements */

  /* Generate and count all moves for current position */
  movecnt = GenMoves (side, moveBuf);

  /* Once we have all the moves available, we loop through them */
  for (i = 0; i < movecnt; ++i)
    {
      /* Not a legal move? Then we unmake it and continue to the next one in the list */
      if (!MakeMove (moveBuf[i]))
	{
	  TakeBack ();
	  continue;
	}

      /* Just in case we want to take count of checks */
//        if (IsInCheck(side))
//        {
//            count_checks++;
//        }

      /* This 'if' takes us to the deep of the position */
      nodes += perft (depth - 1);
      TakeBack ();
    }

  return nodes;
}

/*recibimos una posición en formato FEN del GUI cuando editamos y la configuramos para que la entienda el motor*/
void
fen (char *string)
{
  puts ("Loading fen...");
  printf ("El input: %s\n", string);
//    char c;
//    int sq=0, i=0, j=0;
//    /*tipo de pieza según la letra que recibimos, ejemplo B es bishop (nosotros la tenemos definida con el número 2*/
//    int       tpieza[26] = {6,2,6,6,6,6,6,6,6,6,5,6,6,1,6,0,4,3,6,6,6,6,6,6,6,6};

//    puts("Loading fen 1");

//    /*Rellenamos la tabla de posiciones y colores*/
//    while (sq < 64) {

////        printf("sp=%d\n", sq);

////        puts("Loading fen 2");

////        enroque_mascara[sq] = 15;
//        c = string[i++];
//        if ('a'<c && c<'z') {
//            color[sq] = BLACK;
//            piece[sq] = tpieza[c - 'a'];

//            sq++;
//        }
//        else if ('A'<c && c<'Z') {
//            color[sq] = WHITE;
//            piece[sq] = tpieza[c - 'A'];

//            sq++;
//        }
//        else if ('1'<=c && c<='8') {
//            for (j=0; j<(c - '0'); j++) {
//                color[sq+j] = EMPTY;
//                piece[sq+j] = EMPTY;
//            }
//            sq += j;
//        }
////        puts("Loading fen 3");
//    }

//    /*Color para el turno*/
//    c = string[i++];
//    if (c == 'w')             { side = WHITE; }
//    else if (c == 'b')        { side = BLACK; }


//    /*Flag del enroque*/
//        castle = 0;
//        c = string[i++];
//        while (c == 'K' || c == 'Q' || c == 'k' || c == 'q') {
//            if (c == 'K') {
//                castle = castle + 1;
////                tb1 = 63;
//            }
//            if (c == 'Q') {
//                castle = castle + 2;
////                tb2 = 56;
//            }
//            if (c == 'k') {
//                castle = castle + 4;
////                tn1 = 7;
//            }
//            if (c == 'q') {
//                castle = castle + 8;
////                tn2 = 0;
//            }
//            c = string[i++];
//        }


  /*printf("enroque: %d\n", enroque);
     printf("tb1: %d\n", tb1);
     printf("tb2: %d\n", tb2);
     printf("tn1: %d\n", tn1);
     printf("tn2: %d\n", tn2); */

  /*máscaras para los enroques */
//    enroque_mascara[rb] = 12;
//    enroque_mascara[rn] = 3;
//    enroque_mascara[tb2] = 13;
//    enroque_mascara[tb1] = 14;
//    enroque_mascara[tn2] = 7;
//    enroque_mascara[tn1] = 11;

  /*Flag de al paso */
//    c = string[i++];
//    if (c>='a' && c<='h') {
//        alpaso = (c - 'a');
//        c = string[i++];
//        if (c>='1' && c<='8') alpaso += 8*(7-(c - '1'));
//        else alpaso = -1;
//    }
//    else alpaso = -1;


  /*no comprobamos en el fen la regla de 50 movimientos o el número de jugadas, la ponemos directamente a 0 */
//    regla50 = 0;              /*inicamos la regla de los 50 movimientos*/
//    njugadas = 0;             /*ponemos a 0 el contador de jugadas*/

}
