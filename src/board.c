#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"


/* Just returns the opponent's color */
inline int opponent(int color)
{
    return (!(color));
}

/* Sign returns 1 for White and -1 for Black */
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

/* isSqProtectedByAPawn returns 1 if sq is protected by a pawn of color side */
inline int isSqProtectedByAPawn(int sq, int side)
{
    /* We need to check the columm because in col 1 and 8 sq can only be attacked
     * from one side */
    int col = COL(sq);
    int row = ROW(sq);

    if (side == WHITE)
    {
        if ( row < 7)
        {
            if ( col != 7 && piece[sq + Behind(side) + 1] == PAWN && color[sq + Behind(side) + 1] == side )
                return 1;
            else if ( col != 0 && piece[sq + Behind(side) - 1] == PAWN && color[sq + Behind(side) - 1] == side )
                return 1;
        }
    }
    else if (side == BLACK)
    {
        if ( row > 0)
        {
            if ( col != 7 && piece[sq + Behind(side) + 1] == PAWN && color[sq + Behind(side) + 1] == side )
                return 1;
            else if ( col != 0 && piece[sq + Behind(side) - 1] == PAWN && color[sq + Behind(side) - 1] == side )
                return 1;
        }
    }

    return 0;
}

/* IsProtectedByAKnight returns 1 if square sq is protected by a knight of color side */
int IsSqProtectedByAKnight(int sq, int side)
{
    int y;
    int col = COL (sq);
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
    int y;
    /* Check diagonal lines for attacking of Queen, Bishop, King, Pawn */
    /* go right down */
    y = sq + 9;
    if (y < 64 && COL (y) != 0)
    {
        if (color[y] == side)
        {
            if (piece[y] == BISHOP)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y += 9; y < 64 && COL (y) != 0; y += 9)
            {
                if (piece[y] == BISHOP && color[y] == side)
                    return 1;
                if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                    break;
            }
    }
    /* go left down */
    y = sq + 7;
    if (y < 64 && COL (y) != 7)
    {
        if (color[y] == side)
        {
            if (piece[y] == BISHOP)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y += 7; y < 64 && COL (y) != 7; y += 7)
            {
                if (piece[y] == BISHOP && color[y] == side)
                    return 1;
                if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                    break;

            }
    }
    /* go left up */
    y = sq - 9;
    if (y >= 0 && COL (y) != 7)
    {
        if (color[y] == side)
        {
            if (piece[y] == BISHOP)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y -= 9; y >= 0 && COL (y) != 7; y -= 9)
            {
                if (piece[y] == BISHOP && color[y] == side)
                    return 1;
                if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                    break;

            }
    }
    /* go right up */
    y = sq - 7;
    if (y >= 0 && COL (y) != 0)
    {
        if (color[y] == side)
        {
            if (piece[y] == BISHOP)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y -= 7; y >= 0 && COL (y) != 0; y -= 7)
            {
                if (piece[y] == BISHOP && color[y] == side)
                    return 1;
                if (piece[y] != EMPTY && piece[y] != EPS_SQ)
                    break;
            }
    }

    return 0;
}

/* This function allows us to know whether a capture is bad based on CPW.
For the moment it substitutes SEE (Static Exchange Evaluator)*/
int badCapture(MOVE mcmov) {

    if (piece[mcmov.from] == PAWN)
        return 0;

    /* Capturing equal or more material is never bad */
    if (valuePiece[piece[mcmov.dest]] > valuePiece[piece[mcmov.from]] - 50)
        return 0;

    /* We're capturing a piece with less value than ours, so we want to
    know whether it is protected by a pawn? */
    if ( isSqProtectedByAPawn(mcmov.dest, color[mcmov.dest]) )
        return 1;

    /******************** **************************/
    /* Is the piece protected by a knight? */
    if ( IsSqProtectedByAKnight(mcmov.dest, color[mcmov.dest]) )
        return 1;

    /**********************************************/
    /* Is the piece protected by a bishop? */
    if ( IsSqProtectedByABishop(mcmov.dest, color[mcmov.dest]) )
        return 1;

    /* If we have reached so far then it isn't a bad capture */
    return 0;
}

