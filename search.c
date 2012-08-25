#include <stdio.h>
#include <time.h>

#include "defs.h"
#include "data.h"
#include "protos.h"

//#define NDEBUG
//#include <assert.h>

/*
 ****************************************************************************
 * Search function - a typical alphabeta + quiescent search, main search function *
 * Lack: no move ordering *
 ****************************************************************************
 */

int
Search (int alpha, int beta, int depth, MOVE * pBestMove)
{
    int i;
    int value;			/* To store the evaluation */
    int havemove;			/* Either we have or not a legal move available */
    int movecnt;			/* The number of available moves */

    MOVE moveBuf[200];		/* List of movements */
    MOVE tmpMove;

    havemove = 0;			/* is there a move available? */
    pBestMove->type_of_move = MOVE_TYPE_NONE;

    /* Generate and count all moves for current position */
    movecnt = GenMoves (side, moveBuf);
//    assert (movecnt < 201);

    countSearchCalls++;


    /* Once we have all the moves available, we loop through the posible
     * moves and apply an alpha-beta search */
    for (i = 0; i < movecnt; ++i)
    {
        /********************************************************************
        * Here must be called OrderMove, so we have the moves are ordered before
        picking one up from the list*
        ********************************************************************/

        MoveOrder(i, movecnt, moveBuf);

        if (!MakeMove (moveBuf[i]))
        {
            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            TakeBack ();
            continue;
        }

        /* If we've reached this far, then we have a move available */
        havemove = 1;

        /* This 'if' takes us to the deep of the position, the leaf nodes */
        if (depth - 1 > 0)
        {
            value = -Search (-beta, -alpha, depth - 1, &tmpMove);
        }
        /* If no depth left (leaf node), we evalute the position
           and apply the alpha-beta search.
           In the case of existing a quiescent function, it should be
           called here instead of Eval() */
        else
        {
            value = -Quiescent (-beta, -alpha);
            // value = -Eval();
        }

        /* We've evaluated the position, so we return to the previous position in such a way
           that when we take the next move from moveBuf everything is in order */
        TakeBack ();

        /* Once we have an evaluation, we use it in in an alpha-beta search */
        if (value > alpha)
        {
            /* This move is so good and caused a cutoff */
            if (value >= beta)
            {
                return beta;
            }
            alpha = value;
            /* So far, current move is the best reaction for current position */
            *pBestMove = moveBuf[i];
        }
    }

    /* Once we've checked all the moves, if we have no legal moves,
     * then that's checkmate or stalemate */
    if (!havemove)
    {
        if (IsInCheck (side))
            return -MATE + ply;	/* add ply to find the longest path to lose or shortest path to win */
        else
            return 0;
    }

    /* Finally we return alpha, the score value */
    return alpha;
}

int
Quiescent (int alpha, int beta)
{
    int i;
    int capscnt;
    int stand_pat;
    int score;

    countquiesCalls++;

    /* First we just try the evaluation function */
    stand_pat = Eval ();
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    /* If we haven't got a cut off we generate the captures and
     * store them in cBuf */
    MOVE cBuf[200];
    capscnt = GenCaps (side, cBuf);

    countCapCalls++;

    for (i = 0; i < capscnt; ++i)
    {
//        MoveOrder(i, capscnt, cBuf);

        if (!MakeMove (cBuf[i]))
        {
            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            TakeBack ();
            continue;
        }
        score = -Quiescent (-beta, -alpha);
        TakeBack ();
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

void MoveOrder(int init, int movecount, MOVE *moveBuf)
{
    int i;
    int aux;
    int Max;
    MOVE Tmp;

    Max = moveBuf[init].grade;
    aux = init;

    for(i = (init + 1); i < movecount; i++)
    {
        if (moveBuf[i].grade > Max)
        {
            Max = moveBuf[i].grade;
            aux = i;
        }
    }
    if (Max != moveBuf[init].grade)
    {
        Tmp = moveBuf[init];
        moveBuf[init] = moveBuf[aux];
        moveBuf[aux] = Tmp;
    }
}

MOVE
ComputerThink (int depth)
{
    /* It returns the move the computer makes */
    MOVE m;
    int score;
    double knps;

    /* Reset some values before searching */
    ply = 0;
    count_evaluations = 0;
    count_MakeMove = 0;
    countquiesCalls = 0;
    countCapCalls = 0;
    countSearchCalls = 0;

    clock_t start;
    clock_t stop;
    double t = 0.0;

    /* Start timer */
    start = clock ();

    /* Search now */
    score = Search (-MATE, MATE, depth, &m);

    /* Stop timer */
    stop = clock ();
    t = (double) (stop - start) / CLOCKS_PER_SEC;
    knps = ((double) countquiesCalls / t) / 1000.;
    double ratio_Qsearc_Capcalls = 0;
    ratio_Qsearc_Capcalls = (double) countCapCalls / (double) countquiesCalls;

    double decimal_score = ((double) score) / 100.;
    if (side == BLACK)
    {
        decimal_score = -decimal_score;
    }

    /* After searching, print results */
    if (depth == max_depth)
        printf
        ("Search result: move = %c%d%c%d; depth = %d, score = %.2f, time = %.2f, knps = %.2f\n countCapCalls = %'llu\n countQSearch = %'llu\n moves made = %'llu\n ratio_Qsearc_Capcalls = %.2f\n",
         'a' + Col (m.from), 8 - Row (m.from), 'a' + Col (m.dest),
         8 - Row (m.dest), depth, decimal_score, t, knps, countCapCalls,
         countquiesCalls, count_MakeMove, ratio_Qsearc_Capcalls);
    else
        printf ("Search result: move = %c%d%c%d; depth = %d, score = %.2f\n",
                'a' + Col (m.from), 8 - Row (m.from), 'a' + Col (m.dest), 8
                - Row (m.dest), depth, decimal_score);

    return m;
}
