#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>

#include "defs.h"
#include "data.h"
#include "protos.h"

//#define NDEBUG
//#include <assert.h>

void
startgame ()
{
    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece[i];
        color[i] = color[i];
    }

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
}

void
xboard ()
{
    char line[256], command[256], c;
    int from, dest, i;
    MOVE moveBuf[200], bestMove;
    int movecnt;

    printf ("\n");

    startgame ();

    for (;;)
    {
        fflush (stdout);
        if (side == computer_side)
        {   /* computer's turn */
            /* Find out the best move to react the current position */
            bestMove = ComputerThink (max_depth);
            MakeMove (bestMove);
            /* send move */
            switch (bestMove.type_of_move)
            {
            case MOVE_TYPE_PROMOTION_TO_QUEEN:
                c = 'q';
                break;
            case MOVE_TYPE_PROMOTION_TO_ROOK:
                c = 'r';
                break;
            case MOVE_TYPE_PROMOTION_TO_BISHOP:
                c = 'b';
                break;
            case MOVE_TYPE_PROMOTION_TO_KNIGHT:
                c = 'n';
                break;
            default:
                c = ' ';
                printf ("move %c%d%c%d%c\n", 'a' + Col (bestMove.from), 8
                        - Row (bestMove.from), 'a' + Col (bestMove.dest), 8
                        - Row (bestMove.dest), c);
            }
            continue;
        }

        if (!fgets (line, 256, stdin))
            return;
        if (line[0] == '\n')
            continue;
        sscanf (line, "%s", command);
        if (!strcmp (command, "xboard"))
        {
            continue;
        }
        if (!strcmp (command, "new"))
        {
            startgame ();
            continue;
        }
        if (!strcmp (command, "quit"))
        {
            return;
        }
        if (!strcmp (command, "force"))
        {
            computer_side = EMPTY;
            continue;
        }
        if (!strcmp (command, "white"))
        {
            side = WHITE;
            computer_side = BLACK;
            continue;
        }
        if (!strcmp (command, "black"))
        {
            side = BLACK;
            computer_side = WHITE;
            continue;
        }
        if (!strcmp (command, "sd"))
        {
            sscanf (line, "sd %d", &max_depth);
            continue;
        }
        if (!strcmp (command, "go"))
        {
            computer_side = side;
            continue;
        }
        if (!strcmp (command, "undo"))
        {
            if (hdp == 0)
                continue;
            TakeBack ();
            continue;
        }
        if (!strcmp (command, "remove"))
        {
            if (hdp <= 1)
                continue;
            TakeBack ();
            TakeBack ();
            continue;
        }

        /* maybe the user entered a move? */
        from = command[0] - 'a';
        from += 8 * (8 - (command[1] - '0'));
        dest = command[2] - 'a';
        dest += 8 * (8 - (command[3] - '0'));
        ply = 0;
        movecnt = GenMoves (side, moveBuf);
        /* loop through the moves to see if it's legal */
        for (i = 0; i < movecnt; i++)
            if (moveBuf[i].from == from && moveBuf[i].dest == dest)
            {
                if (piece[from] == PAWN && (dest < 8 || dest > 55))
                {   /* Promotion move? */
                    switch (command[4])
                    {
                    case 'q':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_QUEEN;
                        break;
                    case 'r':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_ROOK;
                        break;
                    case 'b':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_BISHOP;
                        break;
                    case 'n':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_KNIGHT;
                        break;
                    default:
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_QUEEN;
                    }
                }
                if (!MakeMove (moveBuf[i]))
                {
                    TakeBack ();
                    printf ("Illegal move.\n");
                }
                break;

            }
    }
}

int
main ()
{

    char linea[256];
    char args[4][64];

    /* It mainly calls ComputerThink(maxdepth) to the desired ply */

    setlocale (LC_ALL, "");

    char s[256];
    int from;
    int dest;
    int i;

    startgame ();

    max_depth = 6;		/* max depth to search */
    MOVE moveBuf[200];
    int movecnt;

    puts ("Kitteneitor, by Emilio Diaz");
    puts (" Help");
    puts (" d: display board");
    puts (" MOVE: make a move (e.g. b1c3, a7a8q, e1g1)");
    puts (" on: force computer to move");
    puts (" quit: exit");
    puts (" sd n: set engine depth to n plies");
    puts (" undo: take back last move");

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */

    hdp = 0;			/* Current move order */
    for (;;)
    {
        if (side == computer_side)
        {   /* Computer's turn */
            /* Find out the best move to react the current position */
            MOVE bestMove;
            int depth;
            for (depth = 1; depth <= max_depth; depth++)
            {
                bestMove = ComputerThink (depth);
            }
            MakeMove (bestMove);
            PrintBoard ();
            printf ("CASTLE: %d\n", castle);
            continue;
        }

        /* Get user input */
        printf ("k> ");
        if (scanf ("%s", s) == EOF)	/* close program */
            return 0;
        if (!strcmp (s, "d"))
        {
            PrintBoard ();
            continue;
        }
        if (!strcmp (s, "undo"))
        {
            TakeBack ();
            PrintBoard ();
            computer_side = (WHITE + BLACK) - computer_side;
            continue;
        }
        if (!strcmp (s, "xboard"))
        {
            xboard ();
            return 0;
        }
        if (!strcmp (s, "on"))
        {
            computer_side = side;
            continue;
        }
        if (!strcmp (s, "pass"))
        {
            side = (WHITE + BLACK) - side;
            continue;
        }
        if (!strcmp (s, "sd"))
        {
            scanf ("%d", &max_depth);
            continue;
        }

        if (!strcmp (s, "fen"))
        {
            strcpy (fenstring, "");

            sscanf (linea, "fen %s %s %s %s", args[0], args[1], args[2],
                    args[3]);

            strcat (fenstring, args[0]);
            strcat (fenstring, args[1]);
            strcat (fenstring, args[2]);
            strcat (fenstring, args[3]);

            fen (fenstring);
        }

        if (!strcmp (s, "perft"))
        {
            scanf ("%d", &max_depth);
            clock_t start;
            clock_t stop;
            double t = 0.0;
            /* Start timer */
            start = clock ();
            U64 count = perft (max_depth);
            /* Stop timer */
            stop = clock ();
            t = (double) (stop - start) / CLOCKS_PER_SEC;
            printf ("nodes = %'llu\n", count);
            printf ("time = %'.2f s\n", t);
            continue;
        }
        if (!strcmp (s, "quit"))
        {
            printf ("Good bye!\n");
            return 0;
        }

        /* Maybe the user entered a move? */
        from = s[0] - 'a';
        from += 8 * (8 - (s[1] - '0'));
        dest = s[2] - 'a';
        dest += 8 * (8 - (s[3] - '0'));
        ply = 0;
        movecnt = GenMoves (side, moveBuf);

        /* Loop through the moves to see if it's legal */
        for (i = 0; i < movecnt; i++)
            if (moveBuf[i].from == from && moveBuf[i].dest == dest)
            {
                /* Promotion move? */
                if (piece[from] == PAWN && (dest < 8 || dest > 55))
                {
                    switch (s[4])
                    {
                    case 'q':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_QUEEN;
                        break;

                    case 'r':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_ROOK;
                        break;

                    case 'b':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_BISHOP;
                        break;

                    case 'n':
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_KNIGHT;
                        break;

                    default:
                        puts
                        ("promoting to a McGuffin..., I'll give you a queen");
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_QUEEN;
                    }
                }
                if (!MakeMove (moveBuf[i]))
                {
                    TakeBack ();
                    printf ("Illegal move.\n");
                }
                break;
            }
        PrintBoard ();
    }
}
