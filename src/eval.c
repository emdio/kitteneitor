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
#define DOUBLED_PAWN_MALUS             -10
#define DOUBLED_PAWN_CASTLE_MALUS      -25
#define MISSING_PAWN_CASTLE_MALUS      -55
#define HOLE_C3_C6_F3_F6               -30
#define HOLE_B3_B6_G3_G6               -30
#define TRAPPED_ROOK_PENALTY           -70

/* Arrays for scaling mobility values */
int mobRook[16] = {-6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20};
int mobKnight[9] = {-10, -4, 2, 8, 14, 18, 22, 24, 25};
int mobBishop[16] = {-5, -2, 0, 3, 6, 10, 14, 20, 24, 28, 31, 35, 40, 42, 45, 47};
int rangeBishop[16] = {-6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20};

/* For scaling passed pawns depending on the row */
int passedPawnBonus[2][7] = {{90, 75, 55, 25, 17, 10, 0}, {0, 10, 17, 25, 55, 75, 90}};

/* For storing pawns' ranks */
int pawnsRanks[2][10];

/* For scaling pawn number in fun factor */
int numPawnsFunfac[16] = {0, 0, 0, 0, 0, 0, 0, 0, -5, -5, -10, -10, -12, -12, -15, -15};

/* Kings' safety */
int colKing[2] = {0};
int sqKing[2] = {0};

/* To count the material */
int pawns[2] = {0};
int knights[2] = {0};
int bishops[2] = {0};
int rooks[2] = {0};
int queens[2] = {0};

/* The evaluation function */
int Eval(alpha, beta)
{
    /* A traditional counter */
    int i;

    /* Set some values to 0 */

    /* Pawn's info */
    for (i = 0; i < 10; ++i)
    {
        pawnsRanks[WHITE][i] = 0;
        pawnsRanks[BLACK][i] = 7;
    }

    /* The fun factor */
    int ff = 0;

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

    count_evaluations++;

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
//    score = piece_mat[WHITE] - piece_mat[BLACK];
    score = (pawns[WHITE]   - pawns[BLACK])   *  value_piece[PAWN] +
            (knights[WHITE] - knights[BLACK]) *  value_piece[KNIGHT] +
            (bishops[WHITE] - bishops[BLACK]) *  value_piece[BISHOP] +
            (rooks[WHITE]   - rooks[BLACK])   *  value_piece[ROOK] +
            (queens[WHITE]  - queens[BLACK])  *  value_piece[QUEEN];

    /* Is there enough material to keep on playing? */
    if (NoMaterial()) return 0;

    /* Anyone has the pair of bishops? */
    if (bishops[WHITE] == 2 && bishops[BLACK] != 2)
        score += PAIR_BISHOPS;
    else if (bishops[WHITE] != 2 && bishops[BLACK] == 2)
        score -= PAIR_BISHOPS;

    /* Trying the lazy eval */
    int lazy = score;
    if (side == BLACK) lazy = -lazy;
    if ( ( lazy + 300 < alpha ) ||
         ( lazy - 300 > beta  ) )
    {
        return lazy;
    }

    /* Check all the squares searching for the pieces */
    for (i = 0; i < 64; i++)
    {
        if (color[i] == WHITE)
        {
            /* Now we add to the evaluation the value of the
             * piece square tables */
            switch (piece[i])
            {
            case PAWN:
                if (isDoubledPawnWhite(i))
                    score += DOUBLED_PAWN_MALUS;
                if (isPassedPawnWhite(i))
                    score += passedPawnBonus[WHITE][Row(i)];
                if (endGame())
                    score += pst_pawn_endgame[i];
                else
                    score += pst_pawn_midgame[i];
                break;
            case KNIGHT:
                score += pst_knight[i];
                score += mobKnight[KnightMobility(i)];
                break;
            case BISHOP:
                score += pst_bishop[i];
                score += mobBishop[BishopMobility(i)];
                score += rangeBishop[BishopRange(i)];
                break;
            case ROOK:
                score += pst_rook[i];
                score += mobRook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score += ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H1 || i == G1)
                {
                    if(sqKing[WHITE] > E1 && sqKing[WHITE] < H1)
                        score += TRAPPED_ROOK_PENALTY;
                }
                if (i >= A1 && i <= C1)
                {
                    if (sqKing[WHITE] <= D1)
                        score += TRAPPED_ROOK_PENALTY;
                }
                break;
            case QUEEN:
                score += pst_queen[i];
                break;
            case KING:
                if (endGame())
                    score += pst_king_endgame[i];
                else
                {
//                    score += whiteKingSafety();
                    score += pst_king_midgame[i];
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
                    score -= passedPawnBonus[BLACK][Row(i)];
                if (endGame())
                    score -= pst_pawn_endgame[flip[i]];
                else
                    score -= pst_pawn_midgame[flip[i]];
                break;
            case KNIGHT:
                score -= pst_knight[flip[i]];
                score -= mobKnight[KnightMobility(i)];
                break;
            case BISHOP:
                score -= pst_bishop[flip[i]];
                score -= mobBishop[BishopMobility(i)];
                score -= rangeBishop[BishopRange(i)];
                break;
            case ROOK:
                score -= pst_rook[flip[i]];
                score -= mobRook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score -= ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H8 || i == G8)
                {
                    if(sqKing[BLACK] > COLE && sqKing[BLACK] < COLH)
                        score -= TRAPPED_ROOK_PENALTY;
                }
                if(i >= A8 && i <= C8)
                {
                    if(sqKing[BLACK] <= D8)
                        score -= TRAPPED_ROOK_PENALTY;
                }
                break;
            case QUEEN:
                score -= pst_queen[flip[i]];
                break;
            case KING:
                if (endGame())
                    score -= pst_king_endgame[flip[i]];
                else
                {
//                    score += blackKingSafety();
                    score -= pst_king_midgame[flip[i]];
                }
                break;
            }
        }
    }

    /* Finally we return the score, taking into account the side to move
        We add an extra plus because in the same position the side to
        move has some extra advantage*/

