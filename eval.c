#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

//#define NDEBUG
//#include <assert.h>

/*
 ****************************************************************************
 * Evaluation for current position - main "brain" function *
 * Lack: almost no knowlegde; material value + piece square tables *
 ****************************************************************************
 */

/* Bonus and malus */
#define	ROOK_OPEN_COL               25
#define PAIR_BISHOPS                15
#define ADV_TURN_TO_MOVE            20
#define DOUBLED_PAWN_MALUS          15
#define DOUBLED_PAWN_CASTLE_MALUS   25
#define MISSING_PAWN_CASTLE_MALUS   30

/* Arrays for scaling mobility values */
int mob_rook[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};
int mob_knight[9] = {
    -10, -4, 2, 8, 14, 18, 22, 24, 25
};
int mob_bishop[16] = {
    -5, -2, 0, 3, 6, 10, 14, 20, 24, 28, 31, 35, 40, 42, 45, 47
};
int range_bishop[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};
/* For scaling passed pawns depending on the row */
int passed_pawn[6] = {10, 12, 15, 25, 35, 50};

/* Kings' safety */
int posWhiteKing = 0;
int colWhiteKing = 0;
int posBlackKing = 0;
int colBlackKing = 0;
/* To scale pawns shield for king */
int pawnsShieldScale[4] = {0, -5, -15, -40};

/* To count the material */
int whitePawns = 0;
int whiteKnights = 0;
int whiteBishops = 0;
int whiteRooks = 0;
int whiteQueens = 0;
int blackPawns = 0;
int blackKnights = 0;
int blackBishops = 0;
int blackRooks = 0;
int blackQueens = 0;


