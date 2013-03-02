#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

#define BOARDDEBUG

/* This is usually defined by a macro like #define Col(pos) (pos & 7) */
inline int Col (int square)
{
    return square & 7;
}

/* This is usually defined by a macro like #define ROW(pos) (pos >> 3) */
inline int Row (int square)
{
    return square >> 3;
}

/* Just returns the color of the opponent */
inline int Opponent(int color)
{
    return (!(color));
}

/* Sign returns 1 fot White and -1 for Black */
inline int Sign(int color)
{
    return (1 - (color * 2));
}

/* UP is -8, DOWN is +8: Ahead returns the 'forward' direction for color */
inline int Ahead(int color)
{
    return (Sign(color) * UP);
}

/* Behind returns the 'backward' direction for color */
inline int Behind(int color)
{
    return (Sign(color) * DOWN);
}

/* IsSqProtectedByAPawn returns 1 if sq is protected by a pawn of color side */
inline int IsSqProtectedByAPawn(int sq, int side)
{
    /* We need to check the columm because in col 1 and 8 sq can only be attacked
     * from one side */
    int col = Col(sq);

#ifdef BOARDDEBUG
    if (side != BLACK && side != WHITE) printf ("IsSqProtectedByAPawn() wrong color");
    if (sq < 0) printf("IsSqProtectedByAPawn() sq < 0");
    if (sq > 63) printf("IsSqProtectedByAPawn() sq > 63");
#endif

    if ( col != 7 && piece[sq + Behind(side) + 1] == PAWN &&  color[sq + Behind(side) + 1] == side )
        return 1;
    else if ( col != 0 && piece[sq + Behind(side) - 1] == PAWN && color[sq + Behind(side) - 1] == side )
        return 1;

    return 0;
}

