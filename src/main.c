#include <stdio.h>
#include <string.h>
#include <time.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include "defs.h"
#include "data.h"
#include "protos.h"
#include <inttypes.h>

void startgame()
{
    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = initPiece[i];
        color[i] = initColor[i];
    }

//    setDistToKing();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 15;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test1()
{
    /* Piece in each square */
    int piece_test[64] = {
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, KING, EMPTY, PAWN, KING, EMPTY, EMPTY,
            PAWN, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, PAWN,
            EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
    /* Color of each square */
    int color_test[64] = {
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, WHITE, EMPTY, WHITE, BLACK, EMPTY, EMPTY,
            BLACK, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, BLACK,
            EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 15;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test2()
{
    /* Piece in each square */
    int piece_test[64] = {
            EMPTY, ROOK, BISHOP, EMPTY, EMPTY, ROOK, EMPTY, KING,
            EMPTY, EMPTY, QUEEN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
            PAWN, EMPTY, PAWN, PAWN, PAWN, KNIGHT, EMPTY, EMPTY,
            PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
            EMPTY, PAWN, KNIGHT, QUEEN, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, PAWN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
            ROOK, EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, ROOK, KING };
    /* Color of each square */
    int color_test[64] = {
            EMPTY, BLACK, BLACK, EMPTY, EMPTY, BLACK, EMPTY, BLACK,
            EMPTY, EMPTY, BLACK, EMPTY, BLACK, BLACK, BLACK, BLACK,
            BLACK, EMPTY, BLACK, BLACK, BLACK, BLACK, EMPTY, EMPTY,
            WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
            EMPTY, WHITE, WHITE, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, WHITE, EMPTY, WHITE, WHITE, WHITE, WHITE,
            WHITE, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, WHITE, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test3()
{
    /* Piece in each square */
    int piece_test[64] = {
            ROOK, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, KING, EMPTY,
            PAWN, PAWN, EMPTY, BISHOP, EMPTY, PAWN, PAWN, EMPTY,
            EMPTY, EMPTY, KNIGHT, EMPTY, PAWN, EMPTY, KNIGHT, BISHOP,
            EMPTY, EMPTY, EMPTY, PAWN, PAWN, EMPTY, QUEEN, EMPTY,
            EMPTY, EMPTY, PAWN, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, QUEEN, EMPTY, EMPTY, EMPTY, EMPTY, KNIGHT,
            PAWN, EMPTY, PAWN, EMPTY, BISHOP, PAWN, PAWN, PAWN,
            ROOK, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, KING, EMPTY};
    /* Color of each square */
    int color_test[64] = {
            BLACK, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, BLACK, EMPTY,
            BLACK, BLACK, EMPTY, BLACK, EMPTY, BLACK, BLACK, EMPTY,
            EMPTY, EMPTY, BLACK, EMPTY, BLACK, EMPTY, BLACK, WHITE,
            EMPTY, EMPTY, EMPTY, BLACK, WHITE, EMPTY, WHITE, EMPTY,
            EMPTY, EMPTY, BLACK, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
            EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, WHITE,
            WHITE, EMPTY, WHITE, EMPTY, WHITE, WHITE, WHITE, WHITE,
            WHITE, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE, EMPTY};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    /*setDistToKing*/();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test4()
{
    /* Piece in each square */
    int piece_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, ROOK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING};
    /* Color of each square */
    int color_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    /*setDistToKing*/();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test5()
{
    /* Piece in each square */
    int piece_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, EMPTY, KING,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BISHOP, ROOK, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        ROOK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING};
    /* Color of each square */
    int color_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, WHITE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    /*setDistToKing*/();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test6()
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

//    /*setDistToKing*/();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test7()
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

//    /*setDistToKing*/();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test8()
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

//    /*setDistToKing*/();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test9()
{
    puts ("A position where Kitt plays h5h4??");
    puts ("8/5p2/8/2bk1p1p/2N1p3/pK2P1P1/P4P1P/8 b kq - 0 34");
    /* Piece in each square */
    int piece_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BISHOP, KING, EMPTY, PAWN, EMPTY, PAWN,
        EMPTY, EMPTY, KNIGHT, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        PAWN, KING, EMPTY, EMPTY, PAWN, EMPTY, PAWN, EMPTY,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };
    /* Color of each square */
    int color_test[64] =
    {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, BLACK, BLACK, EMPTY, BLACK, EMPTY, BLACK,
        EMPTY, EMPTY, WHITE, EMPTY, BLACK, EMPTY, EMPTY, EMPTY,
        BLACK, WHITE, EMPTY, EMPTY, WHITE, EMPTY, WHITE, EMPTY,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
    };

    int i;
    for (i = 0; i < 64; ++i)
        {
            piece[i] = piece_test[i];
            color[i] = color_test[i];
        }

//    /*setDistToKing*/();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test10()
{
    puts("Just to check the validity of passed pawn dettection ");
    puts("and the value of the rank bonus arrays.");

    /* Piece in each square */
    int piece_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING};
    /* Color of each square */
    int color_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    /*setDistToKing*/();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test11()
{
    puts("Just to check the validity of passed pawn detection ");
    puts("and the value of the rank bonus arrays.");

    /* Piece in each square */
    int piece_test[64] = {
        KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING};
    /* Color of each square */
    int color_test[64] = {
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test12()
{
    puts("A position where Kitt crashed ");

    /* Piece in each square */
    int piece_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, ROOK, EMPTY, EMPTY,
        EMPTY, KNIGHT, ROOK, EMPTY, BISHOP, EMPTY, EMPTY, EMPTY,
        PAWN, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, PAWN, EMPTY, PAWN, EMPTY, KING, PAWN, PAWN,
        EMPTY, PAWN, EMPTY, PAWN, PAWN, PAWN, EMPTY, EMPTY,
        PAWN, KNIGHT, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, PAWN,
        EMPTY, ROOK, BISHOP, EMPTY, ROOK, EMPTY, KING, EMPTY};
    /* Color of each square */
    int color_test[64] = {
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY,
        EMPTY, BLACK, BLACK, EMPTY, BLACK, EMPTY, EMPTY, EMPTY,
        BLACK, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, WHITE, EMPTY, BLACK, BLACK, BLACK,
        EMPTY, WHITE, EMPTY, WHITE, BLACK, BLACK, EMPTY, EMPTY,
        WHITE, WHITE, EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, BLACK, WHITE, EMPTY, WHITE,
        EMPTY, WHITE, WHITE, EMPTY, WHITE, EMPTY, WHITE, EMPTY};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = BLACK;
    computerSide = WHITE;	/* Human is black side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void test13()
{
    puts("Testing squares protected by pawns");

    /* Piece in each square */
    int piece_test[64] = {
        KING, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN,
        PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, KING};
    /* Color of each square */
    int color_test[64] = {
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK,
        WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE,
        BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */
    hdp = 0;
    castle = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void testMoveGen()
{
    puts("Some hard positions for the moves generator.");
    puts("");
    puts(" A worthy test position by Albert Bertilsson for a *true* move generator \n testing!");//Belka: Albert Bertilsson's test position
    puts(" 8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -  \n");
    puts(" Running Perft the following nodes are true: ");
    puts(" depth=1,  nodes=         14");
    puts(" depth=2,  nodes=        191");
    puts(" depth=3,  nodes=      2,812");
    puts(" depth=4,  nodes=     43,238");
    puts(" depth=5,  nodes=    674,624");
    puts(" depth=6,  nodes= 11,030,083");
    puts(" depth=7,  nodes=178,633,661 \n");

    setBoard("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -");

    hdp = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */

    int i;
    for (i=1; i<6; ++i)
        printf("Perft %d: %d\n", i, perft(i));

    puts("");
    puts(" The famous test position by Peter McKenzie for a *true* move generator testing!");//Belka: Albert Bertilsson's test position
    puts(" r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  \n");
    puts(" Running Perft the following nodes are true: ");
    puts(" depth=1,  nodes=           48");
    puts(" depth=2,  nodes=        2,039");
    puts(" depth=3,  nodes=       97,862");
    puts(" depth=4,  nodes=    4,085,603");
    puts(" depth=5,  nodes=  193,690,690");
    puts(" depth=6,  nodes=8,031,647,685");

    setBoard("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R wKQkq -");

    hdp = 0;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */

    for (i=1; i<5; ++i)
        printf("Perft %d: %d\n", i, perft(i));
}

void testEvalSym()
{
    int depth = 1;

    setBoard("2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1w--");
    ComputerThink(depth);
    setBoard("r4rk1/ppb4p/2p3q1/2Pp4/3Pn3/1NNQBn1P/PP3PP1/2RR3Kb--");
    ComputerThink(depth);
    setBoard("5rk1/1b3p1p/pp3p2/3n1N2/1P6/P1qB1PP1/3Q3P/4R1K1w--");
    ComputerThink(depth);
    setBoard("4r1k1/3q3p/p1Qb1pp1/1p6/3N1n2/PP3P2/1B3P1P/5RK1b--");
    ComputerThink(depth);
    setBoard("r3nrk1/2p2p1p/p1p1b1p1/2NpPq2/3R4/P1N1Q3/1PP2PPP/4R1K1w--");
    ComputerThink(depth);
    setBoard("4r1k1/1pp2ppp/p1n1q3/3r4/2nPpQ2/P1P1B1P1/2P2P1P/R3NRK1b--");
    ComputerThink(depth);
    setBoard("3r2k1/1p1b1pp1/pq5p/8/3NR3/2PQ3P/PP3PP1/6K1b--");
    ComputerThink(depth);
    setBoard("6k1/pp3pp1/2pq3p/3nr3/8/PQ5P/1P1B1PP1/3R2K1w--");
    ComputerThink(depth);
    setBoard("r1bqr1k1/pp1nb1p1/4p2p/3p1p2/3P4/P1N1PNP1/1PQ2PP1/3RKB1RwK-");
    ComputerThink(depth);
    setBoard("3rkb1r/1pq2pp1/p1n1pnp1/3p4/3P1P2/4P2P/PP1NB1P1/R1BQR1K1bk-");
    ComputerThink(depth);
    setBoard("2b3k1/4rrpp/p2p4/2pP2RQ/1pP1Pp1N/1P3P1P/1q6/6RKw--");
    ComputerThink(depth);
    setBoard("6rk/1Q6/1p3p1p/1Pp1pP1n/2Pp2rq/P2P4/4RRPP/2B3K1b--");
    ComputerThink(depth);
    setBoard("r3r1k1/pp1q1pp1/4b1p1/3p2B1/3Q1R2/8/PPP3PP/4R1K1w--");
    ComputerThink(depth);
    setBoard("4r1k1/ppp3pp/8/3q1r2/3P2b1/4B1P1/PP1Q1PP1/R3R1K1b--");
    ComputerThink(depth);
    setBoard("2kr3r/pp1q1ppp/5n2/1Nb5/2Pp1B2/7Q/P4PPP/1R3RK1w--");
    ComputerThink(depth);
    setBoard("1r3rk1/p4ppp/7q/2pP1b2/1nB5/5N2/PP1Q1PPP/2KR3Rb--");
    ComputerThink(depth);
    setBoard("r4rk1/1bR1bppp/4pn2/1p2N3/1P6/P3P3/4BPPP/3R2K1b--");
    ComputerThink(depth);
    setBoard("3r2k1/4bppp/p3p3/1p6/1P2n3/4PN2/1Br1BPPP/R4RK1w--");
    ComputerThink(depth);
    setBoard("r4rk1/1p2ppbp/p2pbnp1/q7/3BPPP1/2N2B2/PPP4P/R2Q1RK1b--");
    ComputerThink(depth);
    setBoard("r2q1rk1/ppp4p/2n2b2/3bppp1/Q7/P2PBNP1/1P2PPBP/R4RK1w--");
    ComputerThink(depth);
    setBoard("8/k1b5/P4p2/1Pp2p1p/K1P2P1P/8/3B4/8w--");
    ComputerThink(depth);
    setBoard(" 8/3b4/8/k1p2p1p/1pP2P1P/p4P2/K1B5/8b--");
    ComputerThink(depth);
    setBoard("5rk1/p5pp/8/8/2Pbp3/1P4P1/7P/4RN1Kb--");
    ComputerThink(depth);
    setBoard("4rn1k/7p/1p4p1/2pBP3/8/8/P5PP/5RK1w--");
    ComputerThink(depth);
    setBoard("8/q1n3kp/2p3p1/1p6/1B1pN3/3P1P2/r1P3PP/2Q2K2w--");
    ComputerThink(depth);
    setBoard("2q2k2/R1p3pp/3p1p2/1b1Pn3/1P6/2P3P1/Q1N3KP/8b--");
    ComputerThink(depth);
    setBoard("r1b2r1R/1p2bpk1/4p1p1/4P1N1/p2p4/5Q2/qPP2PP1/1NKR4w--");
    ComputerThink(depth);
    setBoard("1nkr4/Qpp2pp1/5q2/P2P4/4p1n1/4P1P1/1P2BPK1/R1B2R1rb--");
    ComputerThink(depth);
    setBoard("r3r1k1/pp1q1pp1/4b1p1/3p2B1/3Q1R2/8/PPP3PP/4R1K1w--");
    ComputerThink(depth);
    setBoard("4r1k1/ppp3pp/8/3q1r2/3P2b1/4B1P1/PP1Q1PP1/R3R1K1b--");
    ComputerThink(depth);
    setBoard("r1b2r2/3pNpkp/3pn1p1/2pN3P/2PnP3/q3QP2/4BKP1/1R5Rw--");
    ComputerThink(depth);
    setBoard("1r5r/4bkp1/Q3qp2/2pNp3/2Pn3p/3PN1P1/3PnPKP/R1B2R2b--");
    ComputerThink(depth);
    setBoard("r1bq1r2/ppp2pkp/3p2nn/2bN2NQ/2B1P3/8/PPp3PP/R1B2R1Kw--");
    ComputerThink(depth);
    setBoard("r1b2r1k/ppP3pp/8/2b1p3/2Bn2nq/3P2NN/PPP2PKP/R1BQ1R2b--");
    ComputerThink(depth);
    setBoard("r1bq3k/pp2R2p/3B1bp1/2pB1p2/2Pp4/3P2Q1/P1P3PP/6K1w--");
    ComputerThink(depth);
    setBoard("6k1/p1p3pp/3p2q1/2pP4/2Pb1P2/3b1BP1/PP2r2P/R1BQ3Kb--");
    ComputerThink(depth);
}

void countNodes()
{
    puts("Evals a series of positions and count the number of nodes.");
    puts("The idea is to make a check when changes in the sear are made (vg move ordering)");

    int depth = 6;
    int allNodes = 0;

    setBoard("1kr5/3n4/q3p2p/p2n2p1/PppB1P2/5BP1/1P2Q2P/3R2K1 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1n5k/3q3p/pp1p2pB/5r2/1PP1Qp2/P6P/6P1/2R3K1 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1n6/4bk1r/1p2rp2/pP2pN1p/K1P1N2P/8/P5R1/3R4 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1nr5/1k5r/p3pqp1/3p4/1P1P1PP1/R4N2/3Q1PK1/R7 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1q4k1/5p1p/p1rprnp1/3R4/N1P1P3/1P6/P5PP/3Q1R1K w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1qr1k2r/1p2bp2/pBn1p3/P2pPbpp/5P2/2P1QBPP/1P1N3R/R4K2 b k -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1r1b2k1/2r2ppp/p1qp4/3R1NPP/1pn1PQB1/8/PPP3R1/1K6 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1r1r2k1/p3n2p/b1nqpbp1/2pp4/1p3PP1/2PP1N2/PPN3BP/R1BRQ2K w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("1r2n1rk/pP2q2p/P2p4/4pQ2/2P2p2/5B1P/3R1P1K/3R4 w - -");
    ComputerThink(depth);
    allNodes += nodes;
    setBoard("r1bq1rk1/pp3pbp/3Pp1p1/2p5/4PP2/2P5/P2QB1PP/1RB1K2R b K -");
    ComputerThink(depth);
    allNodes += nodes;

    printf ("Total nodes = %d\n", allNodes);
}


void xboard()
{
    char line[256], command[256], c;
    char args[4][64];
    int from, dest, i;
    MOVE moveBuf[200];
    MOVE theBest;
    int movecnt;
    //int illegal_king = 0;

    signal(SIGINT, SIG_IGN);

    printf ("\n");
//    hashKeyPosition(); /* hash of the initial position */
//    hashRndInit();
    startgame ();

    /* Waiting for a command from GUI */
    for (;;)
    {
        fflush (stdout);
        if (side == computerSide)
        {   /* Computer's turn */
            /* Find out the best move to reply to the current position */
            theBest = ComputerThink (maxDepth);
            if (theBest.type_of_move > 8)
                printf ("type of move the best %d \n", theBest.type_of_move);
            makeMove (theBest);
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
            printf ("move %c%d%c%d%c\n", 'a' + COL(theBest.from), 8
                    - ROW(theBest.from), 'a' + COL(theBest.dest), 8
                    - ROW(theBest.dest), c);

            fflush(stdout);
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
        if (!strcmp (command, "d"))
        {
            printBoard ();
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
            computerSide = EMPTY;
            continue;
        }
        /* If we get a result the engine must stop */
        if (!strcmp(command, "result"))
        {
            computerSide = EMPTY;
            continue;
        }
        if (!strcmp(command, "?")) {
            computerSide = EMPTY;
            continue;
        }
        if (!strcmp(command, ".")) {
            continue;
        }
        if (!strcmp(command, "exit")) {
            continue;
        }
        if (!strcmp(command,"setboard"))
        {
            strcpy(fenBuf, "");
            sscanf(line, "setboard %s %s %s %s", args[0],args[1],args[2],args[3]);
            strcat(fenBuf, args[0]);
            strcat(fenBuf, args[1]);
            strcat(fenBuf, args[2]);
            strcat(fenBuf, args[3]);
            setBoard(fenBuf);
            continue;
        }
        if (!strcmp (command, "white"))
        {
            side = WHITE;
            computerSide = BLACK;
            continue;
        }
        if (!strcmp (command, "black"))
        {
            side = BLACK;
            computerSide = WHITE;
            continue;
        }
        if (!strcmp (command, "sd"))
        {
            sscanf (line, "sd %d", &maxDepth);
            continue;
        }
        if (!strcmp (command, "go"))
        {
            computerSide = side;
            continue;
        }
        /* Taken from TSCP: we receive from the GUI the time we have */
        if (!strcmp(command, "time"))
        {
            sscanf (line, "time %ld", &maxTime);
            /* Convert to miliseconds */
            maxTime *= 10;
            maxTime /= 10;
            maxTime -= 300;
            totalTime = maxTime;

//            if (totalTime < 3000)
//                maxDepth = 6;
//            else
            maxDepth = 32;
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
            takeBack ();
            continue;
        }
        if (!strcmp (command, "remove"))
        {
            if (hdp <= 1)
                continue;
            takeBack ();
            takeBack ();
            continue;
        }

        /* Maybe the user entered a move? */
        /* Is that a move? */
        if (command[0] < 'a' || command[0] > 'h' ||
                command[1] < '0' || command[1] > '9' ||
                command[2] < 'a' || command[2] > 'h' ||
                command[3] < '0' || command[3] > '9')
        {
            printf("Error (unknown command): %s\n", command); /* No move, unknown command */
            continue;
        }

        from = command[0] - 'a';
        from += 8 * (8 - (command[1] - '0'));
        dest = command[2] - 'a';
        dest += 8 * (8 - (command[3] - '0'));
        ply = 0;
        movecnt = genMoves (side, moveBuf);

        /* Loop through the moves to see if it's legal */
        for (i = 0; i < movecnt; ++i) {
            if (moveBuf[i].from == from && moveBuf[i].dest == dest)
            {
                if (piece[from] == PAWN && (dest < 8 || dest > 55))
                {
                    if (command[4] != 'q' && command[4] != 'r' && command[4] != 'b' && command[4] != 'n')
                    {
                        printf ("Illegal move. Bad letter for promo\n");
                        goto goon;
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

                if (moveBuf[i].type_of_move > 8)
                    printf ("Type of move the best %d \n", moveBuf[i].type_of_move);

                if (makeMove (moveBuf[i]))
                {
                    goto goon;	/* Legal move */
                }
                else {
                    printf ("Illegal move. King is in check\n");
                    goto goon;
                }
            }
        }
        printf ("Illegal move.\n");  /* illegal move */

goon:
        continue;
    }
}

int main()
{
    char line[256];
    char args[4][64];

    /* It mainly calls ComputerThink(maxdepth) to calculate to desired ply */
    char s[256];
    int from;
    int dest;
    int i;

    hashRndInit();
    startgame ();

    maxDepth = 6;		/* Max depth to search */
    MOVE moveBuf[200];
    MOVE theBest;
    int movecnt;

    /* Belka */
    puts (" \n Kitteneitor version June 5th 2013 by Emilio Diaz \n =================================================\n\n");
    puts (" Help overview:");
    puts (" making a move: e.g. e2e4, c7c5, a7a8q, e1g1 etc.");
    puts (" d ............ displaying current board");
    puts (" on ........... forcing the engine to move");
    puts (" sd <n> ....... setting depth to <n> plies");
    puts (" undo ......... taking back last move (ply)");
    puts (" quit ......... quit console application \n\n");
    /* Belka */

    side = WHITE;
    computerSide = BLACK;	/* Human is white side */

    hdp = 0;			/* Current move order */
    for (;;)
    {
        fflush (stdout);
        if (side == computerSide)
        {
            /* Computer's turn */
            theBest = ComputerThink (maxDepth);

            if (theBest.type_of_move > 8)
                printf ("type of move the best %d \n", theBest.type_of_move);

            makeMove (theBest);

            /* Just the move without pawn crown */
            printf("move %c%d%c%d",
                   'a' + COL(theBest.from),
                   8 - ROW(theBest.from),
                   'a' + COL(theBest.dest),
                   8 - ROW(theBest.dest));
            /* Check whether it's a crown */
            switch (theBest.type_of_move)
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
                  printf("\n");
            }   /* end switch */

            printBoard ();
            printf ("Castle rights: %d\n", castle);
            fflush (stdout);
            continue;
        }

        printf ("k> ");

        /* Get user input */
        if (!fgets (line, 256, stdin))
            return;
        if (line[0] == '\n')
            continue;
        sscanf (line, "%s", s);

//        if (scanf ("%s", s) == EOF)	/* Shut down the program */
//            return 0;

        if (!strcmp (s, "d"))
        {
            printBoard ();
            continue;
        }
        if (!strcmp (s, "test1"))
        {
            test1 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test2"))
        {
            test2 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test3"))
        {
            test3 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test4"))
        {
            test4 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test5"))
        {
            test5 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test6"))
        {
            test6 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test7"))
        {
            test7 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test8"))
        {
            test8 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test9"))
        {
            test9 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test10"))
        {
            test10 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test11"))
        {
            test11 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test12"))
        {
            test12 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "test13"))
        {
            test13 ();
            printBoard();
            continue;
        }
        if (!strcmp (s, "testMoveGen")) //Belka: McKenzie test position
        {
            testMoveGen();
            printBoard();
            continue;
        }
        if (!strcmp (s, "testEvalSym")) //Belka: McKenzie test position
        {
            testEvalSym();
            continue;
        }
        if (!strcmp (s, "countNodes"))
        {
            countNodes();
            continue;
        }
        if (!strcmp (s, "testWhitePassedPawns"))
        {
            testWhitePassedPawns ();
            continue;
        }
        if (!strcmp (s, "testBlackPassedPawns"))
        {
            testBlackPassedPawns ();
            continue;
        }
        if (!strcmp (s, "testWhiteDoubledPawns"))
        {
            testWhiteDoubledPawns ();
            continue;
        }
        if (!strcmp (s, "testBlackDoubledPawns"))
        {
            testBlackDoubledPawns ();
            continue;
        }
        if (!strcmp (s, "testIsIsolatedPawnWhite"))
        {
            testIsIsolatedPawnWhite ();
            continue;
        }
        if (!strcmp (s, "testIsIsolatedPawnBlack"))
        {
            testIsIsolatedPawnBlack ();
            continue;
        }
        if (!strcmp (s, "showPawnsInfo"))
        {
            showPawnsInfo ();
            continue;
        }
        if (!strcmp (s, "testisSqProtectedByAPawn"))
        {
             testisSqProtectedByAPawn();
            continue;
        }
//        if (!strcmp (s, "testIsSqProtectedByAKnight"))
//        {
//             testIsSqProtectedByAKnight();
//            continue;
//        }
//        if (!strcmp (s, "testIsSqProtectedByABishop"))
//        {
//             testIsSqProtectedByABishop();
//            continue;
//        }
        if (!strcmp (s, "testOpenCols"))
        {
             testOpenCols();
            continue;
        }
        if (!strcmp (s, "undo"))
        {
            takeBack ();
            printBoard ();
            computerSide = (WHITE + BLACK) - computerSide;
            continue;
        }
        if (!strcmp(s,"setboard"))
        {
            strcpy(fenBuf, "");
            sscanf(line, "setboard %s %s %s %s", args[0],args[1],args[2],args[3]);
            strcat(fenBuf, args[0]);
            strcat(fenBuf, args[1]);
            strcat(fenBuf, args[2]);
            strcat(fenBuf, args[3]);
            setBoard(fenBuf);
            continue;
        }
        if (!strcmp (s, "xboard"))
        {
            xboard ();
            return 0;
        }
        if (!strcmp (s, "on"))
        {
            computerSide = side;
            continue;
        }
        if (!strcmp (s, "pass"))
        {
            side = (WHITE + BLACK) - side;
            computerSide = (WHITE + BLACK) - side;
            continue;
        }
        if (!strcmp (s, "sd"))
        {
            sscanf (line, "sd %d", &maxDepth);
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
            sscanf (line, "perft %d", &maxDepth);
            clock_t start;
            clock_t stop;
            double t = 0.0;
            /* Start timer */
            start = clock ();
            U64 count = perft (maxDepth);
            /* Stop timer */
            stop = clock ();
            t = (double) (stop - start) / CLOCKS_PER_SEC;
//            printf ("nodes = %'llu\n", count);
            printf ("nodes = %8"  PRId64 "\n", count); // Belka
            printf ("time = %.2f s\n", t);
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
        movecnt = genMoves (side, moveBuf);

        /* Loop through the moves to see whether it's legal */
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
                        puts("Promoting to a McGuffin..., I'll give you a queen");
                        moveBuf[i].type_of_move = MOVE_TYPE_PROMOTION_TO_QUEEN;
                    }
                }
                if (!makeMove (moveBuf[i]))
                {
                    takeBack ();
                    printf ("Illegal move.\n");
                }
                break;
            }
        printBoard ();
    }
}

/*************************************************************************************
        Functions for detecting moves repetition
**************************************************************************************/

/* Random numbers generator */
int random32()
{
    int i, rnd = 0;

    for (i = 0; i < 32; ++i)
        rnd ^= rand() << i;
    return rnd;
}

/* Fill the vars with random numbers */
void hashRndInit()
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

/* Has of the initial position */
void hashKeyPosition()
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

/* Returns the number of times a position is repeated */
int reps()
{
    int i;
    int r = 1;

    for (i = hdp - fifty; i < hdp; i+=2)
        if (hist[i].hashhist == hash.key)
            ++r;
    return r;
}

/* Initializes the table of distances between squares */
//void setDistToKing()
//{
//    int i, j;

//    int dist_bonus[64][64];

//    /* Basic distance table used to generate separate tables for pieces */
//    for (i = 0; i < 64; ++i)
//    {
//       for (j = 0; j < 64; ++j)
//       {
//          dist_bonus[i][j] = 14 - ( abs( COL(i) - COL(j) ) + abs( ROW(i) - ROW(j) ) );

//          qkDist[i][j]  = dist_bonus[i][j] * 5;
//          rkDist[i][j]  =  dist_bonus[i][j];
//          nkDist[i][j]  =  dist_bonus[i][j] * 4;
//          bkDist[i][j]  = dist_bonus[i][j] * 3;
//       }
//    }
////    for (i = 0; i < 64; ++i)
////    {
////       for (j = 0; j < 64; ++j)
////       {
////           printf (" %d-%d - %d\n", i, j, qkDist[i][j]);
////           printf (" %d-%d - %d\n", j, i, qkDist[j][i]);
////           printf (" %d-%d - %d\n", i, j, nkDist[i][j]);
////           printf (" %d-%d - %d\n", j, i, nkDist[j][i]);
////           puts("");
////       }
////    }

//}

void fen(const char *s)
{
    int n;
    int i, sq, a;
    int z;

    n = strlen(s);

    for (i = 0; i < 64; ++i) {
        color[i] = EMPTY;
        piece[i] = EMPTY;
    }

    sq = 0;
    a = 0;

    for (i=0, z = 0; i<n && z == 0; ++i) {
        switch(s[i]) {
        case '1': sq += 1; break;
        case '2': sq += 2; break;
        case '3': sq += 3; break;
        case '4': sq += 4; break;
        case '5': sq += 5; break;
        case '6': sq += 6; break;
        case '7': sq += 7; break;
        case '8': sq += 8; break;
        case 'p': color[sq] = BLACK; piece[sq] = PAWN;   ++sq; break;
        case 'n': color[sq] = BLACK; piece[sq] = KNIGHT; ++sq; break;
        case 'b': color[sq] = BLACK; piece[sq] = BISHOP; ++sq; break;
        case 'r': color[sq] = BLACK; piece[sq] = ROOK;   ++sq; break;
        case 'q': color[sq] = BLACK; piece[sq] = QUEEN;  ++sq; break;
        case 'k': color[sq] = BLACK; piece[sq] = KING;   ++sq; break;
        case 'P': color[sq] = WHITE; piece[sq] = PAWN;   ++sq; break;
        case 'N': color[sq] = WHITE; piece[sq] = KNIGHT; ++sq; break;
        case 'B': color[sq] = WHITE; piece[sq] = BISHOP; ++sq; break;
        case 'R': color[sq] = WHITE; piece[sq] = ROOK;   ++sq; break;
        case 'Q': color[sq] = WHITE; piece[sq] = QUEEN;  ++sq; break;
        case 'K': color[sq] = WHITE; piece[sq] = KING;   ++sq; break;
        case '/': break;
        default: z = 1; break;
        }
        a = i;
    }

    side  = -1;
    ++a;

    for (i=a, z = 0; i<n && z == 0; ++i) {
        switch(s[i]) {
        case 'w': side = WHITE; break;
        case 'b': side = BLACK; break;
        default: z = 1; break;
        }
        a = i;
    }

    castle = 0;

    for (i=a+1, z = 0; i<n && z == 0; ++i) {
        switch(s[i]) {
        case 'K': castle |= 1; break;
        case 'Q': castle |= 2; break;
        case 'k': castle |= 4; break;
        case 'q': castle |= 8; break;
        case '-': break;
        default: z = 1; break;
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
