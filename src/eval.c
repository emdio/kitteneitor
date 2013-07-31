#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

/*
 ****************************************************************************
 * Evaluation for current position - main "brain" function *
 ****************************************************************************
 */

/* Bonus and malus */
#define	ROOK_OPEN_COL                   18
#define PAIR_BISHOPS                    25
#define ADV_TURN_TO_MOVE                10
#define DOUBLED_PAWN_MALUS             -20
#define ISOLATED_PAWN_MALUS            -15
#define DOUBLED_PAWN_CASTLE_MALUS      -25
#define MISSING_PAWN_CASTLE_MALUS      -55
#define HOLE_C3_C6_F3_F6               -30
#define HOLE_B3_B6_G3_G6               -30
#define TRAPPED_ROOK_PENALTY           -70

/* Arrays for scaling mobility values */
int mobRook[16] = {-6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20};
int mobKnight[9] = {-10, -4, 2, 8, 14, 18, 22, 24, 25};
int mobBishop[16] = {-5, -2, 0, 3, 6, 10, 14, 20, 24, 28, 30, 32, 33, 35, 35, 35};
int rangeBishop[16] = {-6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20};

/* For scaling passed pawns depending on the row */
int passedPawnBonus[2][7] = {{0, 70, 65, 45, 25, 17, 10}, {0, 10, 17, 25, 45, 65, 70}};
//int passedPawnBonus[2][8] = {{0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}};

/* For storing pawns' ranks */
int pawnsRanks[2][10];

/* For scaling pawn number in fun factor */
int numPawnsFunfac[16] = {0, 0, 0, 0, 0, 0, 0, 0, -5, -5, -10, -10, -12, -12, -15, -15};

/* Kings' safety */
int colKing[2] = {0};
int sqKing[2] = {0};

/* Shleter pawns matrix */
int wPawnShelter[7][7] = {
                          {-8, -7, -6, -5, -5, -5, -4},
                          {-7, -999, -5, -4, -3, -1, 0},
                          {-6, -6, -999, -7, -5, -3, -2},
                          {-5, -5, -6, -999, -6, -5, -4},
                          {-5, -5, -8, -8, -999, -7, -6},
                          {-5, -5, -10, -10, -10, -999, -8},
                          {-5, -5, -12, -12, -12, -12, -999}
                           };
int bPawnShelter[7][7] = {
                          {-8, -7, -6, -5, -5, -5, -5},
                          {-7, -999, -6, -5, -5, -5, -5},
                          {-6, -5, -999, -6, -8, -10, -12},
                          {-5, -4, -7, -999, -8, -10, -12},
                          {-5, -3, -5, -6, -999, -10, -12},
                          {-5, -1, -3, -5, -7, -999, -12},
                          {-4, 0, -2, -4, -6, -8, -999}
                           };


/* To count the material */
int pawns[2] = {0};
int knights[2] = {0};
int bishops[2] = {0};
int rooks[2] = {0};
int queens[2] = {0};

