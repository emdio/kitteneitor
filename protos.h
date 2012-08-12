/* prototypes */

MOVE ComputerThink(int depth);
int MakeMove(MOVE m);
void TakeBack();
int Eval();
int IsAttacked(int current_side, int k);
int IsInCheck(int current_side);
void Gen_Push(int from, int dest, int type, MOVE * pBuf, int *pMCount);
void Gen_PushKing(int from, int dest, MOVE * pBuf, int *pMCount);
void Gen_PushPawn(int from, int dest, MOVE * pBuf, int *pMCount);
void Gen_PushPawnTwo(int from, int dest, MOVE * pBuf, int *pMCount);
int GenCaps(int current_side, MOVE * pBuf);
int GenMoves(int current_side, MOVE * pBuf);
void PrintBoard();
int Quiescent(int alpha, int beta);
unsigned long long perft(int depth);
