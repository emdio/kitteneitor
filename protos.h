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
inline int Col (int square);
inline int Row (int square);
inline int Opponent (int color);
void MoveOrder(int from, int movecount, MOVE *moveBuf);

//__forceinline int col(int);

/* Eval */
int Eval();
int endGame();

/* Main */
void startgame();
void xboard();

/* Search */
MOVE ComputerThink(int depth);
int Quiescent(int alpha, int beta);
int Search(int alpha, int beta, int depth, MOVE * pBestMove);
void checkup(clock_t stoping_time);
int checkupHalfTime(int stoping_time);

/* Utils */
U64 perft(int depth);
void PrintBoard();