/* The evaluation function */
int eval(int alpha, int beta)
{
    /* A traditional counter */
    int i;

    /* Setting some values to 0 */

    /* Pawn's info */
    for (i = 0; i < 10; ++i)
    {
        pawnsRanks[WHITE][i] = 0;
        pawnsRanks[BLACK][i] = 7;
    }

    /* The fun factor */
//    int ff = 0;

    /* The vars for counting the material */
    pawns[WHITE] = 0;
    knights[WHITE] = 0;
    bishops[WHITE] = 0;
    rooks[WHITE] = 0;
    queens[WHITE] = 0;
    pawns[BLACK] = 0;
    knights[BLACK] = 0;
    bishops[BLACK] = 0;
    rooks[BLACK] = 0;
    queens[BLACK] = 0;

    countEvaluations++;

    /* The score of the position */
    int score = 0;

    for (i = 0; i < 64; ++i)
    {
        if (color[i] == WHITE)
        {
            switch(piece[i])
            {
            case PAWN:
                pawns[WHITE]++;
                getWhitePawnRank(i);
                break;
            case KNIGHT:
                knights[WHITE]++;
                break;
            case BISHOP:
                bishops[WHITE]++;
                break;
            case ROOK:
                rooks[WHITE]++;
                break;
            case QUEEN:
                queens[WHITE]++;
                break;
            case KING:
                sqKing[WHITE] = i;
                break;
            }
        }
        else if (color[i] == BLACK)
        {
            switch(piece[i])
            {
            case PAWN:
                pawns[BLACK]++;
                getBlackPawnRank(i);
                break;
            case KNIGHT:
                knights[BLACK]++;
                break;
            case BISHOP:
                bishops[BLACK]++;
                break;
            case ROOK:
                rooks[BLACK]++;
                break;
            case QUEEN:
                queens[BLACK]++;
                break;
            case KING:
                sqKing[BLACK] = i;
            }
        }
    }

//    printf("\nBlack pawns: ");
//    for (i=0; i<10; ++i)
//    {
//        printf("%4d", pawns[BLACK]Ranks[i]);
//    }
//    puts("");

//    printf ("White pawns: ");
//    for (i=0; i<10; ++i)
//    {
//        printf("%4d", pawns[WHITE]Ranks[i]);
//    }
//    puts("");


    /* After counting the material we update the score */
    score = (pawns[WHITE]   - pawns[BLACK])   *  valuePiece[PAWN] +
            (knights[WHITE] - knights[BLACK]) *  valuePiece[KNIGHT] +
            (bishops[WHITE] - bishops[BLACK]) *  valuePiece[BISHOP] +
            (rooks[WHITE]   - rooks[BLACK])   *  valuePiece[ROOK] +
            (queens[WHITE]  - queens[BLACK])  *  valuePiece[QUEEN];

    /* Is there enough material to keep on playing? */
    if (noMaterial()) return 0;

    /* Does Anyone have the pair of bishops? */
    if (bishops[WHITE] == 2 && bishops[BLACK] != 2)
        score += PAIR_BISHOPS;
    else if (bishops[WHITE] != 2 && bishops[BLACK] == 2)
        score -= PAIR_BISHOPS;

    /* Trying the lazy eval */
    if (!isEndGame())
        {
        int lazy = score;
        if (side == BLACK) lazy = -lazy;
        if ( ( lazy + 300 < alpha ) ||
             ( lazy - 300 > beta  ) )
        {
            return lazy;
        }
    }

    /* Check all the squares searching for the pieces */
    for (i = 0; i < 64; i++)
    {
        if (color[i] == WHITE)
        {
            switch (piece[i])
            {
            case PAWN:
                if (isDoubledPawnWhite(i))
                    score += DOUBLED_PAWN_MALUS;
                if (isPassedPawnWhite(i))
                    score += passedPawnBonus[WHITE][ROW(i)];
                if (isIsolatedPawnWhite(i))
                    score += ISOLATED_PAWN_MALUS;
                if (isEndGame())
                    score += pstPawnEndgame[i];
                else
                    score += pstPawnMidgame[i];
                break;
            case KNIGHT:
                score += pstKnight[i];
                score += mobKnight[knightMobility(i)];
                break;
            case BISHOP:
                score += pstBishop[i];
                score += mobBishop[bishopMobility(i)];
                score += rangeBishop[bishopRange(i)];
                break;
            case ROOK:
                score += pstRook[i];
                score += mobRook[rookMobility(i)];
                if (isOnAnOpenCol(i))
                    score += ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H1 || i == G1)
                {
                    if(COL(sqKing[WHITE]) > COLE && COL(sqKing[WHITE]) < COLH)
                        score += TRAPPED_ROOK_PENALTY;
                }
                if (i >= A1 && i <= C1)
                {
                    if (COL(sqKing[WHITE]) <= COLD)
                        score += TRAPPED_ROOK_PENALTY;
                }
                break;
            case QUEEN:
                score += pstQueen[i];
                break;
            case KING:
                if (isEndGame())
                    score += pstKingEndgame[i];
                else
                {
//                    score += wKingShelter(i);
                    score += pstKingMidgame[i];
                }
                break;
            }
        }
        /* Now the evaluation for black: note the change of
           the sign in the score */
        else
        {
            switch (piece[i])
            {
            case PAWN:
                if (isDoubledPawnBlack(i))
                    score -= DOUBLED_PAWN_MALUS;
                if (isPassedPawnBlack(i))
                    score -= passedPawnBonus[BLACK][ROW(i)];
                if (isIsolatedPawnBlack(i))
                    score -= ISOLATED_PAWN_MALUS;
                if (isEndGame())
                    score -= pstPawnEndgame[flip[i]];
                else
                    score -= pstPawnMidgame[flip[i]];
                break;
            case KNIGHT:
                score -= pstKnight[flip[i]];
                score -= mobKnight[knightMobility(i)];
                break;
            case BISHOP:
                score -= pstBishop[flip[i]];
                score -= mobBishop[bishopMobility(i)];
                score -= rangeBishop[bishopRange(i)];
                break;
            case ROOK:
                score -= pstRook[flip[i]];
                score -= mobRook[rookMobility(i)];
                if (isOnAnOpenCol(i))
                    score -= ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H8 || i == G8)
                {
                    if(COL(sqKing[BLACK]) > COLE && COL(sqKing[BLACK]) < COLH)
                        score -= TRAPPED_ROOK_PENALTY;
                }
                if(i >= A8 && i <= C8)
                {
                    if(COL(sqKing[BLACK]) <= D8)
                        score -= TRAPPED_ROOK_PENALTY;
                }
                break;
            case QUEEN:
                score -= pstQueen[flip[i]];
                break;
            case KING:
                if (isEndGame())
                    score -= pstKingEndgame[flip[i]];
                else
                {
//                    score -= bKingShelter(i);
                    score -= pstKingMidgame[flip[i]];
                }
                break;
            }
        }
    }

    /* Finally we return the score, taking into account the side to move
        We add an extra plus because in the same position the side to
        move has some extra advantage */

