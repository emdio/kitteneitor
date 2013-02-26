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

#define ABS(a)	   (((a) < 0) ? -(a) : (a))

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

/* The squares - I didn't want to use enum to make these general purpose */
#define A1  56
#define B1  57
#define C1  58
#define D1  59
#define E1  60
#define F1  61
#define G1  62
#define H1  63
#define A2  48
#define B2  49
#define C2  50
#define D2  51
#define E2  52
#define F2  53
#define G2  54
#define H2  55
#define A3  40
#define B3  41
#define C3  42
#define D3  43
#define E3  44
#define F3  45
#define G3  46
#define H3  47
#define A4  32
#define B4  33
#define C4  34
#define D4  35
#define E4  36
#define F4  37
#define G4  38
#define H4  39
#define A5  24
#define B5  25
#define C5  26
#define D5  27
#define E5  28
#define F5  29
#define G5  30
#define H5  31
#define A6  16
#define B6  17
#define C6  18
#define D6  19
#define E6  20
#define F6  21
#define G6  22
#define H6  23
#define A7  8
#define B7  9
#define C7  10
#define D7  11
#define E7  12
#define F7  13
#define G7  14
#define H7  15
#define A8  0
#define B8  1
#define C8  2
#define D8  3
#define E8  4
#define F8  5
#define G8  6
#define H8  7

/* Files and ranks */
#define COLA 0
#define COLB 1
#define COLC 2
#define COLD 3
#define COLE 4
#define COLF 5
#define COLG 6
#define COLH 7
#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define ROW5 4
#define ROW6 5
#define ROW7 6
#define ROW8 7

/* Contador para la regla de los 50 movimientos */
extern int fifty;

/* 64 bit integers */
typedef unsigned long long U64;

/* Estructura para triple repeticion */
typedef struct tag_REPET
{
    int key;
    int piece[2][6][64];
    int side;
    int ep[64];
} HASH;


/* A move is defined by its origin and final squares, the castle rights and the kind of
 * move it's: normal, enpasant... */
typedef struct
{
    int from;
    int dest;
    int type_of_move;
    int grade;  /* Used to sort the moves */
} MOVE;

/* For storing all moves of game */
typedef struct
{
    MOVE m;
    int castle;
    int cap;
    int hash;
    int fifty;
} HISTO;

/* Estructura para PV */
typedef struct tag_LINE
{
    int cmove;
    MOVE argmove[50];
} LINE;


/* Triple repeticion */
HASH hash;
