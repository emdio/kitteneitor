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
int BadCapture(MOVE mcmov);
inline int IsSqProtectedByAPawn(int sq, int side);

//__forceinline int col(int);

/* Eval */
int Eval(int alpha, int beta);
int endGame();
inline int isDoubledPawnWhite(int sq);
inline int isDoubledPawnBlack(int sq);
int isPassedPawnWhite(int sq);
int isPassedPawnBlack(int sq);
int BishopMobility(int sq);
int BishopRange(int sq);
int RookMobility(int sq);
int KnightMobility(int sq);
int funFactor();
int NoMaterial();
int isOnAnOpenCol(int sq);
int whiteKingSafety();
int blackKingSafety();

/* Main */
void startgame();
void xboard();
void hash_rnd_init();
void hash_key_position();
void setDistToKing();

/* Search */
MOVE ComputerThink(int depth);
int Quiescent(int alpha, int beta);
int Search(int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline);
void checkup(clock_t stoping_time);
int checkupHalfTime(int stoping_time);
int reps();

/* Utils */
void PrintResult(int count, MOVE *ListMoves);
U64 perft(int depth);
void PrintBoard();
int get_ms();
void fen(char *s);
