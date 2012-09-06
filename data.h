/* Just a copy of data.c */

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
int value_piece[6];
HISTO hist[6000];
int piece[64];
int color[64];
int side;
int computer_side;
int max_depth;
int pst_pawn[64];
int pst_knight[64];
int pst_bishop[64];
int pst_rook[64];
int pst_queen[64];
int pst_king_midgame[64];
int pst_king_endgame[64];
int flip[64];