//    ff = funFactor();

//    if (side == computerSide)
//    {
//        if (side == WHITE)
//            return (score + ff + ADV_TURN_TO_MOVE);
//        return (-score - ff - ADV_TURN_TO_MOVE);
//    }
//    else
//    {
//        if (side == WHITE)
//            return (score + ADV_TURN_TO_MOVE);
//        return (-score - ADV_TURN_TO_MOVE);
//    }

    if (side == WHITE)
        return (score );
    return -score;
}

/*
 *Kings safety
*/
int wKingShelter(int sq)
{
    int i;
    int shelter = 0;

    for (i=0; i<10; ++i)
    {
        if (pawnsRanks[BLACK][i] == 7)
            pawnsRanks[BLACK][i] = 0;
    }

    shelter += wPawnShelter[pawnsRanks[BLACK][COL(sq)]][pawnsRanks[WHITE][COL(sq)]];
//    printf("pawnsRanks[BLACK][COL(sq)], pawnsRanks[WHITE][COL(sq)]] %d - %d\n", pawnsRanks[BLACK][COL(sq)], pawnsRanks[WHITE][COL(sq)]);
//    printf ("Wshelter1 = %d \n", wPawnShelter[pawnsRanks[BLACK][COL(sq)]][pawnsRanks[WHITE][COL(sq)]]);

    shelter += wPawnShelter[pawnsRanks[BLACK][COL(sq) + 1]][pawnsRanks[WHITE][COL(sq) + 1]];
//    printf("pawnsRanks[BLACK][COL(sq)+1], pawnsRanks[WHITE][COL(sq)]+1] %d - %d\n", pawnsRanks[BLACK][COL(sq)+1], pawnsRanks[WHITE][COL(sq)]+1);
//    printf ("Wshelter2 = %d \n", wPawnShelter[pawnsRanks[BLACK][COL(sq)+1]][pawnsRanks[WHITE][COL(sq)+1]]);

    shelter += wPawnShelter[pawnsRanks[BLACK][COL(sq) + 2]][pawnsRanks[WHITE][COL(sq) + 2]];
//    printf ("Wshelter3 = %d \n", wPawnShelter[pawnsRanks[BLACK][COL(sq) + 2]][pawnsRanks[WHITE][COL(sq) + 2]]);


    return shelter;
}
int bKingShelter(int sq)
{
    int i;
    int shelter = 0;

    for (i=0; i<10; ++i)
    {
        if (pawnsRanks[BLACK][i] == 7)
            pawnsRanks[BLACK][i] = 0;
    }

    shelter += bPawnShelter[pawnsRanks[BLACK][COL(sq) - 1]][pawnsRanks[WHITE][COL(sq) - 1]];
    shelter += bPawnShelter[pawnsRanks[BLACK][COL(sq)]][pawnsRanks[WHITE][COL(sq)]];
    shelter += bPawnShelter[pawnsRanks[BLACK][COL(sq) + 1]][pawnsRanks[WHITE][COL(sq) + 1]];

//    printf ("Bshelter = %d \n", shelter);

    return shelter;
}

