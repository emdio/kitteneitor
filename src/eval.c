#include <stdio.h>
#include "defs.h"
#include "data.h"
#include "protos.h"

/*
 ****************************************************************************
 * Evaluation for current position - main "brain" function *
 ****************************************************************************
 */

/* Bonus and malus */
#define	ROOK_OPEN_COL                   18
#define PAIR_BISHOPS                    25
#define ADV_TURN_TO_MOVE                10
#define DOUBLED_PAWN_MALUS             -10
#define DOUBLED_PAWN_CASTLE_MALUS      -25
#define MISSING_PAWN_CASTLE_MALUS      -55
#define HOLE_C3_C6_F3_F6               -30
#define HOLE_B3_B6_G3_G6               -30
#define TRAPPED_ROOK_PENALTY           -70

/* Arrays for scaling mobility values */
int mobRook[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};
int mobKnight[9] = {
    -10, -4, 2, 8, 14, 18, 22, 24, 25
};
int mobBishop[16] = {
    -5, -2, 0, 3, 6, 10, 14, 20, 24, 28, 31, 35, 40, 42, 45, 47
};
int rangeBishop[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};

/* For scaling passed pawns depending on the row */
int passedPawnBonus[2][7] = {90, 75, 55, 25, 17, 10, 0};

/* For storing pawns' ranks */
int pawnsRanks[2][10];

/* For scaling pawn number in fun factor */
int numPawnsFunfac[16] = {0, 0, 0, 0, 0, 0, 0, 0, -5, -5, -10, -10, -12, -12, -15, -15};

/* Kings' safety */
int colKing[2] = 0;

/* To count the material */
int pawns[2] = 0;
int knights[2] = 0;
int bishops[2] = 0;
int rooks[2] = 0;
int queens[2] = 0;

/* The evaluation function */
int Eval(alpha, beta)
{
    /* A traditional counter */
    int i;

    /* Set some values to 0 */

    /* Pawn's info */
    for (i = 0; i < 10; ++i)
    {
        pawnsRanks[WHITE][i] = 0;
        pawnsRanks[BLACK][i] = 7;
    }

    /* The fun factor */
    int ff = 0;

    /* The vars for counting the material */
    pawns[WHITE] = 0;
    knights[WHITE] = 0;
    bishops[WHITE] = 0;
    rooks[WHITE] = 0;
    queens[WHITE] = 0;
    pawns[BLACK] = 0;
    knights[BLACK] = 0;
    bishops[BLACK] = 0;
    rooks[BLACK] = 0;
    queens[BLACK] = 0;

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
                pawns[WHITE]++;
//                pawns[WHITE]Info[(int)Col(i)] += 1<<Row(63 - i);
                getWhitePawnRank(i);
                break;
            case KNIGHT:
                knights[WHITE]++;
                break;
            case BISHOP:
                bishops[WHITE]++;
                break;
            case ROOK:
                rooks[WHITE]++;
                break;
            case QUEEN:
                queens[WHITE]++;
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
                pawns[BLACK]++;
//                pawns[BLACK]Info[(int)Col(i)] += 1<<Row(i);
                getBlackPawnRank(i);
                break;
            case KNIGHT:
                knights[BLACK]++;
                break;
            case BISHOP:
                bishops[BLACK]++;
                break;
            case ROOK:
                rooks[BLACK]++;
                break;
            case QUEEN:
                queens[BLACK]++;
                break;
            case KING:
                posBlackKing = i;
                colBlackKing = Col(i);
                break;
            }
        }
    }

//    printf("\nBlack pawns: ");
//    for (i=0; i<10; ++i)
//    {
//        printf("%4d", pawns[BLACK]Ranks[i]);
//    }
//    puts("");

//    printf ("White pawns: ");
//    for (i=0; i<10; ++i)
//    {
//        printf("%4d", pawns[WHITE]Ranks[i]);
//    }
//    puts("");


    /* After counting the material we update the score */