/*
 ****************************************************************************
 * Move generator *
 ****************************************************************************
 */
void genPush(int from, int dest, int type, MOVE * pBuf, int *pMCount)
{
    MOVE move;
    move.from = from;
    move.dest = dest;
    move.type_of_move = type;

    /* Now we assign the value to 'grade' */
    move.grade = 0;

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
            /* Bad captures get a bad grade */
            if (badCapture(move) == 1)
            {
                move.grade = -10000000 + valuePiece[piece[dest]] - valuePiece[piece[from]];
            }
            /* Ordering by MVV/LVA */
            else
            {
                /* These are "good" captures, but we aren't taking into account factors
                 * such as if the captured piece is protected by a pawn: base on this
                 * kind of checkings we can give different levels of "goodness"? */
                move.grade = 3000000 + valuePiece[piece[dest]] - valuePiece[piece[from]];
                /* It isn't a bad capture, but if the captured piece is not protected
                 * by a pawn we give it an extra push */
                if (!isSqProtectedByAPawn(dest, opponent(color[from])))
                    move.grade += 1000;
            }
        }
        /* So it isn't a capture */
        else
        {
            /* Just to make tests with the difference */
            int mult = 1;

            /* Are we moving to a better/worse square of piece? */
            if (color[from] == WHITE)
            {
                switch (piece[from])
                {
                case PAWN:
                    if (isEndGame())
                        move.grade += pstPawnEndgame[dest] - pstPawnEndgame[from];
                    else
                        move.grade += pstPawnMidgame[dest] - pstPawnMidgame[from];
                    break;
                case KNIGHT:
                    move.grade += mult*(pstKnight[dest] - pstKnight[from]);
                    break;
                case BISHOP:
                    move.grade += mult*(pstBishop[dest] - pstBishop[from]);
                    break;
                case ROOK:
                    move.grade += mult*(pstRook[dest] - pstRook[from]);
                    break;
                case QUEEN:
                    move.grade += mult*(pstQueen[dest] - pstQueen[from]);
                    break;
                case KING:
                    if (isEndGame())
                        move.grade += mult*(pstKingEndgame[dest] - pstKingEndgame[from]);
                    else
                        move.grade += mult*(pstKingMidgame[dest] - pstKingMidgame[from]);
                    break;
                }
            }
            else if (color[from] == BLACK)
            {
                switch (piece[from])
                {
                case PAWN:
                    if (isEndGame())
                        move.grade += pstPawnEndgame[flip[dest]] - pstPawnEndgame[flip[from]];
                    else
                        move.grade += pstPawnMidgame[flip[dest]] - pstPawnMidgame[flip[from]];
                    break;
                case KNIGHT:
                    move.grade += mult*(pstKnight[flip[dest]] - pstKnight[flip[from]]);
                    break;
                case BISHOP:
                    move.grade += mult*(pstBishop[flip[dest]] - pstBishop[flip[from]]);
                    break;
                case ROOK:
                    move.grade += mult*(pstRook[flip[dest]] - pstRook[flip[from]]);
                    break;
                case QUEEN:
                    move.grade += mult*(pstQueen[flip[dest]] - pstQueen[flip[from]]);
                    break;
                case KING:
                    if (isEndGame())
                        move.grade += mult*(pstKingEndgame[flip[dest]] - pstKingEndgame[flip[from]]);
                    else
                        move.grade += mult*(pstKingMidgame[flip[dest]] - pstKingMidgame[flip[from]]);
                    break;
                }
            }

            /* Are we placing a piece on a square defended by a pawn? Sounds like a bad idea */
            if  ( piece[from] != PAWN && isSqProtectedByAPawn(dest, opponent(color[from])) )
                move.grade -= (valuePiece[piece[from]]);

            /* Is a piece attacked by a pawn? Then we probably should move it */
            if  ( piece[from] != PAWN && isSqProtectedByAPawn(from, opponent(color[from])) )
                move.grade += 100*(valuePiece[piece[from]]);

            else if  ( piece[from] == QUEEN || piece[from] == ROOK)
            {
                if (IsSqProtectedByABishop(from, opponent(color[from])))
                    move.grade += (valuePiece[piece[from]]);
                else if (IsSqProtectedByAKnight(from, opponent(color[from])) )
                    move.grade += (valuePiece[piece[from]]);
                else if  ( IsSqProtectedByABishop(dest, opponent(color[from])) )
                    move.grade -= (valuePiece[piece[from]]);
                else if  (IsSqProtectedByAKnight(dest, opponent(color[from])) )
                    move.grade -= (valuePiece[piece[from]]);
            }

            /* Finally we use the history */
            move.grade += 1* history[from][dest];
        }
    }

    /* Once we're done with grading the moves we fill the rest of the fields */
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void genPushNormal(int from, int dest, MOVE * pBuf, int *pMCount)
{
    genPush (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
}

/* Especial cases for Pawn */
/* Pawn can promote */
void genPushPawn(int from, int dest, MOVE * pBuf, int *pMCount)
{
    if (piece[dest] == EPS_SQ)
    {
        genPush (from, dest, MOVE_TYPE_EPS, pBuf, pMCount);
    }
    /* The 7 and 56 are to limit pawns to the 2nd through 7th ranks, which
     * means this isn't a promotion, i.e., a normal pawn move */
    else if (dest > 7 && dest < 56)
    {
        genPush (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    }
    else				/* otherwise it's a promotion */
    {
        genPush (from, dest, MOVE_TYPE_PROMOTION_TO_QUEEN, pBuf, pMCount);
        genPush (from, dest, MOVE_TYPE_PROMOTION_TO_ROOK, pBuf, pMCount);
        genPush (from, dest, MOVE_TYPE_PROMOTION_TO_BISHOP, pBuf, pMCount);
        genPush (from, dest, MOVE_TYPE_PROMOTION_TO_KNIGHT, pBuf, pMCount);
    }
}

/* When a pawn moves two squares ahead then the possibility of an en passant capture appears */
void genPushPawnTwo(int from, int dest, MOVE * pBuf, int *pMCount)
{
    genPush (from, dest, MOVE_TYPE_PAWN_TWO, pBuf, pMCount);
}

/* Especial cases for King */
void genPushKing(int from, int dest, MOVE * pBuf, int *pMCount)
{
    /* Is it a castle? */
    if (from == E1 && (dest == G1 || dest == C1))	/* this is a white castle */
    {
        genPush (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
    }
    else if (from == E8 && (dest == G8 || dest == C8))	/* this is a black castle */
    {
        genPush (from, dest, MOVE_TYPE_CASTLE, pBuf, pMCount);
    }
    else				/* otherwise it's a normal king's move */
    {
        genPush (from, dest, MOVE_TYPE_NORMAL, pBuf, pMCount);
    }
}

/* Gen all moves of current_side to move and push them to pBuf, and return number of moves */
int genMoves(int current_side, MOVE * pBuf)
{
    int i;			/* Counter for the board squares */
    int k;			/* Counter for cols */
    int y;
    int row;
    int col;
    int movecount;
    movecount = 0;

    for (i = 0; i < 64; i++)	/* Scan whole board */
        if (color[i] == current_side)
        {
            switch (piece[i])
            {

            case PAWN:
                col = COL (i);
                row = ROW (i);
                if (current_side == BLACK)
                {
                    if (color[i + 8] == EMPTY)
                        /* Pawn advances one square.
                         * We use genPushPawn because it can be a promotion */
                        genPushPawn (i, i + 8, pBuf, &movecount);
                    if (row == 1 && color[i + 8] == EMPTY
                            && color[i + 16] == EMPTY)
                        /* Pawn advances two squares */
                        genPushPawnTwo (i, i + 16, pBuf, &movecount);
                    if (col && color[i + 7] == WHITE)
                        /* Pawn captures and it can be a promotion */
                        genPushPawn (i, i + 7, pBuf, &movecount);
                    if (col < 7 && color[i + 9] == WHITE)
                        /* Pawn captures and can be a promotion */
                        genPushPawn (i, i + 9, pBuf, &movecount);
                        /* For en passant capture */
                    if (col && piece[i + 7] == EPS_SQ)
                        /* Pawn captures and it can be a promotion */
                        genPushPawn (i, i + 7, pBuf, &movecount);
                    if (col < 7 && piece[i + 9] == EPS_SQ)
                        /* Pawn captures and can be a promotion */
                        genPushPawn (i, i + 9, pBuf, &movecount);
                }
                else
                {
                    if (color[i - 8] == EMPTY)
                        genPushPawn (i, i - 8, pBuf, &movecount);
                    /* Pawn moves 2 squares */
                    if (row == 6 && color[i - 8] == EMPTY
                            && color[i - 16] == EMPTY)
                        genPushPawnTwo (i, i - 16, pBuf, &movecount);
                    /* For captures */
                    if (col && color[i - 9] == BLACK)
                        genPushPawn (i, i - 9, pBuf, &movecount);
                    if (col < 7 && color[i - 7] == BLACK)
                        genPushPawn (i, i - 7, pBuf, &movecount);
                    /* For en passant capture */
                    if (col && piece[i - 9] == EPS_SQ)
                        genPushPawn (i, i - 9, pBuf, &movecount);
                    if (col < 7 && piece[i - 7] == EPS_SQ)
                        genPushPawn (i, i - 7, pBuf, &movecount);
                }
                break;

            case QUEEN:		/* == BISHOP+ROOK */

            case BISHOP:
                for (y = i - 9; y >= 0 && COL (y) != 7; y -= 9)
                {   /* go left up */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (y = i - 7; y >= 0 && COL (y) != 0; y -= 7)
                {   /* go right up */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (y = i + 9; y < 64 && COL (y) != 0; y += 9)
                {   /* go right down */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (y = i + 7; y < 64 && COL (y) != 7; y += 7)
                {   /* go left down */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                if (piece[i] == BISHOP)	/* In the case of the bishop we're done */
                    break;

                /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
            case ROOK:
                col = COL (i);
                for (k = i - col, y = i - 1; y >= k; y--)
                {   /* go left */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (k = i - col + 7, y = i + 1; y <= k; y++)
                {   /* go right */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (y = i - 8; y >= 0; y -= 8)
                {   /* go up */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                for (y = i + 8; y < 64; y += 8)
                {   /* go down */
                    if (color[y] != current_side)
                        genPushNormal (i, y, pBuf, &movecount);
                    if (color[y] != EMPTY)
                        break;
                }
                break;

            case KNIGHT:
                col = COL (i);
                y = i - 6;
                if (y >= 0 && col < 6 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i - 10;
                if (y >= 0 && col > 1 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i - 15;
                if (y >= 0 && col < 7 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i - 17;
                if (y >= 0 && col > 0 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i + 6;
                if (y < 64 && col > 1 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i + 10;
                if (y < 64 && col < 6 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i + 15;
                if (y < 64 && col > 0 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                y = i + 17;
                if (y < 64 && col < 7 && color[y] != current_side)
                    genPushNormal (i, y, pBuf, &movecount);
                break;

            case KING:
                /* The column and rank checks are to make sure it is on the board */
                /* The 'normal' moves */
                col = COL (i);
                if (col && color[i - 1] != current_side)
                    genPushKing (i, i - 1, pBuf, &movecount);	/* left */
                if (col < 7 && color[i + 1] != current_side)
                    genPushKing (i, i + 1, pBuf, &movecount);	/* right */
                if (i > 7 && color[i - 8] != current_side)
                    genPushKing (i, i - 8, pBuf, &movecount);	/* up */
                if (i < 56 && color[i + 8] != current_side)
                    genPushKing (i, i + 8, pBuf, &movecount);	/* down */
                if (col && i > 7 && color[i - 9] != current_side)
                    genPushKing (i, i - 9, pBuf, &movecount);	/* left up */
                if (col < 7 && i > 7 && color[i - 7] != current_side)
                    genPushKing (i, i - 7, pBuf, &movecount);	/* right up */
                if (col && i < 56 && color[i + 7] != current_side)
                    genPushKing (i, i + 7, pBuf, &movecount);	/* left down */
                if (col < 7 && i < 56 && color[i + 9] != current_side)
                    genPushKing (i, i + 9, pBuf, &movecount);	/* right down */

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
                                !isInCheck (current_side) &&
                                !isAttacked (current_side, i + 1))
                        {
                            /* The king goes 2 sq to the left */
                            genPushKing (i, i + 2, pBuf, &movecount);
                        }
                    }

                    /* Can white long castle? */
                    if (castle & 2)
                    {
                        if (col &&
                                color[i - 1] == EMPTY &&
                                color[i - 2] == EMPTY &&
                                color[i - 3] == EMPTY &&
                                !isInCheck (current_side) &&
                                !isAttacked (current_side, i - 1))
                        {
                            /* The king goes 2 sq to the left */
                            genPushKing (i, i - 2, pBuf, &movecount);
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
                                !isInCheck (current_side) &&
                                !isAttacked (current_side, i + 1))
                        {
                            /* The king goes 2 sq to the left */
                            genPushKing (i, i + 2, pBuf, &movecount);
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
                                !isInCheck (current_side) &&
                                !isAttacked (current_side, i - 1))
                        {
                            /* The king goes 2 sq to the left */
                            genPushKing (i, i - 2, pBuf, &movecount);
                        }
                    }
                }

                break;
//                default:
//                printf("Piece type unknown, %d", piece[i]);
            }
        }
    return movecount;
}

/* Gen all captures of current_side to move and push them to pBuf, return number of moves
 * It's necessary at least in order to use quiescent in the search */
int genCaps(int current_side, MOVE * pBuf)
{
    int i;			/* Counter for the board squares */
    int k;			/* Counter for cols */
    int y;
    int row;
    int col;
    int capscount;		/* Counter for possible captures */
    int xside;
    xside = opponent(current_side);
    capscount = 0;

    for (i = 0; i < 64; i++)	/* Scan whole board */
        if (color[i] == current_side)
        {
            switch (piece[i])
            {

            case PAWN:
                col = COL (i);
                row = ROW (i);
                if (current_side == BLACK)
                {
                    /* This isn't a capture, but it's necessary in order to
                     * not overlook promotions */
                    if (row > 7 && color[i + 8] == EMPTY)
                        /* Pawn advances one square.
                         * We use genPushPawn because it can be a promotion */
                        genPushPawn (i, i + 8, pBuf, &capscount);
                    if (col && color[i + 7] == WHITE)
                        /* Pawn captures and it can be a promotion */
                        genPushPawn (i, i + 7, pBuf, &capscount);
                    if (col < 7 && color[i + 9] == WHITE)
                        /* Pawn captures and can be a promotion */
                        genPushPawn (i, i + 9, pBuf, &capscount);
                    /* For en passant capture */
                    if (col && piece[i + 7] == EPS_SQ)
                        /* Pawn captures and it can be a promotion */
                        genPushPawn (i, i + 7, pBuf, &capscount);
                    if (col < 7 && piece[i + 9] == EPS_SQ)
                        /* Pawn captures and can be a promotion */
                        genPushPawn (i, i + 9, pBuf, &capscount);
                }
                else if (current_side == WHITE)
                {
                    if (row < 2 && color[i - 8] == EMPTY)
                        /* This isn't a capture, but it's necesary in order to
                         * not overlook promotions */
                        genPushPawn (i, i - 8, pBuf, &capscount);
                    /* For captures */
                    if (col && color[i - 9] == BLACK)
                        genPushPawn (i, i - 9, pBuf, &capscount);
                    if (col < 7 && color[i - 7] == BLACK)
                        genPushPawn (i, i - 7, pBuf, &capscount);
                    /* For en passant capture */
                    if (col && piece[i - 9] == EPS_SQ)
                        genPushPawn (i, i - 9, pBuf, &capscount);
                    if (col < 7 && piece[i - 7] == EPS_SQ)
                        genPushPawn (i, i - 7, pBuf, &capscount);
                }
                break;

            case KNIGHT:
                col = COL (i);
                y = i - 6;
                if (y >= 0 && col < 6 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i - 10;
                if (y >= 0 && col > 1 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i - 15;
                if (y >= 0 && col < 7 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i - 17;
                if (y >= 0 && col > 0 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i + 6;
                if (y < 64 && col > 1 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i + 10;
                if (y < 64 && col < 6 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i + 15;
                if (y < 64 && col > 0 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                y = i + 17;
                if (y < 64 && col < 7 && color[y] == xside)
                    genPushNormal (i, y, pBuf, &capscount);
                break;

            case QUEEN:		/* == BISHOP + ROOK */

            case BISHOP:
                for (y = i - 9; y >= 0 && COL (y) != 7; y -= 9)
                {   /* go left up */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (y = i - 7; y >= 0 && COL (y) != 0; y -= 7)
                {   /* go right up */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (y = i + 9; y < 64 && COL (y) != 0; y += 9)
                {   /* go right down */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (y = i + 7; y < 64 && COL (y) != 7; y += 7)
                {   /* go left down */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                if (piece[i] == BISHOP)	/* In the case of the bishop we're done */
                    break;

                /* FALL THROUGH FOR QUEEN {I meant to do that!} ;-) */
            case ROOK:
                col = COL (i);
                for (k = i - col, y = i - 1; y >= k; y--)
                {   /* go left */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (k = i - col + 7, y = i + 1; y <= k; y++)
                {   /* go right */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (y = i - 8; y >= 0; y -= 8)
                {   /* go up */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                for (y = i + 8; y < 64; y += 8)
                {   /* go down */
                    if (color[y] != EMPTY)
                    {
                        if (color[y] != current_side)
                            genPushNormal (i, y, pBuf, &capscount);
                        break;
                    }
                }
                break;

            case KING:
                /* the column and rank checks are to make sure it is on the board */
                col = COL (i);
                if (col && color[i - 1] == xside)
                    genPushKing (i, i - 1, pBuf, &capscount);	/* left */
                if (col < 7 && color[i + 1] == xside)
                    genPushKing (i, i + 1, pBuf, &capscount);	/* right */
                if (i > 7 && color[i - 8] == xside)
                    genPushKing (i, i - 8, pBuf, &capscount);	/* up */
                if (i < 56 && color[i + 8] == xside)
                    genPushKing (i, i + 8, pBuf, &capscount);	/* down */
                if (col && i > 7 && color[i - 9] == xside)
                    genPushKing (i, i - 9, pBuf, &capscount);	/* left up */
                if (col < 7 && i > 7 && color[i - 7] == xside)
                    genPushKing (i, i - 7, pBuf, &capscount);	/* right up */
                if (col && i < 56 && color[i + 7] == xside)
                    genPushKing (i, i + 7, pBuf, &capscount);	/* left down */
                if (col < 7 && i < 56 && color[i + 9] == xside)
                    genPushKing (i, i + 9, pBuf, &capscount);	/* right down */
                break;
//                               default:
//                               printf("Piece type unknown");
            }
        }
    return capscount;
}


/* Check if current side is in check. Necessary in order to check legality of moves
   and check if castle is allowed */
int isInCheck(int current_side)
{
    int k;			/* The square where the king is placed */

    /* Find the King of the side to move */
    for (k = 0; k < 64; k++)
        if ((piece[k] == KING) && color[k] == current_side)
            break;

    /* Use isAttacked in order to know if current_side is under check */
    return isAttacked (current_side, k);
}

/* Check and return 1 if square k is attacked by current_side, 0 otherwise. Necessary, vg, to check
 * castle rules (if king goes from e1 to g1, f1 may not be attacked by an enemy piece) */
int isAttacked(int current_side, int k)
{
    int h;
    int y;
    int row;			/* ROW where the square is placed */
    int col;			/* COL where the square is placed */
    int xside;
    xside = opponent(current_side);	/* opposite current_side, who may be attacking */

    /* Situation of the square */
    row = ROW (k);
    col = COL (k);

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
    if (y < 64 && COL (y) != 0)
    {
        if (color[y] == xside)
        {
            if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                return 1;
            if (current_side == BLACK && piece[y] == PAWN)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y += 9; y < 64 && COL (y) != 0; y += 9)
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
    if (y < 64 && COL (y) != 7)
    {
        if (color[y] == xside)
        {
            if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                return 1;
            if (current_side == BLACK && piece[y] == PAWN)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y += 7; y < 64 && COL (y) != 7; y += 7)
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
    if (y >= 0 && COL (y) != 7)
    {
        if (color[y] == xside)
        {
            if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                return 1;
            if (current_side == WHITE && piece[y] == PAWN)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y -= 9; y >= 0 && COL (y) != 7; y -= 9)
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
    if (y >= 0 && COL (y) != 0)
    {
        if (color[y] == xside)
        {
            if (piece[y] == KING || piece[y] == QUEEN || piece[y] == BISHOP)
                return 1;
            if (current_side == WHITE && piece[y] == PAWN)
                return 1;
        }
        if (piece[y] == EMPTY || piece[y] == EPS_SQ)
            for (y -= 7; y >= 0 && COL (y) != 0; y -= 7)
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

int makeMove(MOVE m)
{
    int r;

    countMakeMove++;

    int typeOfMove = m.type_of_move;
//    if (typeOfMove > 8)
//        printf ("type of move %d \n", typeOfMove);

    hist[hdp].m = m;
    hist[hdp].cap = piece[m.dest];	/* Store in history the piece of the dest square */
    hist[hdp].castle = castle;
    hist[hdp].fifty = fifty;
    hist[hdp].hashhist = hash.key;       /* Save tue current position in a hashkey */

    piece[m.dest] = piece[m.from];	/* dest piece is the one in the original square */
    color[m.dest] = color[m.from];	/* The dest square color is the one of the origin piece */
    piece[m.from] = EMPTY;	/* The original square becomes empty */
    color[m.from] = EMPTY;	/* The original color becomes empty */

    /* Remove possible eps piece remaining from former move */
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

    /* En passant capture */
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

    /* Add the eps square when a pawn moves two squares */
    else if (typeOfMove == MOVE_TYPE_PAWN_TWO)
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

    /* Once the move is done we check whether this is a promotion */
    else if (typeOfMove >= MOVE_TYPE_PROMOTION_TO_QUEEN)
    {
        /* In this case we put in the destiny sq for the chosen piece */
        switch (typeOfMove)
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

    else if (typeOfMove == MOVE_TYPE_CASTLE)
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
    castle &= castleMask[m.from] & castleMask[m.dest];

    /* Update the counting for the 50 moves rule */
    if ((piece[m.dest] == PAWN) || (hist[hdp].cap != EMPTY))
       fifty = 0;
    else
       fifty++;

    /* Checking if we're in check after moving */
    r = !isInCheck (side);

    /* After making move, give turn to opponent */
    side = opponent(side);

    /* Get hash.key for the current position */
    hashKeyPosition();

    return r;
}

/* Undo what makeMove did */
void takeBack()
{
    side = opponent(side);
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

    /* If pawn moved two squares in the former move we have to restore
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
     * moving two squares */
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

    /* Unmaking an en passant capture */
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
        /* Take the rook to its original place */
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

    /* Recover the 50 moves counting */
    fifty = hist[hdp].fifty;

    /* Recover the former position in zobrist key */
    hash.key = hist[hdp].hashhist;
}

void testisSqProtectedByAPawn()
{
    int i, j;
    for (i=0; i < 64; i++)
    {
        for (j = WHITE; j <= BLACK; j++)
        {
            if ( isSqProtectedByAPawn(i, j) )
                printf ("Square %d is protected by pawn of side %d \n", i, j);
        }
    }
}

void testIsSqProtectedByAKnight()
{
    int i, j;
    for (i=0; i < 64; i++)
    {
        for (j = WHITE; j <= BLACK; j++)
        {
            if ( IsSqProtectedByAKnight(i, j) )
                printf ("Square %d is protected by knight of side %d \n", i, j);
        }
    }
}

void testIsSqProtectedByABishop()
{
    int i, j;
    for (i=0; i < 64; i++)
    {
        for (j = WHITE; j <= BLACK; j++)
        {
            if ( IsSqProtectedByABishop(i, j) )
                printf ("Square %d is protected by bishop of side %d \n", i, j);
        }
    }
}
