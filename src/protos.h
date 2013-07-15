/* Prototypes */

/* Board */
int isAttacked(int current_side, int k);
int isInCheck(int current_side);
void genPush(int from, int dest, int type, MOVE * pBuf, int *pMCount);
void genPushKing(int from, int dest, MOVE * pBuf, int *pMCount);
void genPushNormal(int from, int dest, MOVE * pBuf, int *pMCount);
void genPushPawn(int from, int dest, MOVE * pBuf, int *pMCount);
void genPushPawnTwo(int from, int dest, MOVE * pBuf, int *pMCount);
int genCaps(int current_side, MOVE * pBuf);
int genMoves(int current_side, MOVE * pBuf);
int makeMove(MOVE m);
void takeBack();
inline int opponent (int color);
void moveOrder(int from, int movecount, MOVE *moveBuf);
int badCapture(MOVE mcmov);
inline int isSqProtectedByAPawn(int sq, int side);

/* Eval */
int eval(int alpha, int beta);
int isEndGame();
inline int isDoubledPawnWhite(int sq);
inline int isDoubledPawnBlack(int sq);
int isPassedPawnWhite(int sq);
int isPassedPawnBlack(int sq);
int isIsolatedPawnWhite(int sq);
int isIsolatedPawnBlack(int sq);
int wKingShelter(int sq);
int bKingShelter(int sq);
int bishopMobility(int sq);
int bishopRange(int sq);
int rookMobility(int sq);
int knightMobility(int sq);
int funFactor();
int noMaterial();
int isOnAnOpenCol(int sq);
int whiteKingSafety();
int blackKingSafety();
int isOppCastles();
void getWhitePawnRank(int sq);
void getBlackPawnRank(int sq);
void testWhitePassedPawns();
void testBlackPassedPawns();
void testWhiteDoubledPawns();
void testBlackDoubledPawns();
void testOpenCols();

/* Tests */
void testIsIsolatedPawnWhite();
void testIsIsolatedPawnBlack();
void showPawnsInfo();
void testisSqProtectedByAPawn();

/* Main */
void startgame();
void xboard();
void hashRndInit();
void hashKeyPosition();
void setDistToKing();
void setBoard(char *string);

/* Search */
MOVE ComputerThink(int depth);
int quiescent(int alpha, int beta);
int search(int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline);
void checkup(clock_t stopping_time);
int checkupHalfTime(int stopping_time);
int reps();

/* Utils */
U64 perft(int depth);
void printBoard();
int getMs();