//    score = piece_mat[WHITE] - piece_mat[BLACK];
    score = (pawns[WHITE] - pawns[BLACK]) * value_piece[PAWN] +
            (knights[WHITE] - knights[BLACK]) * value_piece[KNIGHT] +
            (bishops[WHITE] - bishops[BLACK]) * value_piece[BISHOP] +
            (rooks[WHITE] - rooks[BLACK]) * value_piece[ROOK] +
            (queens[WHITE] - queens[BLACK]) * value_piece[QUEEN];

    /* Is there enough material to keep on playing? */
    if (NoMaterial()) return 0;

    /* Anyone has the pair of bishops? */
    if (bishops[WHITE] == 2 && bishops[BLACK] != 2)
        score += PAIR_BISHOPS;
    else if (bishops[WHITE] == 2 && bishops[BLACK] != 2)
        score -= PAIR_BISHOPS;

    /* Trying the lazy eval */
    int lazy = score;
    if (side == BLACK) lazy = -lazy;
    if ( ( lazy + 300 < alpha ) ||
         ( lazy - 300 > beta  ) )
    {
        return lazy;
    }

    /* Check all the squares searching for the pieces */
    for (i = 0; i < 64; i++)
    {
        if (color[i] == EMPTY)
            continue;
        else if (color[i] == WHITE)
        {
            /* Now we add to the evaluation the value of the
             * piece square tables */
            switch (piece[i])
            {
            case PAWN:
                if (isDoubledPawnWhite(Col(i)))
                    score += DOUBLED_PAWN_MALUS;
                if (isPassedPawnWhite(i))
                    score += passed_pawn_white[Row(i)];
                if (endGame())
                    score += pst_pawn_endgame[i];
                else
                    score += pst_pawn_midgame[i];
                break;
            case KNIGHT:
                score += pst_knight[i];
                score += mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score += pst_bishop[i];
                score += mob_bishop[BishopMobility(i)];
                score += range_bishop[BishopRange(i)];
                break;
            case ROOK:
                score += pst_rook[i];
                score += mob_rook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score += ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H1 || i == G1)
                {
                    if(colWhiteKing > E1 && colWhiteKing < H1)
                        score += TRAPPED_ROOK_PENALTY;
                }

                break;
            case QUEEN:
                score += pst_queen[i];
                break;
            case KING:
                if (endGame())
                    score += pst_king_endgame[i];
                else
                {
                    score += whiteKingSafety();
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
                    score -= DOUBLED_PAWN_MALUS;
                if (isPassedPawnBlack(i))
                    score -= passed_pawn_black[Row(i)];
                if (endGame())
                    score -= pst_pawn_endgame[flip[i]];
                else
                    score -= pst_pawn_midgame[flip[i]];
                break;
            case KNIGHT:
                score -= pst_knight[flip[i]];
                score -= mob_knight[KnightMobility(i)];
                break;
            case BISHOP:
                score -= pst_bishop[flip[i]];
                score -= mob_bishop[BishopMobility(i)];
                score -= range_bishop[BishopRange(i)];
                break;
            case ROOK:
                score -= pst_rook[flip[i]];
                score -= mob_rook[RookMobility(i)];
                /* Is it on an open col? */
                if (isOnAnOpenCol(i))
                    score -= ROOK_OPEN_COL;
                /* Rook trapped by king */
                if(i == H8 || i == G8)
                {
                    if(colBlackKing > E8 && colBlackKing < H8)
                        score -= TRAPPED_ROOK_PENALTY;
                }
                break;
            case QUEEN:
                score -= pst_queen[flip[i]];
                break;
            case KING:
                if (endGame())
                    score -= pst_king_endgame[flip[i]];
                else
                {
                    score += blackKingSafety();
                    score -= pst_king_midgame[flip[i]];
                }
                break;
            }
        }
    }

    /* Finally we return the score, taking into account the side to move
        We add an extra plus because in the same position the side to
        move has some extra advantage*/

//    ff = funFactor();

//    if (side == computer_side)
//    {
//        if (side == WHITE)
//            return (score + ff + ADV_TURN_TO_MOVE);
//        return (-score - ff - ADV_TURN_TO_MOVE);
//    }
//    else
//    {
//        if (side == WHITE)
//            return (score + ADV_TURN_TO_MOVE);
//        return (-score - ADV_TURN_TO_MOVE);
//    }

    if (side == WHITE)
        return (score );
    return -score;
}

