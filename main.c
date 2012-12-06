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
        piece[i] = init_piece[i];
        color[i] = init_color[i];
    }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 15;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void
xboard ()
{
    char line[256], command[256], c;
    int from, dest, i;
    MOVE moveBuf[200];
    MOVE theBest;
    int movecnt;
    //int illegal_king = 0;

    printf ("\n");
//    hash_key_position(); /* hash de la posicion inicial */
//    hash_rnd_init();
    startgame ();

    /* Waiting a command from the GUI */
    for (;;)
    {
        fflush (stdout);
//        setbuf(stdout, NULL);
        if (side == computer_side)
        {   /* computer's turn */
            /* Find out the best move to react the current position */
            theBest = ComputerThink (max_depth);
            MakeMove (theBest);
            /* send move */
            switch (theBest.type_of_move)
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
            }
            printf ("move %c%d%c%d%c\n", 'a' + Col (theBest.from), 8
                    - Row (theBest.from), 'a' + Col (theBest.dest), 8
                    - Row (theBest.dest), c);

            fflush(stdout);
//            setbuf(stdout, NULL);
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
        /*si recibimos un resultado de un juego el motor debe parar*/
        if (!strcmp(command, "result"))
        {
            computer_side = EMPTY;
            continue;
        }
        if (!strcmp(command, "?")) {
            computer_side = EMPTY;
            continue;
        }
        if (!strcmp(command, ".")) {
            continue;
        }
        if (!strcmp(command, "exit")) {
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
        /* Tomado de TSCP recibimos del GUI el tiempo que nos queda */
        if (!strcmp(command, "time"))
        {
            sscanf (line, "time %d", &max_time);
            /*pasamos a milisegundos que es como trabajamos internamente*/
            max_time *= 10;
            max_time /= 10;
            max_time -= 300;
            total_time = max_time;

//            if (total_time < 3000)
//                max_depth = 6;
//            else
            max_depth = 32;
            continue;
        }
        if (!strcmp(command, "otim"))
        {
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

        /* is a move? */
        if (command[0] < 'a' || command[0] > 'h' ||
                command[1] < '0' || command[1] > '9' ||
                command[2] < 'a' || command[2] > 'h' ||
                command[3] < '0' || command[3] > '9')
        {
            printf("Error (unknown command): %s\n", command); /*no move, unknown command */
            continue;
        }

        from = command[0] - 'a';
        from += 8 * (8 - (command[1] - '0'));
        dest = command[2] - 'a';
        dest += 8 * (8 - (command[3] - '0'));
        ply = 0;
        movecnt = GenMoves (side, moveBuf);

        /* loop through the moves to see if it's legal */
        for (i = 0; i < movecnt; ++i) {
            if (moveBuf[i].from == from && moveBuf[i].dest == dest)
            {
                if (piece[from] == PAWN && (dest < 8 || dest > 55))
                {
                    if (command[4] != 'q' && command[4] != 'r' && command[4] != 'b' && command[4] != 'n')
                    {
                        printf ("Illegal move. Bad letter for promo\n");
                        goto continuar;
                    }
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
                    }
                }

                if (MakeMove (moveBuf[i]))
                {
                    goto continuar;	/* legal move */
                }
                else {
                    printf ("Illegal move. King is in check\n");
                    goto continuar;
                }
            }
        }
        printf ("Illegal move.\n");  /* illegal move */

continuar:
        continue;
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

    hash_rnd_init();
    startgame ();

    max_depth = 6;		/* max depth to search */
    MOVE moveBuf[200];
    MOVE theBest;
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

            theBest = ComputerThink (max_depth);

            MakeMove (theBest);

            /* Se manda el movimiento sin enter para verificar coronacion */
            printf("move %c%d%c%d",
                   'a' + Col(theBest.from),
                   8 - Row(theBest.from),
                   'a' + Col(theBest.dest),
                   8 - Row(theBest.dest));
            /* Verificar si es coronacion para poner la nueva pieza */
            switch (bestMove.type_of_move)
            {
               case MOVE_TYPE_PROMOTION_TO_QUEEN:
                  printf("q\n");
                  break;
               case MOVE_TYPE_PROMOTION_TO_ROOK:
                  printf("r\n");
                  break;
               case MOVE_TYPE_PROMOTION_TO_BISHOP:
                  printf("b\n");
                  break;
               case MOVE_TYPE_PROMOTION_TO_KNIGHT:
                  printf("n\n");
                  break;
               default:
                  printf("\n"); /* no es coronacion enviamos el move con enter */
            }   /* end switch */

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
            computer_side = (WHITE + BLACK) - side;
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


/*************************************************************************************
        Funciones para detectar repeticion de movimientos
**************************************************************************************/

/* Generador de numeros aleatorios */
int random32()
{
    int i, rnd = 0;

    for (i = 0; i < 32; ++i)
        rnd ^= rand() << i;
    return rnd;
}

/* Se llenan las variables con numeros aleatorios */
void hash_rnd_init()
{
    int i, j, k;

    srand(0);
    for (i = 0; i < 2; ++i)
        for (j = 0; j < 6; ++j)
            for (k = 0; k < 64; ++k)
                hash.piece[i][j][k] = random32();
    hash.side = random32();
    for (i = 0; i < 64; ++i)
        hash.ep[i] = random32();
}

/* Se obtiene el hash de la posicion actual */
void hash_key_position()
{
    int i;

    hash.key = 0;
    for (i = 0; i < 64; ++i)
        if (color[i] != EMPTY)
            hash.key ^= hash.piece[color[i]][piece[i]][i];
    if (side == BLACK)
        hash.key ^= hash.side;
//    if (enpasant != -1)
//        hash.key ^= hash.ep[enpasant];
}

/* Devuelve el numero de veces que la posicion se ha repetido */
int reps()
{
    int i;
    int r = 1;

    for (i = hdp - fifty; i < hdp; i+=2)
        if (hist[i].hash == hash.key)
            ++r;
    return r;
}

/* Initializes the table of distances between squares */
void setDistToKing()
{
    int i, j;

    int dist_bonus[64][64];

    /* Basic distance table used to generate separate tables for pieces */
    for (i = 0; i < 64; ++i)
    {
       for (j = 0; j < 64; ++j)
       {
          dist_bonus[i][j] = 14 - ( abs( Col(i) - Col(j) ) + abs( Row(i) - Row(j) ) );

          qk_dist[i][j]  = (dist_bonus[i][j] * 5) / 2;
          rk_dist[i][j]  =  dist_bonus[i][j] / 2;
          nk_dist[i][j]  =  dist_bonus[i][j];
          bk_dist[i][j]  = dist_bonus[i][j] / 2;
       }
    }
}
