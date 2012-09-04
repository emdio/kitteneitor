#include "defs.h"

int side;			/* Side to move, value = BLACK or WHITE */
int computer_side;
int max_depth;			/* max depth to search */
HISTO hist[6000];		/* Game length < 6000 */

char fenstring[256];

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
 * castle &= castle_mask[from] & castle_mask[dest]
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
int castle_mask[64] = {
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
U64 count_evaluations;
int count_checks;
U64 count_MakeMove;
U64 countquiesCalls;
U64 countCapCalls;
U64 countSearchCalls;

/* The values of the pieces in centipawns */
int value_piece[6] =
{   VALUE_PAWN, VALUE_KNIGHT, VALUE_BISHOP, VALUE_ROOK, VALUE_QUEEN,
    VALUE_KING
};


/* Board representation */

/* Piece in each square */
int piece[64] = {
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
int color[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
};

//int piece[64] = {
//        ROOK, KNIGHT, BISHOP, QUEEN, EMPTY, KING, EMPTY, ROOK,
//        PAWN, PAWN, EMPTY, PAWN, BISHOP, PAWN, PAWN, PAWN,
//        EMPTY, EMPTY, PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, BISHOP, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        PAWN, PAWN, PAWN, EMPTY, KNIGHT,KNIGHT, PAWN, PAWN,
//        ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY, EMPTY,ROOK };
///* Color of each square */
//int color[64] = {
//        BLACK, BLACK, BLACK, BLACK, EMPTY, BLACK, EMPTY, BLACK,
//        BLACK, BLACK, EMPTY, WHITE, BLACK, BLACK, BLACK, BLACK,
//        EMPTY, EMPTY, BLACK, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, WHITE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
//        WHITE, WHITE, WHITE, EMPTY, WHITE, BLACK, WHITE, WHITE,
//        WHITE, WHITE, WHITE, WHITE, WHITE, EMPTY, EMPTY, WHITE };


/* * * * * * * * * * * * *
 * Piece Square Tables
 * * * * * * * * * * * * */
/* When evaluating the position we'll add a bonus (or malus) to each piece
 * depending on the very square where it's placed. Vg, a knight in d4 will
 * be given an extra +15, whilst a knight in a1 will be penalized with -40.
 * This simple idea allows the engine to make more sensible moves */
int pst_pawn[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    8,12,15,18,18,15,12, 8,
    7, 9, 10,16,16,10,9, 7,
    6, 7, 9,12,12, 9, 7, 6,
    5, 6, 7, 8, 8, 7, 6, 5,
    4, 5, 5, 6, 6, 5, 5, 3,
    3, 3, 3,-21,-21, 3, 3, 3,
    0, 0, 0, 0, 0, 0, 0, 0
};

int pst_knight[64] = {
    -90, -25, -25, -25, -25, -25, -25, -90,
    -30, 0, 0, 0, 0, 0, 0, -30,
    -30, 0, 0, 0, 0, 0, 0, -30,
    -30, 0, 0, 18, 18, 0, 0, -30,
    -30, 0, 0, 16, 16, 0, 0, -30,
    -30, 0, 11, 0, 0, 11, 0, -30,
    -30, 0, 0, 5, 5, 0, 0, -30,
    -40, -30, -24, -21, -21, -24, -30, -40
};

int pst_bishop[64] = {
    -11, 0, 0, 0, 0, 0, 0, -11,
    -11, 5, 0, 0, 0, 0, 5, -11,
    -11, 0, 5, 0, 0, 5, 0, -11,
    -11, 0, 0, 10, 10, 0, 0, -11,
    -11, 0, 8, 9, 9, 8, 0, -11,
    -11, 0, 7, 0, 0, 5, 0, -11,
    -11, 6, 0, 0, 0, 0, 5, -11,
    -11, -21, -27, -20, -20, -27, -21, -11
};

int pst_rook[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    10, 10, 10, 10, 10, 10, 10, 10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 5, 5, 0, 0, 0
};

int pst_queen[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    -6, 7, 8, 8, 8, 8, 7, 6,
    -6, 5, 6, 7, 7, 6, 5, -6,
    -6, 4, 5, 12, 12, 4, 5, -6,
    -6, 4, 5, 6, 6, 5, 4, -6,
    -6, 3, 4, 0, 0, 4, 3, -6,
    -6, 2, 3, 3, 3, 3, 2, -6,
    -9, -2, -2, -2, -2, -2, -2, -9
};

int pst_king[64] = {
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    -25, -25, -25, -25, -25, -25, -25, -25,
    11, 16, -15, -15, -24, -16, 16, 11
};

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