/*
 * Pawns info
 */

void getWhitePawnRank(int sq)
{
int tmpCol = COL(sq) + 1;
    if (ROW(sq) > pawnsRanks[WHITE][tmpCol])
    {
        pawnsRanks[WHITE][tmpCol] = ROW(sq);
    }
}

void getBlackPawnRank(int sq)
{
int tmpCol = COL(sq) + 1;
    if (ROW(sq) < pawnsRanks[BLACK][tmpCol])
    {
        pawnsRanks[BLACK][tmpCol] = ROW(sq);
    }
}

/* Passed pawns */
int isPassedPawnWhite(int sq)
{
    int tmpCol = COL(sq) + 1;
    if (pawnsRanks[BLACK][tmpCol] > ROW(sq))
    {
        if (pawnsRanks[BLACK][tmpCol - 1] >= ROW(sq) &&
            pawnsRanks[BLACK][tmpCol + 1] >= ROW(sq))
            return 1;
    }
    return 0;
}
int isPassedPawnBlack(int sq)
{
    int tmpCol = COL(sq) + 1;
    if (pawnsRanks[WHITE][tmpCol] < ROW(sq))
    {
        if (pawnsRanks[WHITE][tmpCol - 1] <= ROW(sq) &&
            pawnsRanks[WHITE][tmpCol + 1] <= ROW(sq))
            return 1;
    }
    return 0;
}

/* Doubled pawns */
int isDoubledPawnWhite(int sq)
{
    int tmpCol = COL(sq) + 1;
    return (pawnsRanks[WHITE][tmpCol] > ROW(sq));
}
int isDoubledPawnBlack(int sq)
{
    int tmpCol = COL(sq) + 1;
    return (pawnsRanks[BLACK][tmpCol] < ROW(sq));
}

/* Isolated pawns */
int isIsolatedPawnWhite(int sq)
{
    int tmpCol = COL(sq) + 1;
    return (pawnsRanks[WHITE][tmpCol + 1] == 0 &&
            pawnsRanks[WHITE][tmpCol - 1] == 0);
}
int isIsolatedPawnBlack(int sq)
{
    int tmpCol = COL(sq) + 1;
    return (pawnsRanks[BLACK][tmpCol + 1] == 7 &&
            pawnsRanks[BLACK][tmpCol - 1] == 7);
}

