#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "defs.h"
#include "data.h"
#include "protos.h"
#include <inttypes.h>    // belka

MOVE ComputerThink(int m_depth)
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
    stop_time = start_time + max_time;
    half_time = start_time + 0.5 * max_time;

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

        clock_t start;
        clock_t stop;
        double t = 0.0;

        /* Start timer */
        start = clock ();

        /* Search now! */
        score = Search (-MATE, MATE, i, &m, &pline);

        /* If we've searched for a certain percentage of the avaialble time it
        doesn't make sense to start a new ply, so we call it a day */
//        checkup(half_time);


        /* Aqui debe ir el 'if' que hace un break si nos quedamos sin tiempo.
           Tomado de Darky */
        if (must_stop)
        {
            must_stop = 0;
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

            /* belka
    		That is because %llu doesn't work properly under Windows. 
			I suggest using PRIu64 instead and you'll find it's portable to Linux 
			as well.
			You may want to try using the inttypes.h library that gives you types 
			such as int32_t, int64_t, uint64_t etc. You can then use its macros such as: 
			uint64_t x;
			uint32_t y;
			printf("x: %"PRId64", y: %"PRId32"\n", x, y);
			*/
            // In Linux it is %llu and in Windows it is %I64u
            //printf (" %d  %2d %4d %8llu ", i, score, int_time, nodes);
            printf (" %d  %2d %4d %8I64u ", i, score, int_time, nodes);

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
 * Search function - a typical alphabeta + quiescent search, main search function *
 * Lack: no move ordering *
 ****************************************************************************
 */

int Search(int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline)
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
    }

    //    /* If we're in check maybe we want to search deeper */
        if (depth < max_depth - 2 && IsInCheck(side))
            ++depth;

    /* Generate and count all moves for current position */
    movecnt = GenMoves (side, moveBuf);

//    /* If we're in check maybe we want to search deeper */
//    if (depth < max_depth - 2 && IsInCheck(side))
//        ++depth;

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

int Quiescent(int alpha, int beta)
{
    int i;
    int legal = 0;
    int movescnt = 0;
    int score;
    int best;

    MOVE qMovesBuf[200];

    countquiesCalls++;
    nodes++;

    int is_in_check = IsInCheck(side);

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stop_time);
    if (must_stop)
        return 0;

    if (reps() >= 2)
        return 0;

    /* First we just try the evaluation function */
    /* We generate the moves deppending either
       we are in check or not */
    if (is_in_check)
    {
        movescnt = GenMoves(side, qMovesBuf);
        countCapCalls++;
    }
    else
    {
        best = Eval(alpha, beta);
        // --- stand pat cutoff?

        if (best > alpha)
        {
            if (best >= beta)
                return best;
            alpha = best;
        }

        movescnt = GenCaps (side, qMovesBuf);
        countCapCalls++;
    }

    /* Now the alpha-beta search in quiescent */
    for (i = 0; i < movescnt; ++i)
    {
        /* If not in check or promotion (Thx to Pedro) */
        if (!is_in_check && qMovesBuf[i].type_of_move < MOVE_TYPE_PROMOTION_TO_QUEEN)
        {
            /* if bad capture we are done */
            if (BadCapture(qMovesBuf[i])) continue;
        }

        MoveOrder(i, movescnt, qMovesBuf);

        if (!MakeMove (qMovesBuf[i]))
        {
            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            TakeBack ();
            continue;
        }

        /* If we're here then tehre's at least one legal move */
        legal = 1;

        score = -Quiescent (-beta, -alpha);
        TakeBack ();

        if (must_stop)
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

