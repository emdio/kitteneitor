/*
 ****************************************************************************
 * Some definitions *
 ****************************************************************************
 */
#define PAWN    0
#define KNIGHT  1
#define BISHOP  2
#define ROOK    3
#define QUEEN   4
#define KING    5
#define EPS_SQ  6
#define EMPTY   7
#define WHITE   0
#define BLACK   1
#define false   0

#define UP     -8
#define DOWN    8

char fenstring[256];

/* The values of the pieces */
#define VALUE_PAWN 100
#define VALUE_KNIGHT 310
#define VALUE_BISHOP 320
#define VALUE_ROOK 500
#define VALUE_QUEEN 900
#define VALUE_KING 10000

#define MATE 10000 /* equal value of King, losing King==mate */


/* For move generation */
#define MOVE_TYPE_NONE 0
#define MOVE_TYPE_NORMAL 1
#define MOVE_TYPE_CASTLE 2
#define MOVE_TYPE_PAWN_TWO 3
#define MOVE_TYPE_EPS 4
#define MOVE_TYPE_PROMOTION_TO_QUEEN 5
#define MOVE_TYPE_PROMOTION_TO_ROOK 6
#define MOVE_TYPE_PROMOTION_TO_BISHOP 7
#define MOVE_TYPE_PROMOTION_TO_KNIGHT 8

/* Some useful squares */
#define A1 56
#define B1 57
#define C1 58
#define D1 59
#define E1 60
#define F1 61
#define G1 62
#define H1 63
#define A8 0
#define B8 1
#define C8 2
#define D8 3
#define E8 4
#define F8 5
#define G8 6
#define H8 7

/* 64 bit integers */
typedef unsigned long long U64;


/* A move is defined by its origin and final squares, the castle rights and the kind of
 * move it's: normal, enpasant... */
typedef struct {
    int from;
    int dest;
    int type_of_move;
    int grade;  /* Used to sort the moves */
} MOVE;

/* For storing all moves of game */
typedef struct {
    MOVE m;
    int castle;
    int cap;
} HISTO;
