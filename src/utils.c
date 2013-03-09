#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

#include <sys/time.h>
#include <string.h>
struct timeval tv;
struct timezone tz;

//#define NDEBUG
//#include <assert.h>

/*
 ****************************************************************************
 * Utilities *
 ****************************************************************************
 */

void PrintBoard ()
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
            else if (piece[i] == EPS_SQ)
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
U64 perft (int depth)
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

/*get_ms() nos indica la hora actual en milisegundos desde el 1 de enero de 1970*/
int get_ms()
{
    gettimeofday (&tv, &tz);
    return(tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

void fen(char *s)
{
    int n;
    int i, sq, a;
    int z;

    puts("fen 0");
    n = strlen(s);

    puts("fen 1");
    for (i = 0; i < 64; ++i)
        {
        printf("i: %d\n", i);
            color[i] = EMPTY;
            piece[i] = EMPTY;
        }

    sq = 0;
    a = 0;

    puts("fen 1");

    for (i=0, z = 0; i<n && z == 0; ++i)
        {
            switch(s[i])
                {
                case '1':
                    sq += 1;
                    break;
                case '2':
                    sq += 2;
                    break;
                case '3':
                    sq += 3;
                    break;
                case '4':
                    sq += 4;
                    break;
                case '5':
                    sq += 5;
                    break;
                case '6':
                    sq += 6;
                    break;
                case '7':
                    sq += 7;
                    break;
                case '8':
                    sq += 8;
                    break;
                case 'p':
                    color[sq] = BLACK;
                    piece[sq] = PAWN;
                    ++sq;
                    break;
                case 'n':
                    color[sq] = BLACK;
                    piece[sq] = KNIGHT;
                    ++sq;
                    break;
                case 'b':
                    color[sq] = BLACK;
                    piece[sq] = BISHOP;
                    ++sq;
                    break;
                case 'r':
                    color[sq] = BLACK;
                    piece[sq] = ROOK;
                    ++sq;
                    break;
                case 'q':
                    color[sq] = BLACK;
                    piece[sq] = QUEEN;
                    ++sq;
                    break;
                case 'k':
                    color[sq] = BLACK;
                    piece[sq] = KING;
                    ++sq;
                    break;
                case 'P':
                    color[sq] = WHITE;
                    piece[sq] = PAWN;
                    ++sq;
                    break;
                case 'N':
                    color[sq] = WHITE;
                    piece[sq] = KNIGHT;
                    ++sq;
                    break;
                case 'B':
                    color[sq] = WHITE;
                    piece[sq] = BISHOP;
                    ++sq;
                    break;
                case 'R':
                    color[sq] = WHITE;
                    piece[sq] = ROOK;
                    ++sq;
                    break;
                case 'Q':
                    color[sq] = WHITE;
                    piece[sq] = QUEEN;
                    ++sq;
                    break;
                case 'K':
                    color[sq] = WHITE;
                    piece[sq] = KING;
                    ++sq;
                    break;
                case '/':
                    break;
                default:
                    z = 1;
                    break;
                }
            a = i;
        }

    side  = -1;
    ++a;

    for (i=a, z = 0; i<n && z == 0; ++i)
        {
            switch(s[i])
                {
                case 'w':
                    side = WHITE;
                    break;
                case 'b':
                    side = BLACK;
                    break;
                default:
                    z = 1;
                    break;
                }
            a = i;
        }

    castle = 0;

    for (i=a+1, z = 0; i<n && z == 0; ++i)
        {
            switch(s[i])
                {
                case 'K':
                    castle |= 1;
                    break;
                case 'Q':
                    castle |= 2;
                    break;
                case 'k':
                    castle |= 4;
                    break;
                case 'q':
                    castle |= 8;
                    break;
                case '-':
                    break;
                default:
                    z = 1;
                    break;
                }
            a = i;
        }

//    enpasant = -1;

//    for (i=a+1, z = 0; i<n && z == 0; ++i)
//        {
//            switch(s[i])
//                {
//                case '-':
//                    break;
//                case 'a':
//                    enpasant = 0;
//                    break;
//                case 'b':
//                    enpasant = 1;
//                    break;
//                case 'c':
//                    enpasant = 2;
//                    break;
//                case 'd':
//                    enpasant = 3;
//                    break;
//                case 'e':
//                    enpasant = 4;
//                    break;
//                case 'f':
//                    enpasant = 5;
//                    break;
//                case 'g':
//                    enpasant = 6;
//                    break;
//                case 'h':
//                    enpasant = 7;
//                    break;
//                case '1':
//                    enpasant += 56;
//                    break;
//                case '2':
//                    enpasant += 48;
//                    break;
//                case '3':
//                    enpasant += 40;
//                    break;
//                case '4':
//                    enpasant += 32;
//                    break;
//                case '5':
//                    enpasant += 24;
//                    break;
//                case '6':
//                    enpasant += 16;
//                    break;
//                case '7':
//                    enpasant +=  8;
//                    break;
//                case '8':
//                    enpasant +=  0;
//                    break;
//                default:
//                    z = 1;
//                    break;
//                }
//        }
}