//    ff = funFactor();

//    if (side == computer_side)
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
 * Pawns info
 */

void getWhitePawnRank(sq)
{
int tmpCol = Col(sq) + 1;
    if (Row(sq) > pawnsRanks[WHITE][tmpCol])
    {
        pawnsRanks[WHITE][tmpCol] = Row(sq);
    }
}

void getBlackPawnRank(sq)
{
int tmpCol = Col(sq) + 1;
    if (Row(sq) < pawnsRanks[BLACK][tmpCol])
    {
        pawnsRanks[BLACK][tmpCol] = Row(sq);
    }
}

/* Passed pawns */
int isPassedPawnWhite(sq)
{
    int tmpCol = Col(sq) + 1;
    if (pawnsRanks[BLACK][tmpCol] > Row(sq))
    {
        if (pawnsRanks[BLACK][tmpCol - 1] >= Row(sq) &&
            pawnsRanks[BLACK][tmpCol + 1] >= Row(sq))
            return 1;
    }
    return 0;
}
int isPassedPawnBlack(sq)
{
    int tmpCol = Col(sq) + 1;
    if (pawnsRanks[WHITE][tmpCol] < Row(sq))
    {
        if (pawnsRanks[WHITE][tmpCol - 1] <= Row(sq) &&
            pawnsRanks[WHITE][tmpCol + 1] <= Row(sq))
            return 1;
    }
    return 0;
}

/* Doubled pawns */
int isDoubledPawnWhite(sq)
{
    int tmpCol = Col(sq) + 1;
    return (pawnsRanks[WHITE][tmpCol] > Row(sq));
}
int isDoubledPawnBlack(sq)
{
    int tmpCol = Col(sq) + 1;
    return (pawnsRanks[BLACK][tmpCol] < Row(sq));
}