/* IsProtectedByAKnight returns 1 if square sq is protected by a knight of color side */
int IsSqProtectedByAKnight(int sq, int side)
{

#ifdef BOARDDEBUG
    if (side != BLACK && side != WHITE) printf ("IsSqProtectedByAKnight() wrong color");
    if (sq < 0) printf("IsSqProtectedByAKnight() k < 0");
    if (sq > 63) printf("IsSqProtectedByAKnight() k > 63");
#endif

    int y;
    int col = Col (sq);
    y = sq - 6;
    if (y >= 0 && col < 6 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq - 10;
    if (y >= 0 && col > 1 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq - 15;
    if (y >= 0 && col < 7 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq - 17;
    if (y >= 0 && col > 0 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq + 6;
    if (y < 64 && col > 1 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq + 10;
    if (y < 64 && col < 6 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq + 15;
    if (y < 64 && col > 0 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    y = sq + 17;
    if (y < 64 && col < 7 && piece[y] == KNIGHT && color[y] == side)
        return 1;
    return 0;
}

/* IsProtectedByABishop returns 1 if square sq is protected by a bishop of color side */
int IsSqProtectedByABishop(int sq, int side)
{

#ifdef BOARDDEBUG
    if (side != BLACK && side != WHITE) printf ("IsSqProtectedByAKnight() wrong color");
    if (sq < 0) printf("IsSqProtectedByAKnight() k < 0");
    if (sq > 63) printf("IsSqProtectedByAKnight() k > 63");
#endif

    int y;
    for (y = sq - 9; y >= 0 && Col (y) != 7; y -= 9)
        {
            /* go left up */
            if (color[y] != EMPTY)
                {
                    if (color[y] == side)
                        {
                            if (piece[y] == BISHOP)
                                {
                                    return 1;
                                }
                        }
                }
        }
    for (y = sq - 7; y >= 0 && Col (y) != 0; y -= 7)
        {
            /* go right up */
            if (color[y] != EMPTY)
                {
                    if (color[y] == side)
                        {
                            if (piece[y] == BISHOP)
                                {
                                    return 1;
                                }
                        }
                }
        }
    for (y = sq + 9; y < 64 && Col (y) != 0; y += 9)
        {
            /* go right down */
            if (color[y] != EMPTY)
                {
                    if (color[y] == side)
                        {
                            if (piece[y] == BISHOP)
                                {
                                    return 1;
                                }
                        }
                }
        }
    for (y = sq + 7; y < 64 && Col (y) != 7; y += 7)
        {
            /* go left down */
            if (color[y] != EMPTY)
                {
                    if (color[y] == side)
                        {
                            if (piece[y] == BISHOP)
                                {
                                    return 1;
                                }
                        }
                }
        }

    return 0;
}

/*esta función nos permite saber que algunos movimientos son malas capturas, basado en CPW
sustituye de momento a SEE (static exchange evaluator)*/
int BadCapture(MOVE mcmov)
{

    int bishop_protected = 0;
    int knight_protected = 0;

#ifdef BOARDDEBUG
    int typeOfMove = mcmov.type_of_move;
    if (typeOfMove > 8) printf ("type of move %d > 8\n", typeOfMove);
    if (typeOfMove < 0) printf ("type of move %d \n < 8", typeOfMove);
#endif

    if (piece[mcmov.from] == PAWN)
        return 0;

    /* Capturing equal or more material is never bad */
    if (value_piece[piece[mcmov.dest]] > value_piece[piece[mcmov.from]] - 50)
        return 0;

    /* If the captured piece isn't protected by a pawn and its value
     * is bigger than a pawn, then it isn't a bad capture */
//    if (!IsSqProtectedByAPawn(mcmov.dest, color[mcmov.dest]) &&
//            value_piece[piece[mcmov.dest]] > value_piece[PAWN] &&
//            value_piece[piece[mcmov.from]] < value_piece[ROOK] )
//        return 0;

    /* We're capturing a piece with less value than ours, so we want to
    know either it is protected by a pawn? */
    if ( IsSqProtectedByAPawn(mcmov.dest, color[mcmov.dest]) )
        return 1;


//    /**********************************************/
//    /* Is the piece protected by a knight? */
    if ( IsSqProtectedByAKnight(mcmov.dest, color[mcmov.dest]) )
        knight_protected = 1;


//    /**********************************************/
//    /* Is the piece protected by a bishop? */
    if ( IsSqProtectedByABishop(mcmov.dest, color[mcmov.dest]) )
        bishop_protected = 1;


    if (knight_protected || bishop_protected)
        return 1;


    /* If we have reached this far then it isn't a bad capture */
    return 0;
}

/*
 ****************************************************************************
 * Move generator *
 ****************************************************************************
 */
void Gen_Push (int from, int dest, int type, MOVE * pBuf, int *pMCount)
{
    MOVE move;
    move.from = from;
    move.dest = dest;
    move.type_of_move = type;

    /* Now we assign the value to 'grade'
        Lack: Check if a piece is being attacked by a weaker one: for
    example if a knight is being attacked by a pawn, then moving the knight
    probably would be the best move (or at least deserves a position close
    to the top of the table)*/
    move.grade = 0;


#ifdef BOARDDEBUG
    if (from > 63) printf ("Gen_Push from > 63");
    if (dest > 63) printf ("Gen_Push dest > 63");
    if (from < 0) printf ("Gen_Push from < 0");
    if (dest < 0) printf ("Gen_Push dest < 0");
    if (type < 0 ) printf ("Gen_Push type < 0");
    if (type > MOVE_TYPE_PROMOTION_TO_KNIGHT) printf ("Gen_Push type > MOVE_TYPE_PROMOTION_TO_KNIGHT");
#endif

    /* Is it a promotion? */
    if ((type >= MOVE_TYPE_PROMOTION_TO_QUEEN) &&
            (type <= MOVE_TYPE_PROMOTION_TO_KNIGHT))
        {
            if (type == MOVE_TYPE_PROMOTION_TO_QUEEN)
                move.grade = 1000050;
            else if (type == MOVE_TYPE_PROMOTION_TO_ROOK)
                move.grade = 30000;
            else if (type == MOVE_TYPE_PROMOTION_TO_KNIGHT)
                move.grade = 40000;
            else if (type == MOVE_TYPE_PROMOTION_TO_BISHOP)
                move.grade = 20000;
        }
    else
        {
            /* Is it a capture? */
            if ( color[dest] != EMPTY )
                {
                    /*malas capturas por detrás*/
                    if (BadCapture(move) == 1)
                        {
                            move.grade = -10000000 + value_piece[piece[dest]] - value_piece[piece[from]];
                        }
                    /*ordenamos por MVV/LVA*/
                    else
                        {
                            /* These are "good" captures, but we aren't taking into account factors
                             * such as if the captured piece is protected by a pawn: base on this
                             * kind of checkings we can give different levels of "goodness"? */
                            move.grade = 3000000 + value_piece[piece[dest]] - value_piece[piece[from]];
                            /* It isn't a bad capture, but if the captured piece is not protected
                             * by a pawn we give it an extra push */
//                if (!IsSqProtectedByAPawn(dest, Opponent(color[from])))
//                    move.grade += 0;
                        }
                }
            /* So it isn't a capture */
            else
                {
                    /* Just to make tests with the difference */
                    int mult = 1;

                    /* Are we moving to a better/worse piece square? */
                    if (color[from] == WHITE)
                        {
                            switch (piece[from])
                                {
                                case PAWN:
                                    if (endGame())
                                        move.grade += pst_pawn_endgame[dest] - pst_pawn_endgame[from];
                                    else
                                        move.grade += pst_pawn_midgame[dest] - pst_pawn_midgame[from];
                                    break;
                                case KNIGHT:
                                    move.grade += mult*(pst_knight[dest] - pst_knight[from]);
                                    break;
                                case BISHOP:
                                    move.grade += mult*(pst_bishop[dest] - pst_bishop[from]);
                                    break;
                                case ROOK:
                                    move.grade += mult*(pst_rook[dest] - pst_rook[from]);
                                    break;
                                case QUEEN:
                                    move.grade += mult*(pst_queen[dest] - pst_queen[from]);
                                    break;
                                case KING:
                                    if (endGame())
                                        move.grade += mult*(pst_king_endgame[dest] - pst_king_endgame[from]);
                                    else
                                        move.grade += mult*(pst_king_midgame[dest] - pst_king_midgame[from]);
                                    break;
                                }
                        }
                    else if (color[from] == BLACK)
                        {
                            switch (piece[from])
                                {
                                case PAWN:
                                    if (endGame())
                                        move.grade += pst_pawn_endgame[flip[dest]] - pst_pawn_endgame[flip[from]];
                                    else
                                        move.grade += pst_pawn_midgame[flip[dest]] - pst_pawn_midgame[flip[from]];
                                    break;
                                case KNIGHT:
                                    move.grade += mult*(pst_knight[flip[dest]] - pst_knight[flip[from]]);
                                    break;
                                case BISHOP:
                                    move.grade += mult*(pst_bishop[flip[dest]] - pst_bishop[flip[from]]);
                                    break;
                                case ROOK:
                                    move.grade += mult*(pst_rook[flip[dest]] - pst_rook[flip[from]]);
                                    break;
                                case QUEEN:
                                    move.grade += mult*(pst_queen[flip[dest]] - pst_queen[flip[from]]);
                                    break;
                                case KING:
                                    if (endGame())
                                        move.grade += mult*(pst_king_endgame[flip[dest]] - pst_king_endgame[flip[from]]);
                                    else
                                        move.grade += mult*(pst_king_midgame[flip[dest]] - pst_king_midgame[flip[from]]);
                                    break;
                                }
                        }
                    /* Are we placing a piece in a square deffended by a pawn? Sounds like a bad idea */
//            if  ( piece[from] != PAWN && IsSqProtectedByAPawn(dest, Opponent(color[from])) )
//                move.grade -= (value_piece[piece[from]]);
//            /* Is a piece being attacked by a pawn? Then we probably should move it */
//            if  ( piece[from] != PAWN && IsSqProtectedByAPawn(from, Opponent(color[from])) )
//                move.grade += (value_piece[piece[from]]);

//            else if  ( piece[from] == QUEEN || piece[from] == ROOK)
//            {
//                if (IsSqProtectedByABishop(from, Opponent(color[from])))
//                    move.grade += (value_piece[piece[from]]);
//                else if (IsSqProtectedByAKnight(from, Opponent(color[from])) )
//                    move.grade += (value_piece[piece[from]]);
//                else if  ( IsSqProtectedByABishop(dest, Opponent(color[from])) )
//                    move.grade -= (value_piece[piece[from]]);
//                else if  (IsSqProtectedByAKnight(dest, Opponent(color[from])) )
//                    move.grade -= (value_piece[piece[from]]);
//            }


                    /* Finally we use the history */
//            move.grade += 1* history[from][dest];
                }
        }

    /* Once we're done with grading the moves we fill the rest of the fields */
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void Gen_PushNormal (int from, int dest, MOVE * pBuf, int *pMCount)
{
    Gen_Push (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
}

/* Especial cases for Pawn */

/* Pawn can promote */
void Gen_PushPawn (int from, int dest, MOVE * pBuf, int *pMCount)
{
#ifdef BOARDDEBUG
    if (from > 63) printf ("Gen_Push from > 63");
    if (dest > 63) printf ("Gen_Push dest > 63");
    if (from < 0) printf ("Gen_Push from < 0");
    if (dest < 0) printf ("Gen_Push dest < 0");
#endif

    if (piece[dest] == EPS_SQ)
        {
            Gen_Push (from, dest, MOVE_TYPE_EPS, pBuf, pMCount);
        }
    /* The 7 and 56 are to limit pawns to the 2nd through 7th ranks, which
     * means this isn't a promotion, i.e., a normal pawn move */
    else if (dest > 7 && dest < 56)
        {
            Gen_Push (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
        }
    else				/* otherwise it's a promotion */
        {
            Gen_Push (from, dest, MOVE_TYPE_PROMOTION_TO_QUEEN, pBuf, pMCount);
            Gen_Push (from, dest, MOVE_TYPE_PROMOTION_TO_ROOK, pBuf, pMCount);
            Gen_Push (from, dest, MOVE_TYPE_PROMOTION_TO_BISHOP, pBuf, pMCount);
            Gen_Push (from, dest, MOVE_TYPE_PROMOTION_TO_KNIGHT, pBuf, pMCount);
        }
}

/* When a pawn moves two squares then appears the possibility of the en passant capture*/
void Gen_PushPawnTwo (int from, int dest, MOVE * pBuf, int *pMCount)
{
#ifdef BOARDDEBUG
    if (from > 63) printf ("Gen_Push from > 63");
    if (dest > 63) printf ("Gen_Push dest > 63");
    if (from < 0) printf ("Gen_Push from < 0");
    if (dest < 0) printf ("Gen_Push dest < 0");
#endif

    Gen_Push (from, dest, MOVE_TYPE_PAWN_TWO, pBuf, pMCount);
}

/* Especial cases for King */
void Gen_PushKing (int from, int dest, MOVE * pBuf, int *pMCount)
{
#ifdef BOARDDEBUG
    if (from > 63) printf ("Gen_Push from > 63");
    if (dest > 63) printf ("Gen_Push dest > 63");
    if (from < 0) printf ("Gen_Push from < 0");
    if (dest < 0) printf ("Gen_Push dest < 0");
#endif

    /* Is it a castle? */
    if (from == E1 && (dest == G1 || dest == C1))	/* this is a white castle */
        {
            Gen_Push (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
        }
    else if (from == E8 && (dest == G8 || dest == C8))	/* this is a black castle */
        {
            Gen_Push (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
        }
    else				/* otherwise it's a normal king's move */
        {
            Gen_Push (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
        }
}

/* Gen all moves of current_side to move and push them to pBuf, and return number of moves */
int GenMoves (int current_side, MOVE * pBuf)
{
    int i = 0;			/* Counter for the board squares */
    int k = 0;			/* Counter for cols */
    int y = 0;
    int row = 0;
    int col = 0;
    int movecount= 0;

#ifdef BOARDDEBUG
    if (current_side != BLACK && current_side != WHITE) printf ("GenMoves() wrong color");
#endif

    for (i = 0; i < 64; i++)	/* Scan all board */
        if (color[i] == current_side)
            {
                switch (piece[i])
                    {

                    case PAWN:
                        col = Col (i);
                        row = Row (i);
                        if (current_side == BLACK)
                            {
                                if (color[i + 8] == EMPTY)
                                    /* Pawn advances one square.
                                     * We use Gen_PushPawn because it can be a promotion */
                                    Gen_PushPawn (i, i + 8, pBuf, &movecount);
                                if (row == 1 && color[i + 8] == EMPTY
                                        && color[i + 16] == EMPTY)
                                    /* Pawn advances two squares */
                                    Gen_PushPawnTwo (i, i + 16, pBuf, &movecount);
                                if (col && color[i + 7] == WHITE)
                                    /* Pawn captures and it can be a promotion */
                                    Gen_PushPawn (i, i + 7, pBuf, &movecount);
                                if (col < 7 && color[i + 9] == WHITE)
                                    /* Pawn captures and can be a promotion */
                                    Gen_PushPawn (i, i + 9, pBuf, &movecount);
                                /* For en passant capture */
                                if (col && piece[i + 7] == EPS_SQ)
                                    /* Pawn captures and it can be a promotion */
                                    Gen_PushPawn (i, i + 7, pBuf, &movecount);
                                if (col < 7 && piece[i + 9] == EPS_SQ)
                                    /* Pawn captures and can be a promotion */
                                    Gen_PushPawn (i, i + 9, pBuf, &movecount);
                            }
                        else
                            {
                                if (color[i - 8] == EMPTY)
                                    Gen_PushPawn (i, i - 8, pBuf, &movecount);
                                /* Pawn moves 2 squares */
                                if (row == 6 && color[i - 8] == EMPTY
                                        && color[i - 16] == EMPTY)
                                    Gen_PushPawnTwo (i, i - 16, pBuf, &movecount);
                                /* For captures */
                                if (col && color[i - 9] == BLACK)
                                    Gen_PushPawn (i, i - 9, pBuf, &movecount);
                                if (col < 7 && color[i - 7] == BLACK)
                                    Gen_PushPawn (i, i - 7, pBuf, &movecount);
                                /* For en passant capture */
                                if (col && piece[i - 9] == EPS_SQ)
                                    Gen_PushPawn (i, i - 9, pBuf, &movecount);
                                if (col < 7 && piece[i - 7] == EPS_SQ)
                                    Gen_PushPawn (i, i - 7, pBuf, &movecount);
                            }
                        break;

                    case QUEEN:		/* == BISHOP+ROOK */

                    case BISHOP:
                        for (y = i - 9; y >= 0 && Col (y) != 7; y -= 9)
                            {
                                /* go left up */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (y = i - 7; y >= 0 && Col (y) != 0; y -= 7)
                            {
                                /* go right up */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (y = i + 9; y < 64 && Col (y) != 0; y += 9)
                            {
                                /* go right down */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (y = i + 7; y < 64 && Col (y) != 7; y += 7)
                            {
                                /* go left down */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        if (piece[i] == BISHOP)	/* In the case of the bishop we're done */
                            break;

                        /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
                    case ROOK:
                        col = Col (i);
                        for (k = i - col, y = i - 1; y >= k; y--)
                            {
                                /* go left */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (k = i - col + 7, y = i + 1; y <= k; y++)
                            {
                                /* go right */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (y = i - 8; y >= 0; y -= 8)
                            {
                                /* go up */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        for (y = i + 8; y < 64; y += 8)
                            {
                                /* go down */
                                if (color[y] != current_side)
                                    Gen_PushNormal (i, y, pBuf, &movecount);
                                if (color[y] != EMPTY)
                                    break;
                            }
                        break;

                    case KNIGHT:
                        col = Col (i);
                        y = i - 6;
                        if (y >= 0 && col < 6 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i - 10;
                        if (y >= 0 && col > 1 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i - 15;
                        if (y >= 0 && col < 7 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i - 17;
                        if (y >= 0 && col > 0 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i + 6;
                        if (y < 64 && col > 1 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i + 10;
                        if (y < 64 && col < 6 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i + 15;
                        if (y < 64 && col > 0 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        y = i + 17;
                        if (y < 64 && col < 7 && color[y] != current_side)
                            Gen_PushNormal (i, y, pBuf, &movecount);
                        break;

                    case KING:
                        /* the column and rank checks are to make sure it is on the board */
                        /* The 'normal' moves */
                        col = Col (i);
                        if (col && color[i - 1] != current_side)
                            Gen_PushKing (i, i - 1, pBuf, &movecount);	/* left */
                        if (col < 7 && color[i + 1] != current_side)
                            Gen_PushKing (i, i + 1, pBuf, &movecount);	/* right */
                        if (i > 7 && color[i - 8] != current_side)
                            Gen_PushKing (i, i - 8, pBuf, &movecount);	/* up */
                        if (i < 56 && color[i + 8] != current_side)
                            Gen_PushKing (i, i + 8, pBuf, &movecount);	/* down */
                        if (col && i > 7 && color[i - 9] != current_side)
                            Gen_PushKing (i, i - 9, pBuf, &movecount);	/* left up */
                        if (col < 7 && i > 7 && color[i - 7] != current_side)
                            Gen_PushKing (i, i - 7, pBuf, &movecount);	/* right up */
                        if (col && i < 56 && color[i + 7] != current_side)
                            Gen_PushKing (i, i + 7, pBuf, &movecount);	/* left down */
                        if (col < 7 && i < 56 && color[i + 9] != current_side)
                            Gen_PushKing (i, i + 9, pBuf, &movecount);	/* right down */

                        /* The castle moves */
                        if (current_side == WHITE)
                            {
                                /* Can white short castle? */
                                if (castle & 1)
                                    {
                                        /* If white can castle the white king has to be in square 60 */
                                        if (col &&
                                                color[i + 1] == EMPTY &&
                                                color[i + 2] == EMPTY &&
                                                !IsInCheck (current_side) &&
                                                !IsAttacked (current_side, i + 1))
                                            {
                                                /* The king goes 2 sq to the left */
                                                Gen_PushKing (i, i + 2, pBuf, &movecount);
                                            }
                                    }

                                /* Can white long castle? */
                                if (castle & 2)
                                    {
                                        if (col &&
                                                color[i - 1] == EMPTY &&
                                                color[i - 2] == EMPTY &&
                                                color[i - 3] == EMPTY &&
                                                !IsInCheck (current_side) &&
                                                !IsAttacked (current_side, i - 1))
                                            {
                                                /* The king goes 2 sq to the left */
                                                Gen_PushKing (i, i - 2, pBuf, &movecount);
                                            }
                                    }
                            }
                        else if (current_side == BLACK)
                            {
                                /* Can black short castle? */
                                if (castle & 4)
                                    {
                                        /* If white can castle the white king has to be in square 60 */
                                        if (col &&
                                                color[i + 1] == EMPTY &&
                                                color[i + 2] == EMPTY &&
                                                piece[i + 3] == ROOK &&
                                                !IsInCheck (current_side) &&
                                                !IsAttacked (current_side, i + 1))
                                            {
                                                /* The king goes 2 sq to the left */
                                                Gen_PushKing (i, i + 2, pBuf, &movecount);
                                            }
                                    }
                                /* Can black long castle? */
                                if (castle & 8)
                                    {
                                        if (col &&
                                                color[i - 1] == EMPTY &&
                                                color[i - 2] == EMPTY &&
                                                color[i - 3] == EMPTY &&
                                                piece[i - 4] == ROOK &&
                                                !IsInCheck (current_side) &&
                                                !IsAttacked (current_side, i - 1))
                                            {
                                                /* The king goes 2 sq to the left */
                                                Gen_PushKing (i, i - 2, pBuf, &movecount);
                                            }
                                    }
                            }

                        break;
                    default:
                        printf("Piece type unknown, %d", piece[i]);
                    }
            }
#ifdef BOARDDEBUG
    if (movecount > 200) printf ("Too much moves on GenMoves %d \n", movecount);
#endif
    return movecount;
}

/* Gen all captures of current_side to move and push them to pBuf, return number of moves
 * It's necesary at least ir order to use quiescent in the search */
int GenCaps (int current_side, MOVE * pBuf)
{
    int i = 0;			/* Counter for the board squares */
    int k = 0;			/* Counter for cols */
    int y = 0;
    int row = 0;
    int col = 0;
    int capscount = 0;		/* Counter for the posible captures */
    int xside =  Opponent(current_side);

#ifdef BOARDDEBUG
    if (current_side != BLACK && current_side != WHITE) printf ("GenMoves() wrong color");
#endif

    for (i = 0; i < 64; i++)	/* Scan all board */
        if (color[i] == current_side)
            {
                switch (piece[i])
                    {

                    case PAWN:
                        col = Col (i);
                        row = Row (i);
                        if (current_side == BLACK)
                            {
                                /* This isn't a capture, but it's necesary in order to
                                 * not oversee promotions */
                                if (row > 7 && color[i + 8] == EMPTY)
                                    /* Pawn advances one square.
                                     * We use Gen_PushPawn because it can be a promotion */
                                    Gen_PushPawn (i, i + 8, pBuf, &capscount);
                                if (col && color[i + 7] == WHITE)
                                    /* Pawn captures and it can be a promotion */
                                    Gen_PushPawn (i, i + 7, pBuf, &capscount);
                                if (col < 7 && color[i + 9] == WHITE)
                                    /* Pawn captures and can be a promotion */
                                    Gen_PushPawn (i, i + 9, pBuf, &capscount);
                                /* For en passant capture */
                                if (col && piece[i + 7] == EPS_SQ)
                                    /* Pawn captures and it can be a promotion */
                                    Gen_PushPawn (i, i + 7, pBuf, &capscount);
                                if (col < 7 && piece[i + 9] == EPS_SQ)
                                    /* Pawn captures and can be a promotion */
                                    Gen_PushPawn (i, i + 9, pBuf, &capscount);
                            }
                        else if (current_side == WHITE)
                            {
                                if (row < 2 && color[i - 8] == EMPTY)
                                    /* This isn't a capture, but it's necesary in order to
                                     * not oversee promotions */
                                    Gen_PushPawn (i, i - 8, pBuf, &capscount);
                                /* For captures */
                                if (col && color[i - 9] == BLACK)
                                    Gen_PushPawn (i, i - 9, pBuf, &capscount);
                                if (col < 7 && color[i - 7] == BLACK)
                                    Gen_PushPawn (i, i - 7, pBuf, &capscount);
                                /* For en passant capture */
                                if (col && piece[i - 9] == EPS_SQ)
                                    Gen_PushPawn (i, i - 9, pBuf, &capscount);
                                if (col < 7 && piece[i - 7] == EPS_SQ)
                                    Gen_PushPawn (i, i - 7, pBuf, &capscount);
                            }
                        break;

                    case KNIGHT:
                        col = Col (i);
                        y = i - 6;
                        if (y >= 0 && col < 6 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i - 10;
                        if (y >= 0 && col > 1 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i - 15;
                        if (y >= 0 && col < 7 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i - 17;
                        if (y >= 0 && col > 0 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i + 6;
                        if (y < 64 && col > 1 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i + 10;
                        if (y < 64 && col < 6 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i + 15;
                        if (y < 64 && col > 0 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        y = i + 17;
                        if (y < 64 && col < 7 && color[y] == xside)
                            Gen_PushNormal (i, y, pBuf, &capscount);
                        break;

                    case QUEEN:		/* == BISHOP+ROOK */

                    case BISHOP:
                        for (y = i - 9; y >= 0 && Col (y) != 7; y -= 9)
                            {
                                /* go left up */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (y = i - 7; y >= 0 && Col (y) != 0; y -= 7)
                            {
                                /* go right up */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (y = i + 9; y < 64 && Col (y) != 0; y += 9)
                            {
                                /* go right down */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (y = i + 7; y < 64 && Col (y) != 7; y += 7)
                            {
                                /* go left down */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        if (piece[i] == BISHOP)	/* In the case of the bishop we're done */
                            break;

                        /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
                    case ROOK:
                        col = Col (i);
                        for (k = i - col, y = i - 1; y >= k; y--)
                            {
                                /* go left */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (k = i - col + 7, y = i + 1; y <= k; y++)
                            {
                                /* go right */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (y = i - 8; y >= 0; y -= 8)
                            {
                                /* go up */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        for (y = i + 8; y < 64; y += 8)
                            {
                                /* go down */
                                if (color[y] != EMPTY)
                                    {
                                        if (color[y] != current_side)
                                            Gen_PushNormal (i, y, pBuf, &capscount);
                                        break;
                                    }
                            }
                        break;

                    case KING:
                        /* the column and rank checks are to make sure it is on the board */
                        col = Col (i);
                        if (col && color[i - 1] == xside)
                            Gen_PushKing (i, i - 1, pBuf, &capscount);	/* left */
                        if (col < 7 && color[i + 1] == xside)
                            Gen_PushKing (i, i + 1, pBuf, &capscount);	/* right */
                        if (i > 7 && color[i - 8] == xside)
                            Gen_PushKing (i, i - 8, pBuf, &capscount);	/* up */
                        if (i < 56 && color[i + 8] == xside)
                            Gen_PushKing (i, i + 8, pBuf, &capscount);	/* down */
                        if (col && i > 7 && color[i - 9] == xside)
                            Gen_PushKing (i, i - 9, pBuf, &capscount);	/* left up */
                        if (col < 7 && i > 7 && color[i - 7] == xside)
                            Gen_PushKing (i, i - 7, pBuf, &capscount);	/* right up */
                        if (col && i < 56 && color[i + 7] == xside)
                            Gen_PushKing (i, i + 7, pBuf, &capscount);	/* left down */
                        if (col < 7 && i < 56 && color[i + 9] == xside)
                            Gen_PushKing (i, i + 9, pBuf, &capscount);	/* right down */
                        break;
                    default:
                        printf("Piece type unknown");
                    }
            }

#ifdef BOARDDEBUG
    if (capscount > 200) printf ("Too much moves on GenCaps %d \n", capscount);
#endif

    return capscount;
}


/* Check if current side is in check. Necesary in order to check legality of moves
 and check if castle is allowed */
int IsInCheck (int current_side)
{
    int i = 0;
    int k = 0;			/* The square where the king is placed */

#ifdef BOARDDEBUG
    int kingWasFound = 0;
    if (current_side != BLACK && current_side != WHITE) printf ("GenMoves() wrong color");
#endif

    /* Find the King of the side to move */
    for (i = 0; i < 64; i++)
        if ((piece[i] == KING) && color[i] == current_side)
            {
                k = i;
#ifdef BOARDDEBUG
                kingWasFound = 1;
#endif
                break;
            }

#ifdef BOARDDEBUG
    if (k < 0) printf("IsInCheck() k < 0: %d\n", k);
    if (k > 63) printf("IsInCheck() k > 63: %d\n", k);
    if (kingWasFound == 0) printf("IsInCheck() The king was not found!\n");
#endif

    /* Use IsAttacked in order to know if current_side is under check */
    return IsAttacked (current_side, k);
}

/* Check and return 1 if square k is attacked by current_side, 0 otherwise. Necesary, vg, to check
 * castle rules (if king goes from e1 to g1, f1 can't be attacked by an enemy piece) */
int IsAttacked (int current_side, int k)
{
    int h = 0;
    int y = 0;
    int row = 0;			/* Row where the square is placed */
    int col = 0;			/* Col where the square is placed */
    int xside = Opponent(current_side);	/* opposite current_side, who may be attacking */

#ifdef BOARDDEBUG
    if (current_side != BLACK && current_side != WHITE) printf ("GenMoves() wrong color\n");
    if (xside != BLACK && xside != WHITE) printf ("GenMoves() wrong color\n");
    if (k < 0) printf("IsAttacked() k < 0: %d\n", k);
    if (k > 63) printf("IsAttacked() k > 63: %d\n", k);
#endif

    /* Situation of the square */
    row = Row (k);
    col = Col (k);

    /* Check Knight attack */
    if (col > 0 && row > 1 && color[k - 17] == xside && piece[k - 17] == KNIGHT)
        return 1;
    if (col < 7 && row > 1 && color[k - 15] == xside && piece[k - 15] == KNIGHT)
        return 1;
    if (col > 1 && row > 0 && color[k - 10] == xside && piece[k - 10] == KNIGHT)
        return 1;
    if (col < 6 && row > 0 && color[k - 6] == xside && piece[k - 6] == KNIGHT)
        return 1;
    if (col > 1 && row < 7 && color[k + 6] == xside && piece[k + 6] == KNIGHT)
        return 1;
    if (col < 6 && row < 7 && color[k + 10] == xside && piece[k + 10] == KNIGHT)
        return 1;
    if (col > 0 && row < 6 && color[k + 15] == xside && piece[k + 15] == KNIGHT)
        return 1;
    if (col < 7 && row < 6 && color[k + 17] == xside && piece[k + 17] == KNIGHT)
        return 1;

    /* Check horizontal and vertical lines for attacking of Queen, Rook, King */
    /* go down */
    y = k + 8;
    if (y < 64)
        {
            if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                                      || piece[y] == ROOK))
                return 1;
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y += 8; y < 64; y += 8)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    /* go left */
    y = k - 1;
    h = k - col;
    if (y >= h)
        {
            if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                                      || piece[y] == ROOK))
                return 1;
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y--; y >= h; y--)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    /* go right */
    y = k + 1;
    h = k - col + 7;
    if (y <= h)
        {
            if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                                      || piece[y] == ROOK))
                return 1;
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y++; y <= h; y++)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    /* go up */
    y = k - 8;
    if (y >= 0)
        {
            if (color[y] == xside && (piece[y] == KING || piece[y] == QUEEN
                                      || piece[y] == ROOK))
                return 1;
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y -= 8; y >= 0; y -= 8)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y] == ROOK))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    /* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
    /* go right down */
    y = k + 9;
    if (y < 64 && Col (y) != 0)
        {
            if (color[y] == xside)
                {
                    if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                        return 1;
                    if (current_side == BLACK && piece[y] == PAWN)
                        return 1;
                }
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y += 9; y < 64 && Col (y) != 0; y += 9)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                                                  == BISHOP))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    /* go left down */
    y = k + 7;
    if (y < 64 && Col (y) != 7)
        {
            if (color[y] == xside)
                {
                    if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                        return 1;
                    if (current_side == BLACK && piece[y] == PAWN)
                        return 1;
                }
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y += 7; y < 64 && Col (y) != 7; y += 7)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                                                  == BISHOP))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;

                    }
        }
    /* go left up */
    y = k - 9;
    if (y >= 0 && Col (y) != 7)
        {
            if (color[y] == xside)
                {
                    if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                        return 1;
                    if (current_side == WHITE && piece[y] == PAWN)
                        return 1;
                }
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y -= 9; y >= 0 && Col (y) != 7; y -= 9)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                                                  == BISHOP))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;

                    }
        }
    /* go right up */
    y = k - 7;
    if (y >= 0 && Col (y) != 0)
        {
            if (color[y] == xside)
                {
                    if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                        return 1;
                    if (current_side == WHITE && piece[y] == PAWN)
                        return 1;
                }
            if (piece[y] == EMPTY || piece[y] == EPS_SQ)
                for (y -= 7; y >= 0 && Col (y) != 0; y -= 7)
                    {
                        if (color[y] == xside && (piece[y] == QUEEN || piece[y]
                                                  == BISHOP))
                            return 1;
                        if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                            break;
                    }
        }
    return 0;
}

int MakeMove (MOVE m)
{
    int r = 0;

    count_MakeMove++;

#ifdef BOARDDEBUG
    int typeOfMove = m.type_of_move;
    if (typeOfMove > 8) printf ("type of move %d > 8\n", typeOfMove);
    if (typeOfMove < 0) printf ("type of move %d \n < 8", typeOfMove);
#endif

    hist[hdp].m = m;
    hist[hdp].cap = piece[m.dest];	/* store in history the piece of the dest square */
    hist[hdp].castle = castle;
    hist[hdp].fifty = fifty;
    hist[hdp].hash = hash.key;       /* Guardamos la posicion actial en hashkey */

    piece[m.dest] = piece[m.from];	/* dest piece is the one in the original square */
    color[m.dest] = color[m.from];	/* The dest square color is the one of the origin piece */
    piece[m.from] = EMPTY;	/* The original square becomes empty */
    color[m.from] = EMPTY;	/* The original color becomes empty */

    /* en pasant capture */
    if (m.type_of_move == MOVE_TYPE_EPS)
        {
            if (side == WHITE)
                {
                    piece[m.dest + 8] = EMPTY;
                    color[m.dest + 8] = EMPTY;
                }
            else
                {
                    piece[m.dest - 8] = EMPTY;
                    color[m.dest - 8] = EMPTY;
                }
        }

    /* Remove possible eps piece, remaining from former move */
    if (hist[hdp - 1].m.type_of_move == MOVE_TYPE_PAWN_TWO)
        {
            int i;
            for (i = 16; i <= 23; i++)
                {
                    if (piece[i] == EPS_SQ)
                        {
                            piece[i] = EMPTY;
                            break;
                        }
                }
            for (i = 40; i <= 47; i++)
                {
                    if (piece[i] == EPS_SQ)
                        {
                            piece[i] = EMPTY;
                            break;
                        }
                }
        }

    /* Add the eps square when a pawn moves two squares */
    if (m.type_of_move == MOVE_TYPE_PAWN_TWO)
        {
            if (side == BLACK)
                {
                    piece[m.from + 8] = EPS_SQ;
                    color[m.from + 8] = EMPTY;
                }
            else if (side == WHITE)
                {
                    piece[m.from - 8] = EPS_SQ;
                    color[m.from - 8] = EMPTY;
                }
        }

    /* Once the move is done we check either this is a promotion */
    if (m.type_of_move >= MOVE_TYPE_PROMOTION_TO_QUEEN)
        {
            /* In this case we put in the destiny sq the chosen piece */
            switch (m.type_of_move)
                {
                case MOVE_TYPE_PROMOTION_TO_QUEEN:
                    piece[m.dest] = QUEEN;
                    break;

                case MOVE_TYPE_PROMOTION_TO_ROOK:
                    piece[m.dest] = ROOK;
                    break;

                case MOVE_TYPE_PROMOTION_TO_BISHOP:
                    piece[m.dest] = BISHOP;
                    break;

                case MOVE_TYPE_PROMOTION_TO_KNIGHT:
                    piece[m.dest] = KNIGHT;
                    break;

                default:
                    puts ("Impossible to get here...");
                }
        }

    if (m.type_of_move == MOVE_TYPE_CASTLE)
        {
            if (m.dest == G1)
                {
                    /* h1-h8 becomes empty */
                    piece[m.from + 3] = EMPTY;
                    color[m.from + 3] = EMPTY;
                    /* rook to f1-f8 */
                    piece[m.from + 1] = ROOK;
                    color[m.from + 1] = WHITE;
                }
            else if (m.dest == C1)
                {
                    /* h1-h8 becomes empty */
                    piece[m.from - 4] = EMPTY;
                    color[m.from - 4] = EMPTY;
                    /* rook to f1-f8 */
                    piece[m.from - 1] = ROOK;
                    color[m.from - 1] = WHITE;
                }
            else if (m.dest == G8)
                {
                    /* h1-h8 becomes empty */
                    piece[m.from + 3] = EMPTY;
                    color[m.from + 3] = EMPTY;
                    /* rook to f1-f8 */
                    piece[m.from + 1] = ROOK;
                    color[m.from + 1] = BLACK;
                }
            else if (m.dest == C8)
                {
                    /* h1-h8 becomes empty */
                    piece[m.from - 4] = EMPTY;
                    color[m.from - 4] = EMPTY;
                    /* rook to f1-f8 */
                    piece[m.from - 1] = ROOK;
                    color[m.from - 1] = BLACK;
                }
        }

    /* Update ply and hdp */
    ply++;
    hdp++;

    /* Update the castle rights */
    castle &= castle_mask[m.from] & castle_mask[m.dest];

    /* Actualizamos la cuenta de la regla de 50 movimientos */
    if ((piece[m.dest] == PAWN) || (hist[hdp].cap != EMPTY))
        fifty = 0;
    else
        fifty++;

    /* Checking if after making the move we're in check */
    r = !IsInCheck (side);

    /* After making move, give turn to opponent */
    side = Opponent(side);

    /* Obtenemos la hash.key de la posicion actual */
    hash_key_position();

    return r;
}

/* Undo what MakeMove did */
void TakeBack ()
{
    side = Opponent(side);
    hdp--;
    ply--;

    /* Update piece moved and side to move */
    piece[hist[hdp].m.from] = piece[hist[hdp].m.dest];
    piece[hist[hdp].m.dest] = hist[hdp].cap;
    color[hist[hdp].m.from] = side;

    /* Update castle rights */
    castle = hist[hdp].castle;

    /* Return the captured material */
    if (hist[hdp].cap != EMPTY && hist[hdp].cap != EPS_SQ)
        {
            color[hist[hdp].m.dest] = (WHITE + BLACK) - side;
        }
    else
        {
            color[hist[hdp].m.dest] = EMPTY;
        }

    /* Promotion */
    if (hist[hdp].m.type_of_move >= MOVE_TYPE_PROMOTION_TO_QUEEN)
        {
            piece[hist[hdp].m.from] = PAWN;
        }

    /* If pawn moved two squares in the former move, we have to restore
     * the eps square */
    if (hist[hdp - 1].m.type_of_move == MOVE_TYPE_PAWN_TWO)
        {
            if (side == WHITE)
                {
                    piece[hist[hdp - 1].m.dest - 8] = EPS_SQ;
                }
            else if (side == BLACK)
                {
                    piece[hist[hdp - 1].m.dest + 8] = EPS_SQ;
                }
        }

    /* To remove the eps square after unmaking a pawn
     * moving two squares*/
    if (hist[hdp].m.type_of_move == MOVE_TYPE_PAWN_TWO)
        {
            if (side == WHITE)
                {
                    piece[hist[hdp].m.from - 8] = EMPTY;
                    color[hist[hdp].m.from - 8] = EMPTY;
                }
            else
                {
                    piece[hist[hdp].m.from + 8] = EMPTY;
                    color[hist[hdp].m.from + 8] = EMPTY;
                }
        }

    /* Unmaking an en pasant capture */
    if (hist[hdp].m.type_of_move == MOVE_TYPE_EPS)
        {
            if (side == WHITE)
                {
                    /* The pawn */
                    piece[hist[hdp].m.dest + 8] = PAWN;
                    color[hist[hdp].m.dest + 8] = BLACK;
                    /* The eps square */
                    piece[hist[hdp].m.dest] = EPS_SQ;
                }
            else
                {
                    /* The pawn */
                    piece[hist[hdp].m.dest - 8] = PAWN;
                    color[hist[hdp].m.dest - 8] = WHITE;
                    /* The eps square */
                    piece[hist[hdp].m.dest] = EPS_SQ;
                }
        }

    /* Undo Castle: return rook to its original square */
    if (hist[hdp].m.type_of_move == MOVE_TYPE_CASTLE)
        {
            /* Take the tower to its poriginal place */
            if (hist[hdp].m.dest == G1 && side == WHITE)
                {
                    piece[H1] = ROOK;
                    color[H1] = WHITE;
                    piece[F1] = EMPTY;
                    color[F1] = EMPTY;
                }
            else if (hist[hdp].m.dest == C1 && side == WHITE)
                {
                    piece[A1] = ROOK;
                    color[A1] = WHITE;
                    piece[D1] = EMPTY;
                    color[D1] = EMPTY;
                }
            else if (hist[hdp].m.dest == G8 && side == BLACK)
                {
                    piece[H8] = ROOK;
                    color[H8] = BLACK;
                    piece[F8] = EMPTY;
                    color[F8] = EMPTY;
                }
            else if (hist[hdp].m.dest == C8 && side == BLACK)
                {
                    piece[A8] = ROOK;
                    color[A8] = BLACK;
                    piece[D8] = EMPTY;
                    color[D8] = EMPTY;
                }
        }

    /* Ponemos la cuenta de 50 movimientos que llevabamos. */
    fifty = hist[hdp].fifty;

    /* Recuperamos la posicion anterior en zobrist key */
    hash.key = hist[hdp].hash;
}