/* Returns 1 if the pawn of color color in square sq is passed */
int isPassedPawnWhite(int sq)
{
    /* Special case, pawn in A row */
    if (Col(sq) == 0)
    {
        if ( pawns[BLACK]Info[Col(sq)] == 0 &&
             (pawns[BLACK]Info[Col(sq+1)] >= 1<<Row(sq) || pawns[BLACK]Info[Col(sq) + 1] == 0) )
            return 1;
    }
    /* Special case, pawn in H row */
    else if (Col(sq) == 7)
    {
        if ( pawns[BLACK]Info[Col(sq)] == 0 &&
           (pawns[BLACK]Info[Col(sq-1)] >= 1<<Row(sq) || pawns[BLACK]Info[Col(sq) - 1] == 0) )
            return 1;
    }
    else
    {
        if ( pawns[BLACK]Info[Col(sq)] == 0 &&
             (pawns[BLACK]Info[Col(sq) - 1] >= 1<<Row(sq) || pawns[BLACK]Info[Col(sq) - 1] == 0) &&
             (pawns[BLACK]Info[Col(sq) + 1] >= 1<<Row(sq) || pawns[BLACK]Info[Col(sq) + 1] == 0) )
            return 1;
    }
    return 0;
}

int isPassedPawnBlack(int sq)
{
    /* Special case, pawn in A row */
    if (Col(sq) == 0)
    {
        if ( pawns[WHITE]Info[Col(sq)] == 0 &&
             (pawns[WHITE]Info[Col(sq+1)] <= 1<<Row(sq) || pawns[WHITE]Info[Col(sq) + 1] == 0) )
            return 1;
    }
    /* Special case, pawn in H row */
    else if (Col(sq) == 7)
    {
        if ( pawns[WHITE]Info[Col(sq)] == 0 &&
             (pawns[WHITE]Info[Col(sq-1)] <= 1<<Row(sq) || pawns[WHITE]Info[Col(sq) - 1] == 0) )
            return 1;
    }
    else
    {
//        int i;
//        for (i=0; i<8; i++)
//            printf("pawns[WHITE]Info col %d: %d\n", i+1, pawns[WHITE]Info[i]);
//        puts("-----------------------");
//        for (i=0; i<8; i++)
//            printf("pawns[BLACK]Info col %d: %d\n", i+1, pawns[BLACK]Info[i]);

        if ( pawns[WHITE]Info[Col(sq)] == 0 &&
             (pawns[WHITE]Info[Col(sq) - 1] <= 1<<Row(sq) || pawns[WHITE]Info[Col(sq) - 1] == 0) &&
             (pawns[WHITE]Info[Col(sq) + 1] <= 1<<Row(sq) || pawns[WHITE]Info[Col(sq) + 1] == 0) )
            return 1;
    }
    return 0;
}

/*
     KINGS' SAFETY
*/

int whiteKingSafety()
{
    int safety = 0;

    /* The king long castled */
    if (colWhiteKing < COLD)
    {
        safety += whiteKingLongCastle();
    }

    /* The king short castled */
    else if (colWhiteKing > COLE)
    {
        safety += whiteKingShortCastle();
    }

    else /* The king is in the middle of the board */
    {
        /* Open cols close to the king */
        if (pawns[WHITE]Info[COLD] == 0)
        {
            safety -= 15;
            if (pawns[BLACK]Info[COLD] == 0)
            {
                safety -= 35;
            }
        }
        if (pawns[WHITE]Info[COLE] == 0)
        {
            safety -= 15;
            if (pawns[BLACK]Info[COLE] == 0)
            {
                safety -= 35;
            }
        }
    }

    return safety;
}

int blackKingSafety()
{
    int safety = 0;

    /* The king long castled */
    if (colBlackKing < COLD)
    {
        safety += blackKingLongCastle();
    }

    /* The king short castled */
    else if (colBlackKing > COLE)
    {
        safety += blackKingShortCastle();
    }

    /* The king in the middle of the board */
    else
    {
        /* Open cols close to the king */
        if (pawns[BLACK]Info[COLD] == 0)
        {
            safety += 15;
            if (pawns[WHITE]Info[COLD] == 0)
            {
                safety += 35;
            }
        }
        if (pawns[BLACK]Info[COLE] == 0)
        {
            safety += 15;
            if (pawns[WHITE]Info[COLE] == 0)
            {
                safety += 35;
            }
        }
    }

    return safety;
}

/* Kings' long castle */

