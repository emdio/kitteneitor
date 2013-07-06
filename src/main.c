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

void test99()	//Belka: McKenzie test position
{
    puts(" The famous test position by Peter McKenzie for a *true* move generator testing!");
    puts(" r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -  \n");
    puts(" Running Perft the following nodes are true: ");
    puts(" depth=1,  nodes=           48");
    puts(" depth=2,  nodes=        2,039");
    puts(" depth=3,  nodes=       97,862");
    puts(" depth=4,  nodes=    4,085,603");
    puts(" depth=5,  nodes=  193,690,690");
    puts(" depth=6,  nodes=8,031,647,685");

    /* Piece in each square */
    int piece_test[64] = {
        ROOK, EMPTY, EMPTY, EMPTY, KING, EMPTY, EMPTY, ROOK,
        PAWN, EMPTY, PAWN, PAWN, QUEEN, PAWN, BISHOP, EMPTY,
        BISHOP, KNIGHT, EMPTY, EMPTY, PAWN, KNIGHT, PAWN, EMPTY,
        EMPTY, EMPTY, EMPTY, PAWN, KNIGHT, EMPTY, EMPTY, EMPTY,
        EMPTY, PAWN, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, KNIGHT, EMPTY, EMPTY, QUEEN, EMPTY, PAWN,
        PAWN, PAWN, PAWN, BISHOP, BISHOP, PAWN, PAWN, PAWN,
        ROOK, EMPTY, EMPTY, EMPTY, KING, EMPTY, EMPTY, ROOK};
    /* Color of each square */
    int color_test[64] = {
        BLACK, EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, BLACK,
        BLACK, EMPTY, BLACK, BLACK, BLACK, BLACK, BLACK, EMPTY,
        BLACK, BLACK, EMPTY, EMPTY, BLACK, BLACK, BLACK, EMPTY,
        EMPTY, EMPTY, EMPTY, WHITE, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, BLACK, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, WHITE, EMPTY, EMPTY, WHITE, EMPTY, BLACK,
        WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
        WHITE, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, WHITE};

    int i;
    for (i = 0; i < 64; ++i)
    {
        piece[i] = piece_test[i];
        color[i] = color_test[i];
    }

//    setDistToKing();

    side = WHITE;
    computerSide = BLACK;	/* human is white side */
    hdp = 0;
    castle = 15;
    fifty = 0;
    hashKeyPosition(); /* hash of the initial position */
}

void xboard()
{
    char line[256], command[256], c;
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

    char linea[256];
    char args[4][64];

    /* It mainly calls ComputerThink(maxdepth) to the desired ply */

    char  fen_buf[256];  /* For fen support */
    char  *pointer;   /* For fen support */

    setlocale (LC_ALL, "");

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
    puts (" \n Kitteneitor version June 27th 2013 by Emilio Diaz \n =================================================\n\n");
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
            /* Check wether it's a crown */
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
            continue;
        }

        /* Get user input */
        printf ("k> ");
        if (scanf ("%s", s) == EOF)	/* Shut down the program */
            return 0;
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
        if (!strcmp (s, "test99")) //Belka: McKenzie test position
        {
            test99 ();
            printBoard();
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
            scanf ("%d", &maxDepth);
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
            scanf ("%d", &maxDepth);
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