/* Returns 1 if we're in the endgame, 0 otherwise */
int isEndGame() {
//    if (queens[WHITE]==0 && queens[BLACK]==0)
//        return 1;
    if((knights[WHITE] + knights[BLACK] + bishops[WHITE]
        + bishops[BLACK] + rooks[WHITE] + rooks[BLACK]
        + queens[WHITE] + queens[BLACK]) < 4 )
        return 1;

    return 0;
}

/* Returns 1 if sq is on an open col */
inline int isOnAnOpenCol(int sq)
{
    return (pawnsRanks[WHITE][COL(sq) + 1] == 0 && pawnsRanks[BLACK][COL(sq) + 1] == 7);
}

/*
 * MOBILITY
 */

/* Mobility of the bishop: number of empty squares a bishop can reach
 * from its current position */
int bishopMobility(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-9; ((l >= 0) && COL(l) < COL(sq) && piece[l] == EMPTY); l-=9)
        mob++;
    for (l = sq-7; ((l >= 0) && COL(l) > COL(sq) && piece[l] == EMPTY); l-=7)
        mob++;
    for (l = sq+7; ((l <= 63) && COL(l) < COL(sq) && piece[l] == EMPTY); l+=7)
        mob++;
    for (l = sq+9; ((l <= 63) && COL(l) > COL(sq) && piece[l] == EMPTY); l+=9)
        mob++;

    //if (mob > 16) printf("bishop mob too large: %d", mob);

    return mob;
}

/* Range of the bishop: The squares untill reaching a pawn no matter its color */
int bishopRange(int sq)
{
    int l;
    int range = 0;

    for (l = sq-9; ((l >= 0) && COL(l) < COL(sq) && piece[l] != PAWN); l-=9)
        range++;
    for (l = sq-7; ((l >= 0) && COL(l) > COL(sq) && piece[l] != PAWN); l-=7)
        range++;
    for (l = sq+7; ((l <= 63) && COL(l) < COL(sq) && piece[l] != PAWN); l+=7)
        range++;
    for (l = sq+9; ((l <= 63) && COL(l) > COL(sq) && piece[l] != PAWN); l+=9)
        range++;

    //if (range > 16) printf("bishop range too large: %d", range);

    return range;
}

int knightMobility(int sq)
{
    int dest;
    int mob = 0;

    dest = sq - 17;
    if (dest >= 0 && COL(dest) < COL(sq) && piece[dest] == EMPTY) mob++;
    dest = sq - 15;
    if (dest >= 0 && COL(dest) > COL(sq) && piece[dest] == EMPTY) mob++;

    dest = sq - 10;
    if (dest >= 0 && COL(dest) < COL(sq) && piece[dest] == EMPTY) mob++;
    dest = sq - 6;
    if (dest >= 0 && COL(dest) > COL(sq) && piece[dest] == EMPTY) mob++;

    dest = sq + 6;
    if (dest <= 63 && COL(dest) < COL(sq) && piece[dest] == EMPTY) mob++;
    dest = sq + 10;
    if (dest <= 63 && COL(dest) > COL(sq) && piece[dest] == EMPTY) mob++;

    dest = sq + 15;
    if (dest <= 63 && COL(dest) < COL(sq) && piece[dest] == EMPTY) mob++;
    dest = sq + 17;
    if (dest <= 63 && COL(dest) > COL(sq) && piece[dest] == EMPTY) mob++;

    //if (mob > 8) printf("knight mob too large: %d", mob);

    return mob;
}

int rookMobility(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-8; ((l >= 0) && piece[l] == EMPTY); l-=8)
        mob++;
    for (l = sq-1; ((ROW(l) == ROW(sq)) && piece[l] == EMPTY); l-=1)
        mob++;
    for (l = sq+8; ((l <= 63) && piece[l] == EMPTY); l+=8)
        mob++;
    for (l = sq+1; ((ROW(l) == ROW(sq)) && piece[l] == EMPTY); l+=1)
        mob++;

    return mob;
}