int whiteKingLongCastle()
{
    int whiteKingSafety = 0;
    int isOpp = isOppCastles();

        /* Hole in c3 */
        if ( !IsSqProtectedByAPawn(C3, WHITE) )
        {
            whiteKingSafety += HOLE_C3_C6_F3_F6;
            /* Extra penal if hole is attacked by an enemy pawn */
            if ( IsSqProtectedByAPawn(C3, BLACK) )
            {
                whiteKingSafety += HOLE_C3_C6_F3_F6;
            }
        }

        /* Hole in b3 */
        if ( !IsSqProtectedByAPawn(B3, WHITE) )
            whiteKingSafety += HOLE_B3_B6_G3_G6;

        /* Pawns shield */
        // Col A
        if (pawns[WHITE]Info[COLA] == 64) whiteKingSafety += 12;
        else if (pawns[WHITE]Info[COLA] == 32) whiteKingSafety += 6;
        else if (pawns[WHITE]Info[COLA] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;
        // Col B
        if (pawns[WHITE]Info[COLB] == 64) whiteKingSafety +=12;
        else if (pawns[WHITE]Info[COLB] == 32) whiteKingSafety += 6;
        else if (pawns[WHITE]Info[COLB] == 0) whiteKingSafety += 2*MISSING_PAWN_CASTLE_MALUS;
        // Col C
        if (pawns[WHITE]Info[COLC] == 64) whiteKingSafety +=12;
        else if (pawns[WHITE]Info[COLC] == 32) whiteKingSafety += 6;
        else if (pawns[WHITE]Info[COLC] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;

        /* Doubled pawns on castle */
        if (isDoubledPawnWhite(COLA)) whiteKingSafety += 3*DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLB)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLC)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;

        /* Semiopen cols by the oponent */
        if (pawns[BLACK]Info[COLA] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }
        if (pawns[BLACK]Info[COLB] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }
        if (pawns[BLACK]Info[COLC] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }

        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (isOpp)
        {
            if (pawns[WHITE]Info[COLA] == 0 && pawns[BLACK]Info[COLA] == 0) whiteKingSafety -= 35;
            if (pawns[WHITE]Info[COLB] == 0 && pawns[BLACK]Info[COLB] == 0) whiteKingSafety -= 35;
            if (pawns[WHITE]Info[COLC] == 0 && pawns[BLACK]Info[COLC] == 0) whiteKingSafety -= 35;
        }

        /* Pawns storm */
        if (piece[A4] == PAWN && color[A4] == BLACK)
            whiteKingSafety -= 15;
        if (piece[B4] == PAWN && color[B4] == BLACK)
            whiteKingSafety -= 15;
        if (piece[C4] == PAWN && color[C4] == BLACK)
            whiteKingSafety -= 15;
        if (piece[A3] == PAWN && color[A3] == BLACK)
            whiteKingSafety -= 25;
        if (piece[B3] == PAWN && color[B3] == BLACK)
            whiteKingSafety -= 25;
        if (piece[C3] == PAWN && color[C3] == BLACK)
            whiteKingSafety -= 25;

    return whiteKingSafety;
}

