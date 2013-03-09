#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "defs.h"
#include "data.h"
#include "protos.h"


#define SEARCH_DEBUG


MOVE ComputerThink (int m_depth)
{
    /* It returns the move the computer makes */
    MOVE m;
    MOVE bestMove;
    LINE pline;
    int score, i, j;
    double knps;

    memset(history, 0, sizeof(history));

    /* Time management */
    int start_time = get_ms();
//    clock_t start_time = clock();
    stop_time = start_time + max_time;
    half_time = start_time + 0.5 * max_time;

//    printf ("max_time = %d\n", max_time);

    for (i = 1; i <= m_depth; ++i)
        {
            /* Reset some values before searching */
            ply = 0;
            count_evaluations = 0;
            count_MakeMove = 0;
            countquiesCalls = 0;
            countCapCalls = 0;
            countSearchCalls = 0;
            nodes = 0;
            memset(history, 0, sizeof(history));

            clock_t start;
            clock_t stop;
            double t = 0.0;

            /* Start timer */
            start = clock ();

            /* Search now! */
            score = Search (-MATE, MATE, i, &m, &pline);

            /* If we've searched for a certain percentage of the avaialble time it
            doesn't make sense to start a new ply, so we call it a day */
            //checkup(half_time);

            /* Aqui debe ir el 'if' que hace un break si nos quedamos sin tiempo.
               Tomado de Darky */
            if (must_stop)
                {
                    must_stop = 0;
                    fflush(stdout);  /* Limpiamos la salida estandar */
                    break;
                }

            /* If the score is too large we just stop thinking */
            if (ABS(score) > MATE - max_depth)
                {
                    printf("score = %d\n", score);
                    fflush(stdout);
                    break;
                }

            /* Stop timer */
            stop = clock ();
            t = (double) (stop - start) / CLOCKS_PER_SEC;
            knps = ((double) (countquiesCalls + countSearchCalls) / t) / 1000.;
            double ratio_Qsearc_Capcalls = 0;
            ratio_Qsearc_Capcalls = (double) countCapCalls / (double) countquiesCalls;

            double decimal_score = ((double) score) / 100.;
            if (side == BLACK)
                {
                    score = -score;
                    decimal_score = -decimal_score;
                }

            bestMove = m;

            if ( logfile )  // *not* 'user_wants_logfile' in case the original fopen() failed
            {
                fprintf(logfile, "In ComputerThink we have a move\n");
                fflush(logfile);  // always flush the file!
            }

            /* After searching, print results in xboard mode
                depth eval time nodes PV*/
            {
                int int_time = (int)(t * 100);
                printf (" %d  %4d %6d %10llu ", i, score, int_time, nodes);
                /* Printing PV */
                for(j=0; j<pline.cmove; j++)
                    {
                        printf(" %c%d%c%d", 'a' + Col(pline.argmove[j].from),
                               8 - Row(pline.argmove[j].from),
                               'a' + Col(pline.argmove[j].dest),
                               8 - Row(pline.argmove[j].dest));
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
 * Search function - a typical alphabeta + quiescent search, main search function *
 * Lack: no move ordering *
 ****************************************************************************
 */

int Search (int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline)
{

    /* Vars deffinition */
    int i;
    int value;			/* To store the evaluation */
    int havemove;		/* Either we have or not a legal move available */
    int movecnt;		/* The number of available moves */

//    int score;
    MOVE moveBuf[200];  /* List of movements */
    MOVE tmpMove;
    LINE    line;

    nodes++;
    countSearchCalls++;

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stop_time);
    if (must_stop)
        return 0;

    havemove = 0;		/* is there a move available? */
    pBestMove->type_of_move = MOVE_TYPE_NONE;

    /* If we are in a leaf node we cal quiescence instead of eval */
    if (depth == 0)
        {
            pline->cmove = 0;
            return Quiescent(alpha, beta);
            /* Uncomment nest line if want to make tests avoiding qsearch */
            //return Eval(alpha, beta);
        }

    /* If we're in check we want to search deeper */
    if (IsInCheck(side))
        ++depth;

    /* Static null move prunning */
    if (ply && !IsInCheck(side))
        {
            int ev = Eval(-MATE, MATE);
            int evalua = ev - 150;
            if (evalua >= beta)
                return beta;
        }

    /* Generate and count all moves for current position */
    movecnt = GenMoves (side, moveBuf);

    /* Once we have all the moves available, we loop through the posible
     * moves and apply an alpha-beta search */
    for (i = 0; i < movecnt; ++i)
        {
            /* Here must be called OrderMove, so we have the moves are ordered before
            picking one up from the list*/
            MoveOrder(i, movecnt, moveBuf);

            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            if (!MakeMove (moveBuf[i]))
                {
                    TakeBack ();
                    continue;
                }

            /* If we've reached this far, then we have a move available */
            havemove = 1;

            value = -Search(-beta, -alpha, depth - 1, &tmpMove, &line);

            /* We've evaluated the position, so we return to the previous position in such a way
               that when we take the next move from moveBuf everything is in order */
            TakeBack ();

            /* Once we have an evaluation, we use it in in an alpha-beta search */
            if (value > alpha)
                {
                    /* Este movimiento causo un cutoff, asi que incrementamos
                    el valor de historia para que sea ordenado antes la
                    proxima vez que se busque */
                    history[moveBuf[i].from][moveBuf[i].dest] += depth;

                    /* This move is so good and caused a cutoff */
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
            if (IsInCheck (side))
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

int Quiescent (int alpha, int beta)
{
    int i = 0;
    int movescnt = 0;
    int score = 0;
    int best = 0;
    MOVE qMovesBuf[200];

    countquiesCalls++;
    nodes++;

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stop_time);
    if (must_stop)
        return 0;

    if (reps() >= 2)
        return 0;

    best = Eval(alpha, beta);
    // --- stand pat cutoff?
    if (best > alpha)
        {
            if (best >= beta)
                return best;
            alpha = best;
        }

    /* As we are in qasearch we generate the captures */
    movescnt = GenCaps (side, qMovesBuf);
    countCapCalls++;

#ifdef SEARCH_DEBUG
            if (movescnt > 200) printf("Too much moves!: %d", movescnt);
#endif

    /* Now the alpha-beta search in quiescent */
    for (i = 0; i < movescnt; ++i)
        {
        MoveOrder(i, movescnt, qMovesBuf);

            if (!MakeMove (qMovesBuf[i]))
            {
                /* If the current move isn't legal, we take it back
                 * and take the next move in the list */
                TakeBack ();
                continue;
            }

            score = -Quiescent (-beta, -alpha);
            TakeBack ();

            if ((nodes & 1023) == 0)
                checkup(stop_time);
            if (must_stop)
                return 0;

            if (score >= beta)
                return beta;
            if (score > alpha)
                alpha = score;
        }
#ifdef SEARCH_DEBUG
    if (alpha > MATE) printf("alpha too high: %d", alpha);
    if (alpha < -MATE) printf("alpha too low: %d", alpha);
#endif

    return alpha;
}

void MoveOrder(int init, int movecounter, MOVE *orderMovesBuf)
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
void checkup(clock_t stoping_time)
{
    must_stop = 0;
    if (get_ms() >= stoping_time)
        {
            must_stop = 1;
        }
}

