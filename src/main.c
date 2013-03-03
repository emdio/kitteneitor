#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>

#include "defs.h"
#include "data.h"
#include "protos.h"
#include <stdlib.h>

#define MAIN_DEBUG

void startgame ()
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

void test1()
{
    puts("A good position for perft");
    puts("FEN: 8/2K1Pk2/p1p5/3p3p/3P4/4P3/2p5/8 w - - 0 1");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, KING, EMPTY, PAWN, KING, EMPTY, EMPTY,
        PAWN, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, WHITE, EMPTY, WHITE, BLACK, EMPTY, EMPTY,
        BLACK, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, BLACK,
        EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test2()
{
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, ROOK, BISHOP, EMPTY, EMPTY, ROOK, EMPTY, KING,
        EMPTY, EMPTY, QUEEN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
        PAWN, EMPTY, PAWN, PAWN, PAWN, KNIGHT, EMPTY, EMPTY,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        EMPTY, PAWN, KNIGHT, QUEEN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, PAWN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
        ROOK, EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, ROOK, KING
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, BLACK, BLACK, EMPTY, EMPTY, BLACK, EMPTY, BLACK,
        EMPTY, EMPTY, BLACK, EMPTY, BLACK, BLACK, BLACK, BLACK,
        BLACK, EMPTY, BLACK, BLACK, BLACK, BLACK, EMPTY, EMPTY,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, WHITE, WHITE, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, WHITE, EMPTY, WHITE, WHITE, WHITE, WHITE,
        WHITE, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, WHITE, WHITE
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = BLACK;
    computer_side = WHITE;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test3()
{
    puts ("FEN: r4rk1/pp1b1pp1/2n1p1nB/3pP1Q1/2pP4/2q4N/P1P1BPPP/R4RK1 b - - 0 1");
    /* Piece in each square */
    int piece_test[64] =
    {
        ROOK, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, KING, EMPTY,
        PAWN, PAWN, EMPTY, BISHOP, EMPTY, PAWN, PAWN, EMPTY,
        EMPTY, EMPTY, KNIGHT, EMPTY, PAWN, EMPTY, KNIGHT, BISHOP,
        EMPTY, EMPTY, EMPTY, PAWN, PAWN, EMPTY, QUEEN, EMPTY,
        EMPTY, EMPTY, PAWN, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, QUEEN, EMPTY, EMPTY, EMPTY, EMPTY, KNIGHT,
        PAWN, EMPTY, PAWN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
        ROOK, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, KING, EMPTY
    };
    /* Color of each square */
    int color_test[64] =
    {
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, BLACK, EMPTY,
        BLACK, BLACK, EMPTY, BLACK, EMPTY, BLACK, BLACK, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, WHITE,
        EMPTY, EMPTY, EMPTY, BLACK, WHITE, EMPTY, WHITE, EMPTY,
        EMPTY, EMPTY, BLACK, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, WHITE,
        WHITE, EMPTY, WHITE, EMPTY, WHITE, WHITE, WHITE, WHITE,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE, EMPTY
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = BLACK;
    computer_side = WHITE;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test4()
{
    puts("Seting up test 4");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, ROOK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test5()
{
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, EMPTY, KING,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BISHOP, ROOK, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test6()
{
    puts ("Just an easy position to check either it'll make");
    puts ("a bad capture in order to give mate.");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BISHOP, BISHOP,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, PAWN, KING,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, QUEEN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, BLACK, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test7()
{
    puts ("A position where Kitt crashed");
    puts ("8/1b6/4p1k1/2p5/7P/4p1PK/2p2q2/6R1 b - - 1 77");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, KING, EMPTY,
        EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, BISHOP, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, KING,
        EMPTY, PAWN, EMPTY, EMPTY, EMPTY, QUEEN, EMPTY, EMPTY,
        EMPTY, ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, BLACK, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, BLACK, EMPTY, WHITE, EMPTY, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test8()
{
    puts ("A position where Kitt crashed");
    puts ("8/5p2/2Rk4/4p3/4r3/8/r6p/5K2 b - - 3 48");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, ROOK, KING, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, ROOK, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING, EMPTY, EMPTY
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, EMPTY, WHITE, BLACK, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = BLACK;
    computer_side = WHITE;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void test9()
{
    puts ("A position where Kitt crashed");
    puts ("q7/1p1P2p1/2p1R1Q1/5p2/1P1p4/6Pk/P6P/7K w - - 1 41");
    /* Piece in each square */
    int piece_test[64] =
    {
        QUEEN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, PAWN, EMPTY, PAWN, EMPTY, EMPTY, PAWN, EMPTY,
        EMPTY, EMPTY, PAWN, EMPTY, ROOK, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, KING,
        EMPTY, PAWN, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, QUEEN, EMPTY,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING
    };
    /* Color of each square */
    int color_test[64] =
    {
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, WHITE, EMPTY, EMPTY, BLACK, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, BLACK,
        EMPTY, WHITE, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

    setDistToKing();

    side = WHITE;
    computer_side = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hash_key_position(); /* hash de la posicion inicial */
}

void xboard()
{
    char line[256], command[256], c;
    int from, dest, i;
    MOVE moveBuf[200];
    MOVE theBest;
    int movecnt;
    //int illegal_king = 0;

    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
    {
        fprintf(logfile, "Entering xboard()\n");
        fflush(logfile);  // always flush the file!
    }

    signal(SIGINT, SIG_IGN);
    printf ("\n");
//    hash_key_position(); /* hash de la posicion inicial */
//    hash_rnd_init();
    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
    {
        fprintf(logfile, "Calling start_game()\n");
        fflush(logfile);  // always flush the file!
    }
    startgame ();
    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
    {
        fprintf(logfile, "Finished calling start_game()\n");
        fflush(logfile);  // always flush the file!
    }

    /* Waiting a command from the GUI */
    for (;;)
        {
            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "Right before flushing in xboard()\n");
                fflush(logfile);  // always flush the file!
            }

            fflush (stdout);

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "Righ after flushing in xboard()\n");
                fflush(logfile);  // always flush the file!
            }

            if (side == computer_side)
                {

                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                    {
                        fprintf(logfile, "side == computer_side in xboard(): %d %d\n", side, computer_side);
                        fflush(logfile);  // always flush the file!
                    }

                    /* computer's turn */
                    /* Find out the best move to react the current position */
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                    {
                        fprintf(logfile, "Calling 'theBest = ComputerThink (max_depth);' in xboard\n", side, computer_side);
                        fflush(logfile);  // always flush the file!
                    }
                    theBest = ComputerThink (max_depth);
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                    {
                        fprintf(logfile, "Finished Calling 'theBest = ComputerThink (max_depth);' in xboard\n", side, computer_side);
                        fprintf(logfile, "Now we calll MakeMove(TheBest)\n");
                        fflush(logfile);  // always flush the file!
                    }
                    MakeMove (theBest);

                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                    {
                        fprintf(logfile, "Now we have to send the move in xboard\n", side, computer_side);
                        fflush(logfile);  // always flush the file!
                    }
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
                            c = '\n';
                        }
                    printf ("move %c%d%c%d%c\n", 'a' + Col (theBest.from), 8
                            - Row (theBest.from), 'a' + Col (theBest.dest), 8
                            - Row (theBest.dest), c);

                    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                    {
                        fprintf(logfile, "Printing result in xboard()\n");
                        fflush(logfile);  // always flush the file!
                    }

                    /* Obtenemos los movimientos del contrario para saber si el juego finalizo */
                    movecnt = GenMoves(side, moveBuf);
                    /* Si es final imprime el resultado */
                    PrintResult(movecnt, moveBuf);

                    continue;
                }

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "Right after computer turn if: side %d computer_side %d\n", side, computer_side);
                fflush(logfile);  // always flush the file!
            }

            if (!fgets(line, 256, stdin))
            {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "if (!fgets (line, 256, stdin)) happened!!!\n");
                    fflush(logfile);  // always flush the file!
                }
                return;
            }

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "Right after 'if (!fgets (line, 256, stdin))'\n", side, computer_side);
                fflush(logfile);  // always flush the file!
            }

            if (line[0] == '\n')
            {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "line[0] == 'n' happened!\n");
                    fflush(logfile);  // always flush the file!
                }
                continue;
            }

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "Scanning for a command in xboard()\n");
                fflush(logfile);  // always flush the file!
            }

            sscanf (line, "%s", command);

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "We got command in xboard()\n");
                fflush(logfile);  // always flush the file!
            }

            if (!strcmp (command, "xboard"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called xboard in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    continue;
                }
            if (!strcmp (command, "new"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called new in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    startgame ();
                    continue;
                }
            if (!strcmp (command, "quit"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called quit in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    return;
                }
            if (!strcmp (command, "force"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called force in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    computer_side = EMPTY;
                    continue;
                }
            /*si recibimos un resultado de un juego el motor debe parar*/
            if (!strcmp(command, "result"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called result in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    computer_side = EMPTY;
                    continue;
                }
            if (!strcmp(command, "?"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called command '?'\n");
                    fflush(logfile);  // always flush the file!
                }
                    computer_side = EMPTY;
                    continue;
                }
            if (!strcmp(command, "."))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called . in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    continue;
                }
            if (!strcmp(command, "exit"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called exit in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    continue;
                }
            if (!strcmp (command, "white"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called white in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    side = WHITE;
                    computer_side = BLACK;
                    continue;
                }
            if (!strcmp (command, "black"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called black in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    side = BLACK;
                    computer_side = WHITE;
                    continue;
                }
            if (!strcmp (command, "sd"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called sd in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    sscanf (line, "sd %d", &max_depth);
                    continue;
                }
            if (!strcmp (command, "go"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Entering go in xboard\n");
                    fflush(logfile);  // always flush the file!
                }
                    computer_side = side;
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Leaving go in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    continue;
                }
            /* Tomado de TSCP recibimos del GUI el tiempo que nos queda */
            if (!strcmp(command, "time"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called time in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    sscanf (line, "time %d", &max_time);
                    /*pasamos a milisegundos que es como trabajamos internamente*/
                    max_time *= 10;
                    max_time /= 10;
                    max_time -= 300;
                    total_time = max_time;

//            if (total_time < 3000)
//                max_depth = 6;
//            else
                    max_depth = 3;
                    continue;
                }
            if (!strcmp(command, "otim"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called otime in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    continue;
                }
            if (!strcmp (command, "undo"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called undo in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    if (hdp == 0)
                        continue;
                    TakeBack ();
                    continue;
                }
            if (!strcmp (command, "remove"))
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Called remove in xboard()\n");
                    fflush(logfile);  // always flush the file!
                }
                    if (hdp <= 1)
                        continue;
                    TakeBack ();
                    TakeBack ();
                    continue;
                }

            /* maybe the user entered a move? */
            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "We have a move in xboard()\n");
                fflush(logfile);  // always flush the file!
            }

            /* is a move? */
            if (command[0] < 'a' || command[0] > 'h' ||
                    command[1] < '0' || command[1] > '9' ||
                    command[2] < 'a' || command[2] > 'h' ||
                    command[3] < '0' || command[3] > '9')
                {
                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                {
                    fprintf(logfile, "Unknown command received in xboard(): ");
                    fprintf(logfile, "%d %d %d %d \n", command[0],command[1], command[2], command[3]);
                    fflush(logfile);  // always flush the file!
                }
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
            for (i = 0; i < movecnt; ++i)
                {
                    if (moveBuf[i].from == from && moveBuf[i].dest == dest)
                        {
                            if (piece[from] == PAWN && (dest < 8 || dest > 55))
                                {
                                    if (command[4] != 'q' && command[4] != 'r' && command[4] != 'b' && command[4] != 'n')
                                        {
                                            printf ("Illegal move. Bad letter for promo\n");
                                            goto continuar;
                                        }
                                    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                                    {
                                        fprintf(logfile, "Looping through moves in xboard(): ");
                                        fflush(logfile);  // always flush the file!
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

//                if (moveBuf[i].type_of_move > 8)
//                    printf ("type of move the best %d \n", moveBuf[i].type_of_move);

                            if (MakeMove (moveBuf[i]))
                                {
                                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                                {
                                    fprintf(logfile, "We read a move in xboard()\n");
                                    fflush(logfile);  // always flush the file!
                                }
                                    goto continuar;	/* legal move */
                                }
                            else
                                {
                                if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
                                {
                                    fprintf(logfile, "Ilegal move in xboard(): ");
                                    fflush(logfile);  // always flush the file!
                                }
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

int main()
{

    FILE *ini;
    FILE *log;
    char val[128];
    int  use_log_file = 0;

    char linea[256];
    char args[4][64];

    /* It mainly calls ComputerThink(maxdepth) to the desired ply */

    char  fen_buf[256];  /* Para soporte fen */
    char  *pointer;   /* Para soporte fen */

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

    /* Load the conf file */
    ini = fopen("kitt.ini", "r");
    if (ini == NULL)
    {
        printf("I couldn't open the ini file.\n");
    }
    else
    {
        printf("I could open the ini file.\n");
    }

    while (fscanf(ini, "%s %d", val, &use_log_file) != EOF)
    {
        printf("%s %d\n", val, use_log_file);
//        printf("Whatever.\n");
    }
    fclose(ini);

    if ( use_log_file )
        logfile = fopen("kitt.log", "w");

    if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
    {
        fprintf(logfile, "First text added to log file\n");
        fflush(logfile);  // always flush the file!
    }


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
                {
                    /* Computer's turn */

                    theBest = ComputerThink (max_depth);

#ifdef MAIN_DEBUG
                    if (theBest.type_of_move > 8)
                        printf ("type of move the best > 8: %d \n", theBest.type_of_move);
                    if (theBest.type_of_move < 0)
                        printf ("type of move the best < 0: %d \n", theBest.type_of_move);
#endif

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
            if (!strcmp (s, "test1"))
                {
                    test1 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test2"))
                {
                    test2 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test3"))
                {
                    test3 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test4"))
                {
                    test4 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test5"))
                {
                    test5 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test6"))
                {
                    test6 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test7"))
                {
                    test7 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test8"))
                {
                    test8 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "test9"))
                {
                    test9 ();
                    PrintBoard();
                    continue;
                }
            if (!strcmp (s, "undo"))
                {
                    TakeBack ();
                    PrintBoard ();
                    computer_side = (WHITE + BLACK) - computer_side;
                    continue;
                }
            if (!strcmp(s,"setboard"))
                {
                    strcpy(fen_buf, linea);
                    pointer = strstr(fen_buf, " ");
                    pointer++;
                    fen(pointer);
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

//        if (!strcmp (s, "fen"))
//        {
//            strcpy (fenstring, "");

//            sscanf (linea, "fen %s %s %s %s", args[0], args[1], args[2],
//                    args[3]);

//            strcat (fenstring, args[0]);
//            strcat (fenstring, args[1]);
//            strcat (fenstring, args[2]);
//            strcat (fenstring, args[3]);

//            fen (fenstring);
//        }

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

void PrintResult(int count, MOVE *ListMoves)
{
    int i;

    /* Hay un movimiento legal ? */
    for (i = 0; i < count; ++i)
        {
            if (MakeMove(ListMoves[i]))
                {
                    TakeBack();
                    break;
                }
            else
                TakeBack();
        }

    if (i == count)
        {
            /* Mate o ahogado */
            computer_side = EMPTY;   /* modo force */
            if (IsInCheck(side))
                {
                    /* Mate */
                    if (side == WHITE)
                        printf("0-1 {Black mates}\n");
                    else
                        printf("1-0 {White mates}\n");
                }
            else
                /* Ahogado */
                printf("1/2-1/2 {Stalemate}\n");
        }
    //else if (fifty >= 100)
    //{
    ///* Regla de los 50 movimientos */
    //printf("1/2-1/2 {Draw by fifty move rule}\n");
    //computer_side = EMPTY;   /* modo force */
    //}
    //else if (reps() == 3)
    //{
    ///* Triple repeticion */
    //printf("1/2-1/2 {Draw by repetition}\n");
    //computer_side = EMPTY;   /* modo force */
    //}
    //else if (NoMaterial())
    //{
    ///* Insuficiencia de Material */
    //printf("1/2-1/2 {Insufficient material}\n");
    //computer_side = EMPTY;   /* modo force */
    //}
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
                    dist_bonus[i][j] = 14 - ( ABS( Col(i) - Col(j) ) + ABS( Row(i) - Row(j) ) );

                    qk_dist[i][j]  = dist_bonus[i][j] * 5;
                    rk_dist[i][j]  =  dist_bonus[i][j];
                    nk_dist[i][j]  =  dist_bonus[i][j] * 4;
                    bk_dist[i][j]  = dist_bonus[i][j] * 3;
                }
        }
//    for (i = 0; i < 64; ++i)
//    {
//       for (j = 0; j < 64; ++j)
//       {
//           printf (" %d-%d - %d\n", i, j, qk_dist[i][j]);
//           printf (" %d-%d - %d\n", j, i, qk_dist[j][i]);
//           printf (" %d-%d - %d\n", i, j, nk_dist[i][j]);
//           printf (" %d-%d - %d\n", j, i, nk_dist[j][i]);
//           puts("");
//       }
//    }

}

void fen(const char *s)
{
    int n;
    int i, sq, a;
    int z;

    n = strlen(s);

    for (i = 0; i < 64; ++i)
        {
            color[i] = EMPTY;
            piece[i] = EMPTY;
        }

    sq = 0;
    a = 0;

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

//    for (i=a+1, z = 0; i<n && z == 0; ++i) {
//        switch(s[i]) {
//        case '-': break;

//        case 'a': EPS_SQ = 0; break;
//        case 'b': EPS_SQ = 1; break;
//        case 'c': EPS_SQ = 2; break;
//        case 'd': EPS_SQ = 3; break;
//        case 'e': EPS_SQ = 4; break;
//        case 'f': EPS_SQ = 5; break;
//        case 'g': EPS_SQ = 6; break;
//        case 'h': EPS_SQ = 7; break;
//        case '1': EPS_SQ += 56; break;
//        case '2': EPS_SQ += 48; break;
//        case '3': EPS_SQ += 40; break;
//        case '4': EPS_SQ += 32; break;
//        case '5': EPS_SQ += 24; break;
//        case '6': EPS_SQ += 16; break;
//        case '7': EPS_SQ +=  8; break;
//        case '8': EPS_SQ +=  0; break;
//        default: z = 1; break;
//        }
}
