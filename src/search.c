#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "defs.h"
#include "data.h"
#include "protos.h"
#include <inttypes.h>	// Belka

MOVE ComputerThink(int m_depth)
{
    /* It returns the move the computer makes */
    MOVE m;
    MOVE bestMove;
    LINE pline;
    int score, i, j;
//    double knps;
    memset(history, 0, sizeof(history));

    /* Time management */
    int start_time = getMs();
    stopTime = start_time + maxTime;
    halfTime = start_time + 0.5 * maxTime;

    for (i = 1; i <= m_depth; ++i)
    {
        /* Reset some values before searching */
        ply = 0;
        countEvaluations = 0;
        countMakeMove = 0;
        countquiesCalls = 0;
        countCapCalls = 0;
        countSearchCalls = 0;
        nodes = 0;

        clock_t start;
        clock_t stop;
        double t = 0.0;

        /* Start timer */
        start = clock ();

        /* Search now! */
        score = search (-MATE, MATE, i, &m, &pline);

        /* If we've searched for a certain percentage of the available time it
        doesn't make sense to start a new ply, so we call it a day */
//        checkup(halfTime);


        /* Aqui debe ir el 'if' que hace un break si nos quedamos sin tiempo.
           Tomado de Darky */
        if (mustStop)
        {
            mustStop = 0;
            fflush(stdout);
            break;
        }

        /* Stop timer */
        stop = clock ();
        t = (double) (stop - start) / CLOCKS_PER_SEC;
//        knps = ((double) (countquiesCalls + countSearchCalls) / t) / 1000.;
//        double ratio_Qsearc_Capcalls = 0;
//        ratio_Qsearc_Capcalls = (double) countCapCalls / (double) countquiesCalls;

        double decimal_score = ((double) score) / 100.;
        if (side == BLACK)
        {
            decimal_score = -decimal_score;
        }

        bestMove = m;

        /* If the score is too large we just stop thinking */
        if (ABS(score) > MATE - 32)
        {
            printf("score = %d\n", score);
            fflush(stdout);
            break;
        }

        /* After searching, print results in xboard mode
            depth eval time nodes PV*/
        {
            int int_time = (int)(t * 100);
            printf (" %d  %2d %4d %8"  PRId64, i, score, int_time, nodes); // Belka

            /* Printing PV */
            for(j=0; j<pline.cmove; j++)
            {
              printf(" %c%d%c%d", 'a' + COL(pline.argmove[j].from),
                                   8 - ROW(pline.argmove[j].from),
                                  'a' + COL(pline.argmove[j].dest),
                                   8 - ROW(pline.argmove[j].dest));
              /* Checking either it's a promotion */
              switch (pline.argmove[j].type_of_move)
              {
                 case MOVE_TYPE_PROMOTION_TO_QUEEN:
                    printf("q");
                    break;
                 case MOVE_TYPE_PROMOTION_TO_ROOK:
                    printf("r");
                    break;
                 case MOVE_TYPE_PROMOTION_TO_BISHOP:
                    printf("b");
                    break;
                 case MOVE_TYPE_PROMOTION_TO_KNIGHT:
                    printf("n");
                    break;
               }
            }
        }
        puts("");
        fflush(stdout);
    }
    return bestMove;
}

/*
 ****************************************************************************
 * Search function - a typical alpha-beta + quiescent search, main search function *
 ****************************************************************************
 */

int search(int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline)
{
    /* Vars deffinition */
    int i;
    int value;			/* To store the evaluation */
    int havemove;		/* Either we have or not a legal move available */
    int movecnt;		/* The number of available moves */
//    int score;
    MOVE moveBuf[200];  /* List of moves */
    MOVE tmpMove;
    LINE    line;

    nodes++;
    countSearchCalls++;

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stopTime);
    if (mustStop)
        return 0;

    havemove = 0;		/* is there a move available? */
    pBestMove->type_of_move = MOVE_TYPE_NONE;

    /* If we are in a leaf node we cal quiescence instead of eval */
    if (depth == 0)
    {
       pline->cmove = 0;
       return quiescent(alpha, beta);
    }

    //    /* If we're in check maybe we want to search deeper */
        if (depth < maxDepth - 2 && isInCheck(side))
            ++depth;

    /* Generate and count all moves for current position */
    movecnt = genMoves (side, moveBuf);

