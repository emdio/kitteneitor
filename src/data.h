/* Just a copy of data.c */
#include <time.h>

//MOVE bestMove;

/* 64 bit integers */
typedef unsigned long long U64;

extern clock_t max_time;
extern clock_t total_time;
extern clock_t stop_time;
extern clock_t half_time;
extern int must_stop;

extern int history[64][64];
extern int castle;
extern int castle_mask[64];
extern int hdp;
extern int ply;
extern U64 nodes;
extern U64 count_evaluations;
extern int count_checks;
extern U64 count_MakeMove;
extern U64 countquiesCalls;
extern U64 countCapCalls;
extern U64 countSearchCalls;
extern U64 nodes;
extern int value_piece[6];
extern HISTO hist[6000];
extern int piece[64];
extern int color[64];
extern int init_color[64];
extern int init_piece[64];
extern int side;
extern int computer_side;
extern int max_depth;
extern int pst_pawn_midgame[64];
extern int pst_pawn_endgame[64];
extern int pst_knight[64];
extern int pst_bishop[64];
extern int pst_rook[64];
extern int pst_queen[64];
extern int pst_king_midgame[64];
extern int pst_king_endgame[64];
extern int flip[64];
/* The king distance vars */
extern int qk_dist[64][64];
extern int rk_dist[64][64];
extern int nk_dist[64][64];
extern int bk_dist[64][64];
/* Contador para la regla de los 50 movimientos */
extern int fifty;
