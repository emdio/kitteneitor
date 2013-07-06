#include "defs.h"
#include "data.h"
#include <time.h>

/* Contador para la regla de los 50 movimientos */
int fifty;
int side;			/* Side to move, value = BLACK or WHITE */
int computerSide;
int maxDepth;			/* max depth to search */
HISTO hist[6000];		/* Game length < 6000 */
char fenstring[256];
int history[64][64];

/* For castle rights we use a bitfield, like in TSCP
 *
 * 0001 -> White can short castle
 * 0010 -> White can long castle
 * 0100 -> Black can short castle
 * 1000 -> Black can long castle
 *
 * 15 = 1111 = 1*2^3 + 1*2^2 + 1*2^1 + 1*2^0
 *
 */
int castle = 15;		/* At start position all castle types ar available */


/* The next mask is applied like this
 *
 * castle &= castleMask[from] & castleMask[dest]
 *
 * When from and dest are whatever pieces, then nothing happens, otherwise
 * the values are chosen in such a way that if vg the white king moves
 * to F1 then
 *
 * castle = castle & (12 & 15)
 * 1111 & (1100 & 1111) == 1111 & 1100 == 1100
 *
 * and white's lost all its castle rights
 *
 * */
int castleMask[64] = {
    7, 15, 15, 15, 3, 15, 15, 11,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    13, 15, 15, 15, 12, 15, 15, 14
};

int hdp;			/* Current move order */
//int allmoves = 0;

/* For searching */
U64 nodes;			/* Count all visited nodes when searching */
int ply;			/* ply of search */
U64 countEvaluations;
int countChecks;
U64 countMakeMove;
U64 countquiesCalls;
U64 countCapCalls;
U64 countSearchCalls;

/* The values of the pieces in centipawns */
int valuePiece[6] =
{   VALUE_PAWN, VALUE_KNIGHT, VALUE_BISHOP, VALUE_ROOK, VALUE_QUEEN,
    VALUE_KING
};


/* Board representation */
int color[64];
int piece[64];
clock_t maxTime = 9999999;
clock_t stopTime;
clock_t halfTime;
clock_t totalTime;
int mustStop;

/* Piece in each square */
int initPiece[64] = {
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK
};

/* Color of each square */
int initColor[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};

///* Piece in each square */
//int piece[64] = {
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, KING, EMPTY, PAWN, KING, EMPTY, EMPTY,
//        PAWN, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, PAWN,
//        EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };
///* Color of each square */
//int color[64] = {
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, WHITE, EMPTY, WHITE, BLACK, EMPTY, EMPTY,
//        BLACK, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, BLACK,
//        EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY };




/* * * * * * * * * * * * *
 * Piece Square Tables
 * * * * * * * * * * * * */
/* When evaluating the position we'll add a bonus (or malus) to each piece
 * depending on the very square where it's placed. Vg, a knight in d4 will
 * be given an extra +15, whilst a knight in a1 will be penalized with -40.
 * This simple idea allows the engine to make more sensible moves */

int pstPawnMidgame[64] ={ 0,   0,   0,   0,   0,   0,   0,   0,
                            1,   5,   8,  11,  11,   8,   5,   1,
                            0,   2,   3,   9,   9,   3,   2,   0,
                           -1,   0,   2,   5,   5,   2,   0,  -1,
                           -2,  -1,   0,   1,   1,   0,  -1,  -2,
                           -3,  -2,  -2,  -1,  -1,  -2,  -2,  -3,
                           -4,  -4,  -4, -18, -18,  -4,  -4,  -4,
                            0,   0,   0,   0,   0,   0,   0,   0};

int pstPawnEndgame[64] = { 0,   0,   0,   0,   0,   0,   0,   0,
                            19,  19,  19,  19,  19,  19,  19,  19,
                            14,  14,  14,  14,  14,  14,  14,  14,
                             4,   4,   4,   4,   4,   4,   4,   4,
                            -6,  -6,  -6,  -6,  -6,  -6,  -6,  -6,
                           -11, -11, -11, -11, -11, -11, -11, -11,
                           -16, -16, -16, -27, -27, -16, -16, -16,
                             0, 0, 0, 0, 0, 0, 0, 0} ;



int pstKnight[64] = {-70, -15, -15, -15, -15, -15, -15, -70,
                      -20, 15, 15, 15, 15, 15, 15, -20,
                      -20, 15, 20, 25, 25, 20, 15, -20,
                      -20, 10, 22, 28, 28, 22, 10, -20,
                      -20, 10, 10, 26, 26, 10, 10, -20,
                      -20, 10, 21, 17, 17, 21, 10, -20,
                      -20, 10, 10, 15, 15, 10, 10, -20,
                      -30, -10, -14, -11, -11, -14, -10, -30} ;

int pstBishop[64] = {-8, 3, 3, 3, 3, 3, 3, -8,
                       -8, 8, 3, 3, 3, 3, 8, -8,
                       -8, 3, 8, 3, 3, 8, 3, -8,
                       -8, 3, 3, 13, 13, 3, 3, -8,
                       -8, 3, 11, 12, 12, 11, 3, -8,
                       -8, 3, 10, 3, 3, 10, 3, -8,
                       -8, 9, 3, 3, 3, 3, 9, -8,
                       -8, -18, -24, -17, -17, -24, -18, -8} ;

int pstRook[64] = {-1, -1, -1, -1, -1, -1, -1, -1,
                    9, 9, 9, 9, 9, 9, 9, 9,
                    -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, -1, -1, -1, -1, -1,
                    -1, -1, -1, 4, 4, -1, -1, -1} ;

int pstQueen[64] = {-2, -2, -2, -2, -2, -2, -2, -2,
                     -4, 5, 6, 6, 6, 6, 5, -4,
                     -6, 3, 6, 5, 5, 6, 3, -6,
                     -8, 2, 5, 10, 10, 5, 2, -8,
                     -8, 2, 3, 4, 4, 3, 2, -8,
                     -8, 1, 2, -2, -2, 2, 1, -8,
                     -8, 0, 1, 1, 1, 1, 0, -8,
                     -11, -4, -4, -4, -4, -4, -4, -11} ;

int pstKingMidgame[64] = {11, 16, -15, -15, -15, -15, 16, 11,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25,
                            -25, -25, -25, -25, -25, -25, -25, -25} ;

int pstKingEndgame[64] = {0, 10, 20, 30, 30, 20, 10, 0,
                            10, 20, 30, 40, 40, 30, 20, 10,
                            20, 30, 40, 50, 50, 40, 30, 20,
                            30, 40, 50, 60, 60, 50, 40, 30,
                            30, 40, 50, 60, 60, 50, 40, 30,
                            20, 30, 40, 50, 50, 40, 30, 20,
                            10, 20, 30, 40, 40, 30, 20, 10,
                            0, 10, 20, 30, 30, 20, 10, 0} ;

/* The flip array is used to calculate the piece/square
values for BLACKS pieces, without needing to write the
arrays for them (idea taken from TSCP).
The piece/square value of a white pawn is pst_pawn[sq]
and the value of a black pawn is pst_pawn[flip[sq]] */
int flip[64] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7
};