//    /* If we're in check maybe we want to search deeper */
//    if (depth < maxDepth - 2 && isInCheck(side))
//        ++depth;

    /* Once we have all the moves available, we loop through the possible
     * moves and apply an alpha-beta search */
    for (i = 0; i < movecnt; ++i)
    {
        /* Here must be called OrderMove, so the moves are ordered before
        picking one up from the list*/
        moveOrder(i, movecnt, moveBuf);

        /* If the current move isn't legal, we take it back
         * and take the next move in the list */
        if (!makeMove (moveBuf[i]))
        {
            takeBack ();
            continue;
        }

        /* If we've reached this far, then we have a move available */
        havemove = 1;

        value = -search(-beta, -alpha, depth - 1, &tmpMove, &line);

        /* We've evaluated the position, so we return to the previous position in such a way
           that when we take the next move from moveBuf everything is in order */
        takeBack ();

        /* Once we have an evaluation, we use it in an alpha-beta search */
        if (value > alpha)
        {
            /* This move is so good and caused a cutoff */
            history[moveBuf[i].from][moveBuf[i].dest] += depth;

            if (value >= beta)
            {
                return beta;
            }

            alpha = value;

            /* So far, current move is the best reaction for current position */
            *pBestMove = moveBuf[i];

            /* Update the principal line */
            pline->argmove[0] = moveBuf[i];
            memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(MOVE));
            pline->cmove = line.cmove + 1;
        }
    }

    /* Once we've checked all the moves, if we have no legal moves,
     * then that's checkmate or stalemate */
    if (!havemove)
    {
        if (isInCheck (side))
            return -MATE + ply;	/* add ply to find the longest path to lose or shortest path to win */
        else
            return 0;
    }

    /* 3 vecez la misma posicion */
    if (reps() >= 2)
        return 0;

    if (fifty >= 100) /* 50 jugadas o mas */
        return 0;

    /* Finally we return alpha, the score value */
    return alpha;
}

int quiescent(int alpha, int beta)
{
    int i;
    int legal = 0;
    int movescnt = 0;
    int score;
    int best;

    MOVE qMovesBuf[200];

    countquiesCalls++;
    nodes++;

    int is_in_check = isInCheck(side);

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stopTime);
    if (mustStop)
        return 0;

    if (reps() >= 2)
        return 0;

    /* First we just try the evaluation function */
    /* We generate the moves deppending either we are in check or not */
    if (is_in_check)
    {
        movescnt = genMoves(side, qMovesBuf);
        countCapCalls++;
    }
    else
    {
        best = eval(alpha, beta);
        // --- stand pat cutoff?

        if (best > alpha)
        {
            if (best >= beta)
                return best;
            alpha = best;
        }

        movescnt = genCaps (side, qMovesBuf);
        countCapCalls++;
    }

    /* Now the alpha-beta search in quiescent */
    for (i = 0; i < movescnt; ++i)
    {
        /* If not in check or promotion (Thx to Pedro) */
        if (!is_in_check && qMovesBuf[i].type_of_move < MOVE_TYPE_PROMOTION_TO_QUEEN)
        {
            /* if bad capture we are done */
            if (badCapture(qMovesBuf[i])) continue;
        }

        moveOrder(i, movescnt, qMovesBuf);

        if (!makeMove (qMovesBuf[i]))
        {
            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            takeBack ();
            continue;
        }

        /* If we're here then tehre's at least one legal move */
        legal = 1;

        score = -quiescent (-beta, -alpha);
        takeBack ();

        if (mustStop)
            return 0;

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    /* If it's a check and there are no legal moves then it's checkmate */
    if (is_in_check && !legal)
        alpha = -MATE + ply;

    return alpha;
}

void moveOrder(int init, int movecounter, MOVE *orderMovesBuf)
{
    int i = 0;
    int aux = 0;
    int Max = 0;
    MOVE Tmp;

    Max = orderMovesBuf[init].grade;
    aux = init;

    for(i = (init + 1); i < movecounter; i++)
    {
        if (orderMovesBuf[i].grade > Max)
        {
            Max = orderMovesBuf[i].grade;
            aux = i;
        }
    }
    if (Max != orderMovesBuf[init].grade)
    {
        Tmp = orderMovesBuf[init];
        orderMovesBuf[init] = orderMovesBuf[aux];
        orderMovesBuf[aux] = Tmp;
    }
}

/* checkup() is called once in a while during the search. */
void checkup(clock_t stopping_time)
{
    mustStop = 0;
    if (getMs() >= stopping_time)
    {
        mustStop = 1;
    }
}

