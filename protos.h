/* Prototypes */

/* Board */
int IsAttacked(int current_side, int k);
int IsInCheck(int current_side);
void Gen_Push(int from, int dest, int type, MOVE * pBuf, int *pMCount);
void Gen_PushKing(int from, int dest, MOVE * pBuf, int *pMCount);
void Gen_PushNormal(int from, int dest, MOVE * pBuf, int *pMCount);
void Gen_PushPawn(int from, int dest, MOVE * pBuf, int *pMCount);
void Gen_PushPawnTwo(int from, int dest, MOVE * pBuf, int *pMCount);
int GenCaps(int current_side, MOVE * pBuf);
int GenMoves(int current_side, MOVE * pBuf);
int MakeMove(MOVE m);
void TakeBack();

/* Eval */
int Eval();

/* Main */
void startgame();
void xboard();

/* Search */
MOVE ComputerThink(int depth);
int Quiescent(int alpha, int beta);
int Search(int alpha, int beta, int depth, MOVE * pBestMove);

/* Utils */
unsigned long long perft(int depth);
void PrintBoard();








