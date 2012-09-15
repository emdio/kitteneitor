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
#define	ROOK_OPEN_COL		30
#define PAIR_BISHOPS        20

/* To store the material of each side */
//int piece_mat[2];

/* Arrays for scaling mobility values */
int mob_knight[9] = {
  -10, -4, 2, 8, 14, 18, 22, 24, 25
};

int mob_bishop[16] = {
  -15, -10, 0, 8, 16, 20, 28, 32, 36, 40, 44, 48, 52, 55, 57, 60
};

/* To count the material */
int whitePawns;
int whiteKnights;
int whiteBishops;
int whiteRooks;
int whiteQueens;
int blackPawns;
int blackKnights;
int blackBishops;
int blackRooks;
int blackQueens;



/* The evaluation function */
int
Eval ()
{
    /* Set some values to 0 */
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

    /* A counter for the board squares */
    int i;

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
                }
            }
        else if (color[i] == BLACK)
            {
                switch(piece[i])
                {
                case PAWN:
                    blackPawns++;
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
                }
            }
        }


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
                score += pst_pawn[i];
                break;
            case KNIGHT:
                score += pst_knight[i];
                score += mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score += pst_bishop[i];
                score += mob_bishop[BishopRange(i)];
                break;
            case ROOK:
                score += pst_rook[i];
                if (OpenColRook(i))
                    score += ROOK_OPEN_COL;
                break;
            case QUEEN:
                score += pst_queen[i];
                break;
            case KING:
                if (endGame())
                    score += pst_king_endgame[i];
                else
                    score += pst_king_midgame[i];
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
                score -= pst_pawn[flip[i]];
                break;
            case KNIGHT:
                score -= pst_knight[flip[i]];
                score -= mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score -= pst_bishop[flip[i]];
                score -= mob_bishop[BishopRange(i)];
                break;
            case ROOK:
                score -= pst_rook[flip[i]];
                if (OpenColRook(i))
                    score -= ROOK_OPEN_COL;
                break;
            case QUEEN:
                score -= pst_queen[flip[i]];
                break;
            case KING:
                if (endGame())
                    score -= pst_king_endgame[flip[i]];
                else
                    score -= pst_king_midgame[flip[i]];
                break;
            }
        }
    }

    /* Finally we return the score, taking into account the side to move
        We add an extra plus 10 because in the same position the side to
        move has some extra advantage*/
    if (side == WHITE)
        return score + ff + 10;
    return -score - ff - 10;
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

/* Mobility */
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

/* The squares till reach a pawn no matter its color */
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

int MobilityRook(int sq)
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

/* Returns 1 if rook is on an open column */
int OpenColRook(int sq)
{
    int l;
    int mob = 0;

    for (l = sq-8; ((l >= 0) && piece[l] != PAWN); l-=8)
        mob++;
    for (l = sq+8; ((l <= 63) && piece[l] != PAWN); l+=8)
        mob++;

    /* If it can reach 7 squares without finding a pawn then
    the rook is on an open column*/
    if (mob == 7)
        return 1;
    return 0;
}

/* Returns 1 if no enough material on the board */
int NoMaterial()
{
    int piecesOnBoard = whitePawns + whiteKnights + whiteBishops + whiteRooks + whiteQueens +
                        blackPawns + blackKnights + blackBishops + blackRooks + blackQueens;
    if (piecesOnBoard == 0)
        return 1;
    return 0;
}

/* The fun factor: the main idea is to encourage interesting positions:
    -Queens on the board
    -Not too much pawns*/
int funFactor()
{
    int ff;

    /* We like queens on the board */
    if (whiteQueens >= 1 || blackQueens >= 1) ff += 0;
    /* Too many pawns on the board aren't that funny */
    if (whitePawns + blackPawns < 12) ff += 5;
    /* No queens at all? That doesn't rule */
    if (whiteQueens == 0 && blackQueens == 0) ff -= 10;

    return ff;
}