/* Are we in the endgame? */
inline int endGame()
{
    if (queens[WHITE]==0 || queens[BLACK]==0)
        return 1;
    if (pawns[WHITE] + pawns[BLACK] < 8)
        return 1;
    return 0;
}

/* Returns 1 if sq is on an open col */
inline int isOnAnOpenCol(int sq)
{
    return (pawnsRanks[WHITE][Col(sq) + 1] == 0 && pawnsRanks[BLACK][Col(sq) + 1] == 7);
}

/*
 * MOBILITY
 */

/* Mobility of the bishop: number of empty squares a bishop can reach
 * from its actual position */
int BishopMobility(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-9; ((l >= 0) && Col(l) < Col(sq) && piece[l] == EMPTY); l-=9)
        mob++;
    for (l = sq-7; ((l >= 0) && Col(l) > Col(sq) && piece[l] == EMPTY); l-=7)
        mob++;
    for (l = sq+7; ((l <= 63) && Col(l) < Col(sq) && piece[l] == EMPTY); l+=7)
        mob++;
    for (l = sq+9; ((l <= 63) && Col(l) > Col(sq) && piece[l] == EMPTY); l+=9)
        mob++;

    return mob;
}

/* Range of the bishop: The squares till reach a pawn no matter its color */
int BishopRange(int sq)
{
    int l;
    int range = 0;

    for (l = sq-9; ((l >= 0) && Col(l) < Col(sq) && piece[l] != PAWN); l-=9)
        range++;
    for (l = sq-7; ((l >= 0) && Col(l) > Col(sq) && piece[l] != PAWN); l-=7)
        range++;
    for (l = sq+7; ((l <= 63) && Col(l) < Col(sq) && piece[l] != PAWN); l+=7)
        range++;
    for (l = sq+9; ((l <= 63) && Col(l) > Col(sq) && piece[l] != PAWN); l+=9)
        range++;

    return range;
}

int KnightMobility(int sq)
{
    int l;
    int mob = 0;

    l = sq - 17;
    if (l >= 0 && Col(l) < Col(sq) && piece[l] == EMPTY) mob++;
    l = sq - 15;
    if (l >= 0 && Col(l) > Col(sq) && piece[l] == EMPTY) mob++;

    l = sq - 10;
    if (l >= 0 && Col(l) < Col(sq) && piece[l] == EMPTY) mob++;
    l = sq - 6;
    if (l >= 0 && Col(l) > Col(sq) && piece[l] == EMPTY) mob++;

    l = sq + 6;
    if (l <= 63 && Col(l) < Col(sq) && piece[l] == EMPTY) mob++;
    l = sq + 10;
    if (l <= 63 && Col(l) > Col(sq) && piece[l] == EMPTY) mob++;

    l = sq + 15;
    if (l <= 63 && Col(l) < Col(sq) && piece[l] == EMPTY) mob++;
    l = sq + 17;
    if (l <= 63 && Col(l) > Col(sq) && piece[l] == EMPTY) mob++;

    return mob;
}

int RookMobility(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-8; ((l >= 0) && piece[l] == EMPTY); l-=8)
        mob++;
    for (l = sq-1; ((Row(l) == Row(sq)) && piece[l] == EMPTY); l-=1)
        mob++;
    for (l = sq+8; ((l <= 63) && piece[l] == EMPTY); l+=8)
        mob++;
    for (l = sq+1; ((Row(l) == Row(sq)) && piece[l] == EMPTY); l+=1)
        mob++;

    return mob;
}

/* Returns 1 if no enough material on the board */
int NoMaterial()
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
//    if ( !endGame() && (abs(colWhiteKing - colBlackKing) > 4) )
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
                printf ("White passed pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
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
                printf ("Black passed pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
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
                printf ("White doubled pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
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
                printf ("Black doubled pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
        }
    }
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
        printf ("Open col in square %d, column %d\n", i, Col(i));
    }
}