int blackKingLongCastle()
{
    int blackKingSafety = 0;
    int isOpp = isOppCastles();

    /* Hole in c6 */
    if ( !IsSqProtectedByAPawn(C6, BLACK) )
    {
        blackKingSafety -= HOLE_C3_C6_F3_F6;
        /* Extra penal if hole is attacked by an enemy pawn */
        if ( IsSqProtectedByAPawn(C6, WHITE) )
        {
            blackKingSafety -= HOLE_C3_C6_F3_F6;
        }
    }

    /* Hole in b6 */
    if ( !IsSqProtectedByAPawn(B6, BLACK) )
        blackKingSafety -= HOLE_B3_B6_G3_G6;

    /* Pawns shield */
    // Col A
    if (pawns[BLACK]Info[COLA] == 2) blackKingSafety -= 12;
    else if (pawns[BLACK]Info[COLA] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLA] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;
    // Col B
    if (pawns[BLACK]Info[COLB] == 2) blackKingSafety -= 12;
    else if (pawns[BLACK]Info[COLB] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLB] == 0) blackKingSafety -= 2*MISSING_PAWN_CASTLE_MALUS;
    // Col C
    if (pawns[BLACK]Info[COLC] == 2) blackKingSafety -= 12;
    else if (pawns[BLACK]Info[COLC] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLC] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnBlack(COLA)) blackKingSafety -= 3*DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLB)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLC)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (pawns[WHITE]Info[COLA] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (pawns[WHITE]Info[COLB] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (pawns[WHITE]Info[COLC] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (pawns[WHITE]Info[COLA] == 0 && pawns[BLACK]Info[COLA] == 0) blackKingSafety += 35;
        if (pawns[WHITE]Info[COLB] == 0 && pawns[BLACK]Info[COLB] == 0) blackKingSafety += 35;
        if (pawns[WHITE]Info[COLC] == 0 && pawns[BLACK]Info[COLC] == 0) blackKingSafety += 35;
    }

    /* Pawns storm */
    if (piece[A5] == PAWN && color[A5] == WHITE)
        blackKingSafety += 15;
    if (piece[B5] == PAWN && color[B5] == WHITE)
        blackKingSafety += 15;
    if (piece[C5] == PAWN && color[C5] == WHITE)
        blackKingSafety += 15;
    if (piece[A6] == PAWN && color[A6] == WHITE)
        blackKingSafety += 25;
    if (piece[B6] == PAWN && color[B6] == WHITE)
        blackKingSafety += 25;
    if (piece[C6] == PAWN && color[C6] == WHITE)
        blackKingSafety += 25;

    return blackKingSafety;
}

/* Kings' short castle */

int whiteKingShortCastle()
{
    int whiteKingSafety = 0;
    int isOpp = isOppCastles();

    /* Hole in f3 */
    if ( !IsSqProtectedByAPawn(F3, WHITE) )
    {
        whiteKingSafety += HOLE_C3_C6_F3_F6;
        /* Extra penal if hole is attacked by an enemy pawn */
        if ( IsSqProtectedByAPawn(F3, BLACK) )
        {
            whiteKingSafety += HOLE_C3_C6_F3_F6;
        }
    }

    /* Hole in g3 */
    if ( !IsSqProtectedByAPawn(G3, WHITE) )
        whiteKingSafety += HOLE_B3_B6_G3_G6;

    /* Pawns shield */
    // Col F
    if (pawns[WHITE]Info[COLF] == 64) whiteKingSafety += 12;
    else if (pawns[WHITE]Info[COLF] == 32) whiteKingSafety += 6;
    else if (pawns[WHITE]Info[COLF] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;
    // Col G
    if (pawns[WHITE]Info[COLG] == 64) whiteKingSafety +=12;
    else if (pawns[WHITE]Info[COLG] == 32) whiteKingSafety += 6;
    else if (pawns[WHITE]Info[COLG] == 0) whiteKingSafety += 2*MISSING_PAWN_CASTLE_MALUS;
    // Col H
    if (pawns[WHITE]Info[COLH] == 64) whiteKingSafety +=12;
    else if (pawns[WHITE]Info[COLH] == 32) whiteKingSafety += 6;
    else if (pawns[WHITE]Info[COLH] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnWhite(COLF)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnWhite(COLG)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnWhite(COLH)) whiteKingSafety += 3*DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (pawns[BLACK]Info[COLF] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }
    if (pawns[BLACK]Info[COLG] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }
    if (pawns[BLACK]Info[COLH] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (pawns[WHITE]Info[COLF] == 0 && pawns[BLACK]Info[COLF] == 0) whiteKingSafety -= 135;
        if (pawns[WHITE]Info[COLG] == 0 && pawns[BLACK]Info[COLG] == 0) whiteKingSafety -= 135;
        if (pawns[WHITE]Info[COLH] == 0 && pawns[BLACK]Info[COLH] == 0) whiteKingSafety -= 235;
    }

    /* Pawns storm */
    if (piece[F4] == PAWN && color[F4] == BLACK)
        whiteKingSafety -= 15;
    if (piece[G4] == PAWN && color[G4] == BLACK)
        whiteKingSafety -= 15;
    if (piece[H4] == PAWN && color[H4] == BLACK)
        whiteKingSafety -= 15;
    if (piece[F3] == PAWN && color[F3] == BLACK)
        whiteKingSafety -= 25;
    if (piece[G3] == PAWN && color[G3] == BLACK)
        whiteKingSafety -= 25;
    if (piece[H3] == PAWN && color[H3] == BLACK)
        whiteKingSafety -= 25;
}

int blackKingShortCastle()
{
    int blackKingSafety = 0;
    int isOpp = isOppCastles();

    /* Hole in f6 */
    if ( !IsSqProtectedByAPawn(F6, BLACK) )
    {
        blackKingSafety -= HOLE_C3_C6_F3_F6;
        /* Extra penal if hole is attacked by an enemy pawn */
        if ( IsSqProtectedByAPawn(F6, WHITE) )
        {
            blackKingSafety -= HOLE_C3_C6_F3_F6;
        }
    }

    /* Hole in g6 */
    if ( !IsSqProtectedByAPawn(G6, BLACK) )
        blackKingSafety -= HOLE_B3_B6_G3_G6;

    /* Pawns shield */
    // Col F
    if (pawns[BLACK]Info[COLF] == 2) blackKingSafety -= 12;
    else if (pawns[BLACK]Info[COLF] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLF] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;
    // Col G
    if (pawns[BLACK]Info[COLG] == 2) blackKingSafety -=12;
    else if (pawns[BLACK]Info[COLG] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLG] == 0) blackKingSafety -= 2*MISSING_PAWN_CASTLE_MALUS;
    // Col H
    if (pawns[BLACK]Info[COLH] == 2) blackKingSafety -=12;
    else if (pawns[BLACK]Info[COLH] == 4) blackKingSafety -= 6;
    else if (pawns[BLACK]Info[COLH] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnBlack(COLF)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLG)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLH)) blackKingSafety -= 3*DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (pawns[WHITE]Info[COLF] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (pawns[WHITE]Info[COLG] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (pawns[WHITE]Info[COLH] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (pawns[WHITE]Info[COLF] == 0 && pawns[BLACK]Info[COLF] == 0) blackKingSafety += 135;
        if (pawns[WHITE]Info[COLG] == 0 && pawns[BLACK]Info[COLG] == 0) blackKingSafety += 135;
        if (pawns[WHITE]Info[COLH] == 0 && pawns[BLACK]Info[COLH] == 0) blackKingSafety += 135;
    }

    /* Pawns storm */
    if (piece[F5] == PAWN && color[F5] == WHITE)
        blackKingSafety += 15;
    if (piece[G5] == PAWN && color[G5] == WHITE)
        blackKingSafety += 15;
    if (piece[H5] == PAWN && color[H5] == WHITE)
        blackKingSafety += 15;
    if (piece[F6] == PAWN && color[F6] == WHITE)
        blackKingSafety += 25;
    if (piece[G6] == PAWN && color[G6] == WHITE)
        blackKingSafety += 25;
    if (piece[H6] == PAWN && color[H6] == WHITE)
        blackKingSafety += 25;

    return blackKingSafety;
}

void getWhitePawnRank(sq)
{
int tmpCol = Col(sq) + 1;
    if (Row(sq) > pawns[WHITE]Ranks[tmpCol])
    {
        pawns[WHITE]Ranks[tmpCol] = Row(sq);
    }
}

void getBlackPawnRank(sq)
{
int tmpCol = Col(sq) + 1;
    if (Row(sq) < pawns[BLACK]Ranks[tmpCol])
    {
        pawns[BLACK]Ranks[tmpCol] = Row(sq);
    }
}

/* Is it an opposite kings position? */
int isOppCastles()
{
    return abs(colBlackKing - colWhiteKing) > 4;
}

/* Are we in the endgame? */
inline int endGame()
{
    if (queens[WHITE]==0 || queens[BLACK]==0)
        return 1;
    if (pawns[WHITE] + pawns[BLACK] < 8)
        return 1;
    return 0;
}

/* Returns 0 if sq is on an open col */
inline int isOnAnOpenCol(int sq)
{
    return (pawns[WHITE]Ranks[Col(sq) + 1] == 0 && pawns[BLACK]Ranks[Col(sq) + 1] == 7);
}

/* Mobility of the bishop: number of empty squares a bishop can reach
 * from its actual position */
int BishopMobility(int sq)
{
    int l;
    int mob = 0;

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
    if (pawns[WHITE] == 0 && pawns[BLACK] == 0)
        if (rooks[WHITE] == 0 && rooks[BLACK] == 0)
            if (queens[WHITE] == 0 && queens[BLACK] == 0)
                if (bishops[WHITE] <= 1 && bishops[BLACK] <= 1)
                    if (whiteKnights <= 1 && knights[BLACK] <= 1)
                        if (whiteKnights == 0 || bishops[WHITE] == 0)
                            if (knights[BLACK] == 0 || bishops[BLACK] == 0)
                                return 1;
    return 0;
}

/* Function to check if x is power of 2. It's used to find out
   doubled pawns */
inline int isDoubledPawnWhite(int col)
{
  int tmp = pawns[WHITE]Info[col];

//  printf("pawns[WHITE]Info[col] is: %d\n", tmp);

  /* First x in the below expression is for the case when x is 0 */
  return !(tmp && (!(tmp & (tmp - 1))));
}

inline int isDoubledPawnBlack(int col)
{
  int tmp = pawns[BLACK]Info[col];

//  printf("pawns[BLACK]Info[col] is: %d\n", tmp);

  /* First x in the below expression is for the case when x is 0 */
  return !(tmp && (!(tmp & (tmp - 1))));
}



/*
 *  The fun factor: the main idea is to encourage interesting positions:
 *  -Queens on the board
 *  -Not too much pawns, to give place to open positions
 *  -Unbalanced material
 *  -...
 */
int funFactor()
{
    int funfa = 0;

    /* If we aren't in the endgame we like opposite castles */
    if ( !endGame() && (abs(colWhiteKing - colBlackKing) > 4) )
        funfa += 10;
    /* We like queens on the board */
    if (queens[WHITE] >= 1 || queens[BLACK] >= 1)
        funfa += 10;
    /* Taking into account the number of pawns on the board */
    funfa += num_pawns_funfac[pawns[WHITE] + pawns[BLACK]];
    /* No queens at all? That doesn't rule */
//    if (queens[WHITE] == 0 && queens[BLACK] == 0)
//        funfa -= 10;
    /* Encouraging the exchange (I hope) */
    if (rooks[WHITE] != rooks[BLACK])
        funfa += 10;
    /* Unbalanced material is welcome */
    if (queens[WHITE] != queens[BLACK])
        funfa += 10;
    if ( (bishops[WHITE] + whiteKnights) != (bishops[BLACK] + knights[BLACK]) )
        funfa += 10;

    return funfa;
}

/*
       TESTS FUNCTIONS
*/
void testWhitePassedPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == WHITE)
        {
            for (j=0; j<8; ++j)
            {
                pawns[WHITE]Info[j] = 0;
                pawns[BLACK]Info[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    pawns[BLACK]Info[(int)Col(j)] += 1<<Row(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    pawns[WHITE]Info[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isPassedPawnWhite(i))
                printf ("White passed pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
        }
    }
}

void testBlackPassedPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<8; ++j)
            {
                pawns[WHITE]Info[j] = 0;
                pawns[BLACK]Info[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    pawns[BLACK]Info[(int)Col(j)] += 1<<Row(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    pawns[WHITE]Info[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[BLACK]Info col %d: %d\n", j+1, pawns[BLACK]Info[j]);
//            }
            if (isPassedPawnBlack(i))
                printf ("Black passed pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
        }
    }
}

void testWhiteDoubledPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == WHITE)
        {
            for (j=0; j<8; ++j)
            {
                pawns[WHITE]Info[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == WHITE)
                    pawns[WHITE]Info[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }

            if (isDoubledPawnWhite(Col(i)))
                printf ("White doubled pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
        }
    }
}

void testBlackDoubledPawns()
{
    int i = 0, j=0;

    for (i=0; i<64; i++)
    {
        if (piece[i] == PAWN && color[i] == BLACK)
        {
            for (j=0; j<8; ++j)
            {
                pawns[WHITE]Info[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    pawns[BLACK]Info[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("pawns[WHITE]Info col %d: %d\n", j+1, pawns[WHITE]Info[j]);
//            }

            if (isDoubledPawnBlack(Col(i)))
                printf ("Balck doubled pawn in %d, 1<<Row(sq) is %d\n", i, 1<<Row(i));
        }
    }
}

void testOpenCols()
{
    int i = 0, j=0;

    for (j = 0; j < 10; ++j)
    {
        pawns[WHITE]Ranks[j] = 0;
        pawns[BLACK]Ranks[j] = 7;
    }

    for (i = 0; i < 64; i++)
    {
        if (piece[i] == PAWN)
        {
             for (j = 0; j < 64; ++j)
            {
                if (piece[j] == PAWN && color[j] == WHITE)
                    getWhitePawnRank(j);
                else if (piece[j] == PAWN && color[j] == BLACK)
                    getBlackPawnRank(j);
            }
        }
    }

    for (i = 0; i < 64; i++)
    {
    if (isOnAnOpenCol(i))
        printf ("Open col in square %d, column %d\n", i, Col(i));
    }
}
