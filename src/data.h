/* Just a copy of data.c */
#include <time.h>
#include <stdio.h>

MOVE bestMove;

FILE *logfile;

clock_t max_time;
clock_t total_time;
clock_t stop_time;
clock_t half_time;
int must_stop;

int history[64][64];
int castle;
int castle_mask[64];
int hdp;
int ply;
U64 nodes;
U64 count_evaluations;
int count_checks;
U64 count_MakeMove;
U64 countquiesCalls;
U64 countCapCalls;
U64 countSearchCalls;
U64 nodes;
int value_piece[6];
HISTO hist[6000];
int piece[64];
int color[64];
int init_color[64];
int init_piece[64];
int side;
int computer_side;
int max_depth;
int pst_pawn_midgame[64];
int pst_pawn_endgame[64];
int pst_knight[64];
int pst_bishop[64];
int pst_rook[64];
int pst_queen[64];
int pst_king_midgame[64];
int pst_king_endgame[64];
int flip[64];
/* The king distance vars */
int qk_dist[64][64];
int rk_dist[64][64];
int nk_dist[64][64];
int bk_dist[64][64];
/* Contador para la regla de los 50 movimientos */
int fifty;