/* Pawn's info */
int whitePawnsInfo[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int blackPawnsInfo[8] = {0, 0, 0, 0, 0, 0, 0, 0};


/* The evaluation function */
int
Eval (alpha, beta)
{
    /* A traditional counter */
    int i;

    /* Set some values to 0 */
    /* Pawn's info */
    for (i=0; i<8; ++i)
    {
        whitePawnsInfo[i] = 0;
        blackPawnsInfo[i] = 0;
    }



    /* The fun factor */
    int ff = 0;

    /* The vars for counting the material */
    whitePawns = 0;
    whiteKnights = 0;
    whiteBishops = 0;
    whiteRooks = 0;
    whiteQueens = 0;
    blackPawns = 0;
    blackKnights = 0;
    blackBishops = 0;
    blackRooks = 0;
    blackQueens = 0;


    count_evaluations++;


    /* The score of the position */
    int score = 0;

    for (i = 0; i < 64; ++i)
    {
//        if (color[i] == EMPTY)
//            continue;
//        else
//        {
        /* Just counting the wood on the board */
//            piece_mat[color[i]] += value_piece[piece[i]];
        if (color[i] == WHITE)
        {
            switch(piece[i])
            {
            case PAWN:
                whitePawns++;
                whitePawnsInfo[(int)Col(i)] += 1<<Row(i);
                break;
            case KNIGHT:
                whiteKnights++;
                break;
            case BISHOP:
                whiteBishops++;
                break;
            case ROOK:
                whiteRooks++;
                break;
            case QUEEN:
                whiteQueens++;
                break;
            case KING:
                posWhiteKing = i;
                colWhiteKing = Col(i);
                break;
            }
        }
        else if (color[i] == BLACK)
        {
            switch(piece[i])
            {
            case PAWN:
                blackPawns++;
                blackPawnsInfo[(int)Col(i)] += 1<<Row(i);
                break;
            case KNIGHT:
                blackKnights++;
                break;
            case BISHOP:
                blackBishops++;
                break;
            case ROOK:
                blackRooks++;
                break;
            case QUEEN:
                blackQueens++;
                break;
            case KING:
                posBlackKing = i;
                colBlackKing = Col(i);
                break;
            }
        }
    }

//    printf("\nBlack pawns: ");
//    for (i=0; i<8; ++i)
//    {
//        printf("%4d", blackPawnsInfo[i]);
//    }
//    puts("");

//    printf ("White pawns: ");
//    for (i=0; i<8; ++i)
//    {
//        printf("%4d", whitePawnsInfo[i]);
//    }
//    puts("");


    /* After counting the material we update the score */
//    score = piece_mat[WHITE] - piece_mat[BLACK];
    score = whitePawns * value_piece[PAWN] +
            whiteKnights * value_piece[KNIGHT] +
            whiteBishops * value_piece[BISHOP] +
            whiteRooks * value_piece[ROOK] +
            whiteQueens * value_piece[QUEEN] -
            blackPawns * value_piece[PAWN] -
            blackKnights * value_piece[KNIGHT] -
            blackBishops * value_piece[BISHOP] -
            blackRooks * value_piece[ROOK] -
            blackQueens * value_piece[QUEEN];

    /* Trying the lazy eval */
    int lazy = score;
    if (side == BLACK) lazy = -lazy;
    if ( ( lazy + 500 < alpha ) ||
         ( lazy - 500 > beta  ) )
    {
        return lazy;
    }

    /* Is there enough material to keep on playing? */
    if (NoMaterial()) return 0;

    /* Anyone has the pair of bishops? */
    if (whiteBishops==2 && blackBishops!=2)
        score += PAIR_BISHOPS;
    else if (blackBishops==2 && whiteBishops!=2)
        score -= PAIR_BISHOPS;

    /* Check all the squares searching for the pieces */
    for (i = 0; i < 64; i++)
    {
        if (color[i] == EMPTY)
            continue;
        if (color[i] == WHITE)
        {
            /* Now we add to the evaluation the value of the
             * piece square tables */
            switch (piece[i])
            {
            case PAWN:
                if (isDoubledPawnWhite(Col(i)))
                    score -= DOUBLED_PAWN_MALUS;
                if (isPassedPawnWhite(i))
                    score += passed_pawn[Row(i)];
                if (endGame())
                    score += pst_pawn_endgame[i];
                else
                    score += pst_pawn_midgame[i];
                break;
            case KNIGHT:
                score += pst_knight[i];
                score += nk_dist[i][posBlackKing];
                score += mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score += pst_bishop[i];
                score += bk_dist[i][posBlackKing];
                score += mob_bishop[BishopMobility(i)];
                score += range_bishop[BishopRange(i)];
                break;
            case ROOK:
                score += pst_rook[i];
                score += rk_dist[i][posBlackKing];
                score += mob_rook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score += ROOK_OPEN_COL;
                break;
            case QUEEN:
                score += pst_queen[i];
                score += qk_dist[i][posBlackKing];
                break;
            case KING:
                if (endGame())
                    score += pst_king_endgame[i];
                else
                {
                    score += whiteKingSafety(i);
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
                if (isDoubledPawnBlack(Col(i)))
                    score += DOUBLED_PAWN_MALUS;
                if (isPassedPawnBlack(i))
                    score -= passed_pawn[Row(i)];
                if (endGame())
                    score -= pst_pawn_endgame[flip[i]];
                else
                    score -= pst_pawn_midgame[flip[i]];
                break;
            case KNIGHT:
                score -= pst_knight[flip[i]];
                score -= nk_dist[i][posWhiteKing];
                score -= mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score -= pst_bishop[flip[i]];
                score -= bk_dist[i][posWhiteKing];
                score -= mob_bishop[BishopMobility(i)];
                score -= range_bishop[BishopRange(i)];
                break;
            case ROOK:
                score -= pst_rook[flip[i]];
                score -= rk_dist[i][posWhiteKing];
                score -= mob_rook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score -= ROOK_OPEN_COL;
                break;
            case QUEEN:
                score -= pst_queen[flip[i]];
                score -= qk_dist[i][posWhiteKing];
                break;
            case KING:
                if (endGame())
                    score -= pst_king_endgame[flip[i]];
                else
                {
                    score -= blackKingSafety(i);
                    score -= pst_king_midgame[flip[i]];
                }
                break;
            }
        }
    }

    /* Finally we return the score, taking into account the side to move
        We add an extra plus 10 because in the same position the side to
        move has some extra advantage*/

    ff = funFactor();
//    ff = 0;
//    printf("ff=%d\n", ff);
    if (side == computer_side)
    {
        if (side == WHITE)
            return (score + ff + ADV_TURN_TO_MOVE);
        return (-score - ff - ADV_TURN_TO_MOVE);
    }
    else
    {
        if (side == WHITE)
            return (score + ADV_TURN_TO_MOVE);
        return (-score - ADV_TURN_TO_MOVE);
    }

//    if (side == WHITE)
//        return (score );
//    return (-score );
}

/* Returns 1 if the pawn of color color in square sq is passed */
int isPassedPawnWhite(int sq)
    {
        /* Special case, pawn in A row */
        if (Col(sq) == 0)
        {
            if ( blackPawnsInfo[Col(sq)] == 0 &&
                 (1<<Row(sq) <= blackPawnsInfo[Col(sq+1)]) )
                return 1;
        }
        /* Special case, pawn in H row */
        else if (Col(sq) == 7)
        {
            if ( blackPawnsInfo[Col(sq)] == 0 &&
                 (1<<Row(sq) <= blackPawnsInfo[Col(sq-1)]) )
                return 1;
        }
        else
        {
//            printf("1<<Row %d\n", 1<<Row(sq));
//            printf("blackPawnsInfo[Col(sq)] %d\n", blackPawnsInfo[Col(sq)]);
//            printf("blackPawnsInfo[Col(sq)-1] %d\n", blackPawnsInfo[Col(sq)-1]);
//            printf("blackPawnsInfo[Col(sq)+1] %d\n", blackPawnsInfo[Col(sq)+1]);
            if ( blackPawnsInfo[Col(sq)] == 0 &&
                 (1<<Row(sq)) <= blackPawnsInfo[Col(sq)-1] &&
                 (1<<Row(sq)) <= blackPawnsInfo[Col(sq)+1] )
                return 1;
        }
        return 0;
    }

int isPassedPawnBlack(int sq)
    {
        /* Special case, pawn in A row */
        if (Col(sq) == 0)
        {
            if ( whitePawnsInfo[Col(sq)] == 0 &&
                 (whitePawnsInfo[Col(sq+1)] <= 1<<Row(sq)) )
                return 1;
        }
        /* Special case, pawn in H row */
        else if (Col(sq) == 7)
        {
            if ( whitePawnsInfo[Col(sq)] == 0 &&
                 (whitePawnsInfo[Col(sq-1)] <= 1<<Row(sq)) )
                return 1;
        }
        else
        {
            if ( whitePawnsInfo[Col(sq)] == 0 &&
                 (whitePawnsInfo[Col(sq-1)] <= 1<<Row(sq)) &&
                 (whitePawnsInfo[Col(sq+1)] <= 1<<Row(sq)) )
                return 1;
        }
        return 0;
    }

int whiteKingSafety()
{
    int safety = 0;

    /* To scale pawns shield */
    int noShield = 0;

    /* The king long castled */
    if (colWhiteKing < COLD)
    {
        if (whitePawnsInfo[COLA] < 64) noShield++;
        if (whitePawnsInfo[COLB] < 64) noShield++;
        if (whitePawnsInfo[COLC] < 64) noShield++;
        /* Doubled pawns on castle */
        if (isDoubledPawnWhite(COLA)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLB)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLC)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        /* Semiopen cols by the oponent */
        if (blackPawnsInfo[COLA] == 0) safety -= 25;
        if (blackPawnsInfo[COLB] == 0) safety -= 25;
        if (blackPawnsInfo[COLC] == 0) safety -= 25;
        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (colBlackKing > COLE)
        {
            if (whitePawnsInfo[COLA] == 0 && blackPawnsInfo[0] == 0) safety -= 35;
            if (whitePawnsInfo[COLB] == 0 && blackPawnsInfo[1] == 0) safety -= 35;
            if (whitePawnsInfo[COLC] == 0 && blackPawnsInfo[2] == 0) safety -= 35;
        }
        /* Pawns shield */
        if (whitePawnsInfo[COLA] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (whitePawnsInfo[COLB] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (whitePawnsInfo[COLC] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
    }
    /* The king short castled */
    else if (colWhiteKing > COLE)
    {
        if (whitePawnsInfo[COLF] < 64) noShield++;
        if (whitePawnsInfo[COLG] < 64) noShield++;
        if (whitePawnsInfo[COLH] < 64) noShield++;
        /* Doubled pawns on castle */
        if (isDoubledPawnWhite(COLF)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLG)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLH)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        /* Semiopen cols by the oponent */
        if (blackPawnsInfo[COLF] == 0) safety -= 25;
        if (blackPawnsInfo[COLG] == 0) safety -= 25;
        if (blackPawnsInfo[COLH] == 0) safety -= 25;
        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (colBlackKing < 4)
        {
            if (whitePawnsInfo[COLF] == 0 && blackPawnsInfo[COLF] == 0) safety -= 35;
            if (whitePawnsInfo[COLG] == 0 && blackPawnsInfo[COLG] == 0) safety -= 35;
            if (whitePawnsInfo[COLH] == 0 && blackPawnsInfo[COLH] == 0) safety -= 35;
        }
        /* Pawns shield */
        if (whitePawnsInfo[COLF] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (whitePawnsInfo[COLG] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (whitePawnsInfo[COLH] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
    }
    else
    {
        /* Open cols close to the king */
        if (whitePawnsInfo[COLD] == 0 && blackPawnsInfo[COLD] == 0) safety -= 25;
        if (whitePawnsInfo[COLE] == 0 && blackPawnsInfo[COLE] == 0) safety -= 25;
    }

    safety += pawnsShieldScale[noShield];

    return safety;
}

int blackKingSafety()
{
    int safety = 0;

    /* To scale pawns shield */
    int noShield = 0;

    /* The king long castled */
    if (colBlackKing < COLD)
    {
        if (blackPawnsInfo[COLA] > 2) noShield++;
        if (blackPawnsInfo[COLB] > 2) noShield++;
        if (blackPawnsInfo[COLC] > 2) noShield++;
        /* Doubled pawns on castle */
        if (isDoubledPawnBlack(COLA)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnBlack(COLB)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnBlack(COLC)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        /* Semiopen cols by the oponent */
        if (whitePawnsInfo[COLA] == 0) safety -= 25;
        if (whitePawnsInfo[COLB] == 0) safety -= 25;
        if (whitePawnsInfo[COLC] == 0) safety -= 25;
        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (colWhiteKing > COLD)
        {
            if (whitePawnsInfo[COLA] == 0 && blackPawnsInfo[COLA] == 0) safety -= 35;
            if (whitePawnsInfo[COLB] == 0 && blackPawnsInfo[COLB] == 0) safety -= 35;
            if (whitePawnsInfo[COLC] == 0 && blackPawnsInfo[COLC] == 0) safety -= 35;
        }
        /* Pawns shield */
        if (blackPawnsInfo[COLA] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (blackPawnsInfo[COLB] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (blackPawnsInfo[COLC] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
    }
    /* The king short castled */
    else if (colBlackKing > COLE)
    {
        if (blackPawnsInfo[COLF] > 2) noShield++;
        if (blackPawnsInfo[COLG] > 2) noShield++;
        if (blackPawnsInfo[COLH] > 2) noShield++;
        /* Doubled pawns on castle */
        if (isDoubledPawnBlack(COLF)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnBlack(COLG)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnBlack(COLH)) safety -= DOUBLED_PAWN_CASTLE_MALUS;
        /* Semiopen cols by the oponent */
        if (whitePawnsInfo[COLF] == 0) safety -= 25;
        if (whitePawnsInfo[COLG] == 0) safety -= 25;
        if (whitePawnsInfo[COLH] == 0) safety -= 25;
        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (colWhiteKing < COLE)
        {
            if (whitePawnsInfo[COLF] == 0 && blackPawnsInfo[COLF] == 0) safety -= 35;
            if (whitePawnsInfo[COLG] == 0 && blackPawnsInfo[COLG] == 0) safety -= 35;
            if (whitePawnsInfo[COLH] == 0 && blackPawnsInfo[COLH] == 0) safety -= 35;
        }
        /* Pawns shield */
        if (blackPawnsInfo[COLF] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (blackPawnsInfo[COLG] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
        if (blackPawnsInfo[COLH] == 0) safety -= MISSING_PAWN_CASTLE_MALUS;
    }
    else
    {
        /* Open cols close to the king */
        if (whitePawnsInfo[COLD] == 0 && blackPawnsInfo[3] == 0) safety -= 25;
        if (whitePawnsInfo[COLE] == 0 && blackPawnsInfo[4] == 0) safety -= 25;
    }

    safety += pawnsShieldScale[noShield];

    return safety;
}

/* Are we in the endgame? */
inline int endGame()
{
    if (whiteQueens==0 || blackQueens==0)
        return 1;
    if (whitePawns + blackPawns < 8)
        return 1;
    return 0;
}

/* Returns 0 if it sq is on an open col */
inline int isOnAnOpenCol(int sq)
{
    if (whitePawnsInfo[Col(sq)] == 0 && blackPawnsInfo[Col(sq)] == 0)
        return 1;
    return 0;
}

/* Mobility of the bishop: number of empty squares a bishop can reach 
 * from its actual position */
int BishopMobility(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-9; ((l >= 0) && Col(l) < Col(sq) && piece[l] == EMPTY); l-=9)
        mob++;
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
    if (whitePawns == 0 && blackPawns == 0)
        if (whiteRooks == 0 && blackRooks == 0)
            if (whiteQueens == 0 && blackQueens == 0)
                if (whiteBishops <= 1 && blackBishops <= 1)
                    if (whiteKnights <= 1 && blackKnights <= 1)
                        return 1;
    return 0;
}

/* Function to check if x is power of 2. It's used to find out
   doubled pawns */
inline int isDoubledPawnWhite(int col)
{
  int tmp = whitePawnsInfo[col];

  /* First x in the below expression is for the case when x is 0 */
  return !(tmp && (!(tmp & (tmp - 1))));
}
inline int isDoubledPawnBlack(int col)
{
  int tmp = blackPawnsInfo[col];

  /* First x in the below expression is for the case when x is 0 */
  return !(tmp && (!(tmp & (tmp - 1))));
}



/*
 *  The fun factor: the main idea is to encourage interesting positions:
 *  -Queens on the board
 *  -Not too much pawns, to give place to open positions
 *  -Unbalanced material
 */
int funFactor()
{
    int funfa = 0;

    /* If we aren't in the endgame we like opposite castles */
    if ( !endGame() && (abs(colWhiteKing - colBlackKing) > 4) )
        funfa += 10;
    /* We like queens on the board */
    if (whiteQueens >= 1 || blackQueens >= 1)
        funfa += 10;
    /* Too many pawns on the board aren't that funny */
    if (whitePawns + blackPawns < 12)
        funfa += 5;
    /* Too many many pawns on the board aren't that funny */
    if (whitePawns + blackPawns == 16)
        funfa -= 10;
    /* No queens at all? That doesn't rule */
//    if (whiteQueens == 0 && blackQueens == 0)
//        funfa -= 10;
    /* Encouraging the exchange (I hope) */
    if (whiteRooks != blackRooks)
        funfa += 10;
    /* Unbalanced material is welcome */
    if (whiteQueens != blackQueens)
        funfa += 10;
    if ( (whiteBishops + whiteKnights) != (blackBishops + blackKnights) )
        funfa += 10;
    
    return funfa;
}


