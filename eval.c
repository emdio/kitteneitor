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

/* To store the material of each side */
int piece_mat[2];

/* Arrays for scaing mobility values */
int mov_knight[9] = {
  -10, -4, 2, 8, 14, 18, 22, 24, 25
};

int mov_bishop[16] = {
  -20, -10, 0, 10, 20, 30, 38, 44, 48, 52, 54, 57, 58, 59, 60, 60
};


/* The evaluation function */
int
Eval ()
{

    count_evaluations++;

    /* A counter for the board squares */
    int i;

    /* The score of the position */
    int score = 0;

    /* Mobility */
    int Mob = 0;

    /* First pass around the board */
    piece_mat[WHITE] = 0;
    piece_mat[BLACK] = 0;
    for (i = 0; i < 64; ++i)
    {
        if (color[i] == EMPTY)
            continue;
        else
            /* Just counting the wood on the board */
            piece_mat[color[i]] += value_piece[piece[i]];
    }

    /* After counting the material we update the score */
    score = piece_mat[WHITE] - piece_mat[BLACK];

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
                score += mov_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score += pst_bishop[i];
                score += mov_bishop[BishopMobility(i)];
                break;
            case ROOK:
                score += pst_rook[i];
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
                score -= pst_knight[i];
//                score -= mov_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score -= pst_bishop[i];
//                score -= mov_bishop[BishopMobility(i)];
                break;
            case ROOK:
                score -= pst_rook[flip[i]];
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
        return score + 10;
    return -score - 10;
}

/* Are we in the endgame? */
inline int endGame()
{
    int allMaterial = piece_mat[WHITE] + piece_mat[BLACK];
    if (allMaterial < 22600)
        return 1;
    return 0;
}

/* Mobility */
int BishopMobility(int sq)
{
    int l;
    int m = 0;

    for (l = sq-9; ((l >= 0) && Col(l) < Col(sq) && piece[l] == EMPTY); l-=9)
        m++;
    for (l = sq-7; ((l >= 0) && Col(l) > Col(sq) && piece[l] == EMPTY); l-=7)
        m++;
    for (l = sq+7; ((l <= 63) && Col(l) < Col(sq) && piece[l] == EMPTY); l+=7)
        m++;
    for (l = sq+9; ((l <= 63) && Col(l) > Col(sq) && piece[l] == EMPTY); l+=9)
        m++;

    return m;
}

int KnightMobility(int sq)
{
    int l;
    int m = 0;

    l = sq - 17;
    if (l >= 0 && Col(l) < Col(sq) && piece[l] == EMPTY) m++;
    l = sq - 15;
    if (l >= 0 && Col(l) > Col(sq) && piece[l] == EMPTY) m++;

    l = sq - 10;
    if (l >= 0 && Col(l) < Col(sq) && piece[l] == EMPTY) m++;
    l = sq - 6;
    if (l >= 0 && Col(l) > Col(sq) && piece[l] == EMPTY) m++;

    l = sq + 6;
    if (l <= 63 && Col(l) < Col(sq) && piece[l] == EMPTY) m++;
    l = sq + 10;
    if (l <= 63 && Col(l) > Col(sq) && piece[l] == EMPTY) m++;

    l = sq + 15;
    if (l <= 63 && Col(l) < Col(sq) && piece[l] == EMPTY) m++;
    l = sq + 17;
    if (l <= 63 && Col(l) > Col(sq) && piece[l] == EMPTY) m++;

    return m;
}