/* Returns 1 if no enough material on the board */
int noMaterial()
{
    if (pawns[WHITE] == 0 && pawns[BLACK] == 0)
        if (rooks[WHITE] == 0 && rooks[BLACK] == 0)
            if (queens[WHITE] == 0 && queens[BLACK] == 0)
                if (bishops[WHITE] <= 1 && bishops[BLACK] <= 1)
                    if (knights[WHITE] <= 1 && knights[BLACK] <= 1)
                        if (knights[WHITE] == 0 || bishops[WHITE] == 0)
                            if (knights[BLACK] == 0 || bishops[BLACK] == 0)
                                return 1;
    return 0;
}

/*
 *  The fun factor: the main idea is to encourage interesting positions:
 *  -Queens on the board
 *  -Not too much pawns, to give place to open positions
 *  -Unbalanced material
 *  -...
 */
//int funFactor()
//{
//    int funfa = 0;

//    /* If we aren't in the endgame we like opposite castles */
//    if ( !isEndGame() && (abs(colWhiteKing - colBlackKing) > 4) )
//        funfa += 10;
//    /* We like queens on the board */
//    if (queens[WHITE] >= 1 || queens[BLACK] >= 1)
//        funfa += 10;
//    /* Taking into account the number of pawns on the board */
//    funfa += num_pawns_funfac[pawns[WHITE] + pawns[BLACK]];
//    /* No queens at all? That doesn't rule */
////    if (queens[WHITE] == 0 && queens[BLACK] == 0)
////        funfa -= 10;
//    /* Encouraging the exchange (I hope) */
//    if (rooks[WHITE] != rooks[BLACK])
//        funfa += 10;
//    /* Unbalanced material is welcome */
//    if (queens[WHITE] != queens[BLACK])
//        funfa += 10;
//    if ( (bishops[WHITE] + whiteKnights) != (bishops[BLACK] + knights[BLACK]) )
//        funfa += 10;

//    return funfa;
//}

/*
 * TESTS FUNCTIONS
 */
void testWhitePassedPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == WHITE)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isPassedPawnWhite(i))
                printf ("White passed pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void testBlackPassedPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isPassedPawnBlack(i))
                printf ("Black passed pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void testWhiteDoubledPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == WHITE)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isDoubledPawnWhite(i))
                printf ("White doubled pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void testBlackDoubledPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isDoubledPawnBlack(i))
                printf ("Black doubled pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void testIsIsolatedPawnWhite()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == WHITE)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isIsolatedPawnWhite(i))
                printf ("White isolated pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void testIsIsolatedPawnBlack()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isIsolatedPawnBlack(i))
                printf ("Black isolated pawn in %d, 1<<ROW(sq) is %d\n", i, 1<<ROW(i));
        }
    }
}

void showPawnsInfo()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<10; ++j)
            {
                pawnsRanks[WHITE][j] = 0;
                pawnsRanks[BLACK][j] = 7;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
            }
        }
    }

    for (j=0; j<10; ++j)
    {
        printf ("%d ", pawnsRanks[1][j]);
    }
    puts("");
    for (j=0; j<10; ++j)
    {
        printf ("%d ", pawnsRanks[0][j]);
    }
    puts("");

}

void testOpenCols()
{
    int i = 0, j=0;

    for (j = 0; j < 10; ++j)
    {
        pawnsRanks[WHITE][j] = 0;
        pawnsRanks[BLACK][j] = 7;
    }

    for (i = 0; i < 64; i++)
    {
        if (piece[i] == PAWN)
        {
             for (j = 0; j < 64; ++j)
            {
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
                else if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
            }
        }
    }

    for (i = 0; i < 64; i++)
    {
    if (isOnAnOpenCol(i))
        printf ("Open col in square %d, column %d\n", i, COL(i));
    }
}
