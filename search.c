#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "defs.h"
#include "data.h"
#include "protos.h"

//#define NDEBUG
//#include <assert.h>

MOVE
ComputerThink (int m_depth)
{
    /* It returns the move the computer makes */
    MOVE m;
    MOVE bestMove;
    LINE pline;
    int score, i, j;
    double knps;

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
//        checkup(half_time);


        /* Aqui debe ir el 'if' que hace un break si nos quedamos sin tiempo.
           Tomado de Darky */
        if (must_stop)
        {
            must_stop = 0;
            fflush(stdout);  /* Limpiamos la salida estandar */
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
        if (abs(score) > MATE - 10)
        {
            printf("score = %d\n", score);
            fflush(stdout);
            break;
        }



        /* After searching, print results */
        {
            printf ("Search result: move = %c%d%c%d; depth = %d, score = %.2f\n",
                    'a' + Col (bestMove.from), 8 - Row (bestMove.from), 'a' + Col (bestMove.dest), 8
                    - Row (bestMove.dest), i, decimal_score);

            /* Printing PV */
            for(j=0; j<pline.cmove; j++)
            {
              printf(" %c%d%c%d", 'a' + Col(pline.argmove[j].from),
                                   8 - Row(pline.argmove[j].from),
                                  'a' + Col(pline.argmove[j].dest),
                                   8 - Row(pline.argmove[j].dest));
              /* Paso 3 Si es coronacion ponemos la nueva pieza */
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
        if (i == m_depth)
        {
            printf
            ("\nSearch result final: move = %c%d%c%d; depth = %d, score = %.2f, time = %.2f s, knps = %.2f\n countCapCalls = %'llu\n countQSearch = %'llu\n moves made = %'llu\n ratio_Qsearc_Capcalls = %.2f\n nodes = %'llu\n",
             'a' + Col (bestMove.from), 8 - Row (bestMove.from), 'a' + Col (bestMove.dest),
             8 - Row (bestMove.dest), i, decimal_score, t, knps, countCapCalls,
             countquiesCalls, count_MakeMove, ratio_Qsearc_Capcalls, nodes);
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

int
Search (int alpha, int beta, int depth, MOVE * pBestMove, LINE * pline)
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


//    int R = 2;
//    if (depth - 1 - R > 1 &&
//        depth >= 5 &&
//        !IsInCheck(side) &&
//        !endGame() )
//    {
//        side = (BLACK + WHITE) - side;
//        score = -Search(depth - 1 - R, 1 - alpha, 0 - alpha, &tmpMove, &line);
//        side = (BLACK + WHITE) - side;
//        if (score >= beta)
//            return beta;
//    }



    if (depth == 0)
    {
       pline->cmove = 0;
       return Quiescent(alpha, beta);
    }

    /* Generate and count all moves for current position */
    movecnt = GenMoves (side, moveBuf);

    /* If we're in check maybe we want to search deeper */
    if (IsInCheck(side))
        ++depth;

    /* Once we have all the moves available, we loop through the posible
     * moves and apply an alpha-beta search */
    for (i = 0; i < movecnt; ++i)
    {
        /* Here must be called OrderMove, so we have the moves are ordered before
        picking one up from the list*/
        MoveOrder(i, movecnt, moveBuf);

        /* This a test similiar to what is done in qsearch, but the
         * result is really bad. Maybe it makes sense if the move
         * ordering is improvedor seting alower threshold */
//        if (moveBuf[i].grade < 0) continue;



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

//    for (i = 0; i < hdp; ++i)
//        printf("hash %d: %d-%d\n", i, hist[i].hash, hash.key);
//        if (hist[i].hash == hash.key)

//    if (reps() > 2)
//        printf ("reps: %d \n", reps());



    /* 3 vecez la misma posicion */
    if (reps() >= 2)
        return 0;

    if (fifty >= 100) /* 50 jugadas o mas */
        return 0;


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
    nodes++;

    /* Do some housekeeping every 1024 nodes */
    if ((nodes & 1023) == 0)
        checkup(stop_time);
    if (must_stop)
        return 0;


    /* First we just try the evaluation function */

    score = Eval (alpha, beta);
    stand_pat = score;
    if (stand_pat >= beta)
        return beta;

    int BIG_DELTA = VALUE_QUEEN;
    if ( score < alpha - BIG_DELTA )
    {
        return alpha;
    }

    if (alpha < stand_pat)
        alpha = stand_pat;


    /* If we haven't got a cut off we generate the captures and
     * store them in cBuf */
    MOVE cBuf[200];
    capscnt = GenCaps (side, cBuf);
    countCapCalls++;

    /* Now the alpha-beta search in quiescent */
    for (i = 0; i < capscnt; ++i)
    {
        /* If it's a bad capture we don't need to go on (tx to Pedro) */
//        if (BadCapture(cBuf[i])) continue;

//        if ( cBuf[i].grade < 0 ) continue;

        MoveOrder(i, capscnt, cBuf);

//        if (BadCapture(cBuf[i])) continue;

        if (!MakeMove (cBuf[i]))
        {
            /* If the current move isn't legal, we take it back
             * and take the next move in the list */
            TakeBack ();
            continue;
        }

        if (BadCapture(cBuf[i])) continue;

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

/* checkup() is called once in a while during the search. */
void checkup(clock_t stoping_time)
{
    must_stop = 0;
    if (get_ms() >= stoping_time)
    {
        must_stop = 1;
    }
}

