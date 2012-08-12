HISTO hist[6000]; /* Game length < 6000 */

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
int castle; /* At start position all castle types ar available*/


/* This mask is applied like this
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
int castle_mask[64];

int hdp; /* Current move order */

/* For searching */
//unsigned long long nodes; /* Count all visited nodes when searching */
int ply; /* ply of search */
int count_evaluations;
int count_checks;
int count_MakeMove;
int countquiesCalls;
int countCapCalls;

/* The values of the pieces in centipawns */
int value_piece[6];


/* Board representation */

/* Piece in each square */
int piece[64];

/* Color of each square */
int color[64];

int side; /* Side to move, value = BLACK or WHITE */
int computer_side;
int max_depth; /* max depth to search */
//unsigned long long nodes; /* Count all visited nodes when searching */

/* * * * * * * * * * * * *
 * Piece Square Tables
 * * * * * * * * * * * * */
/* When evaluating the position we'll add a bonus (or malus) to each piece
 * depending on the very square where it's placed. Vg, a knight in d4 will
 * be given an extra +15, whilst a knight in a1 will be penalized with -40.
 * This simple idea allows the engine to make more sensible moves */
int pst_pawn[64];

int pst_knight[64];

int pst_bishop[64];

int pst_rook[64];

int pst_king[64];

/* The flip array is used to calculate the piece/square
values for BLACKS pieces, without needing to write the
arrays for them (idea taken from TSCP).
The piece/square value of a white pawn is pst_pawn[sq]
and the value of a black pawn is pst_pawn[flip[sq]] */
int flip[64];
