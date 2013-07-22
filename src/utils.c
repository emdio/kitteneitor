#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"
#include <sys/time.h>
#include <string.h>

struct timeval tv;
struct timezone tz;


/*
 ****************************************************************************
 * Utilities *
 ****************************************************************************
 */

void
printBoard ()
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
            printf ("   |");

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
    printf ("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
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
    movecnt = genMoves (side, moveBuf);

    /* Once we have all the moves available, we loop through them */
    for (i = 0; i < movecnt; ++i)
    {
        /* Not a legal move? Then we unmake it and continue to the next one in the list */
        if (!makeMove (moveBuf[i]))
        {
            takeBack ();
            continue;
        }

        /* Just in case we want to take count of checks */
//        if (IsInCheck(side))
//        {
//            countChecks++;
//        }

        /* This 'if' takes us to the deep of the position */
        nodes += perft (depth - 1);
        takeBack ();
    }

    return nodes;
}

/*getMs() nos indica la hora actual en milisegundos desde el 1 de enero de 1970*/
int getMs()
{
	gettimeofday (&tv, &tz);
	return(tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

/* Receiving a fen position from the command line */
void setBoard(char *string)
{
    char c;
    char epSq = 0;
    int i = 0;
    int sq = 0;
    int j = 0;
    int pieceType[] ={6,2,6,6,6,6,6,6,6,6,5,6,6,1,6,0,4,3,6,6,6,6,6,6,6,6};

    printf("Fen string: %s\n", string);

    /* Fill up the arrays of color and piece */
        while (sq < 64)
        {
            c = string[i++];
            if ('a'<c && c<'z')
            {
                color[sq] = BLACK;
                piece[sq] = pieceType[c - 'a'];
                sq++;
            }
            else if ('A'<c && c<'Z')
            {
                color[sq] = WHITE;
                piece[sq] = pieceType[c - 'A'];
                sq++;
            }
            else if ('1'<=c && c<='8')
            {
                for (j=0; j<(c - '0'); j++)
                {
                    color[sq+j] = EMPTY;
                    piece[sq+j] = EMPTY;
                }
                sq += j;
            }
        }

        /* Whose turn is it? */
        c = string[i++];
        if (c == 'w')		{ side = WHITE; }
        else if (c == 'b')	{ side = BLACK; }

        computerSide = EMPTY;

        /* Set the castle rights */
        castle = 0;
        c = string[i++];
        while (c == 'K' || c == 'Q' || c == 'k' || c == 'q')
        {
            if (c == 'K')
            {
                castle = castle + 1;
            }
            if (c == 'Q')
            {
                castle = castle + 2;
            }
            if (c == 'k')
            {
                castle = castle + 4;
            }
            if (c == 'q')
            {
                castle = castle + 8;
            }
            c = string[i++];
        }
        printf("Castle = %d\n", castle);

        if (c != '-')
            c = string[i--];

        /* En passant square */
        c = string[i++];
        if (c>='a' && c<='h')
        {
            puts("ep square!");
            epSq = (c - 'a');
            c = string[i++];
            if (c>='1' && c<='8')
            {
                epSq += 8*(7-(c - '1'));
                printf("En passant square: %d\n", epSq);
                piece[epSq] = EPS_SQ;
                color[epSq] = EMPTY;
            }
        }

        hdp = 0;
        fifty = 0;
        hashKeyPosition(); /* hash of the initial position */
}
