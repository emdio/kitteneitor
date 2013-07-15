/* Just a copy of data.c */
#include <time.h>

/* 64 bit integers */
typedef unsigned long long U64;

extern clock_t maxTime;
extern clock_t totalTime;
extern clock_t stopTime;
extern clock_t halfTime;
extern int mustStop;
extern char fenBuf[256];
extern int history[64][64];
extern int castle;
extern int castleMask[64];
extern int hdp;
extern int ply;
extern U64 nodes;
extern U64 countEvaluations;
extern int countChecks;
extern U64 countMakeMove;
extern U64 countquiesCalls;
extern U64 countCapCalls;
extern U64 countSearchCalls;
extern int valuePiece[6];
extern HISTO hist[6000];
extern int piece[64];
extern int color[64];
extern int initColor[64];
extern int initPiece[64];
extern int side;
extern int computerSide;
extern int maxDepth;
extern int pstPawnMidgame[64];
extern int pstPawnEndgame[64];
extern int pstKnight[64];
extern int pstBishop[64];
extern int pstRook[64];
extern int pstQueen[64];
extern int pstKingMidgame[64];
extern int pstKingEndgame[64];
extern int flip[64];
/* The king distance vars */
extern int qkDist[64][64];
extern int rkDist[64][64];
extern int nkDist[64][64];
extern int bkDist[64][64];
/* Contador para la regla de los 50 movimientos */
extern int fifty;
