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
int mob_rook[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};
int mob_knight[9] = {
    -10, -4, 2, 8, 14, 18, 22, 24, 25
};
int mob_bishop[16] = {
    -5, -2, 0, 3, 6, 10, 14, 20, 24, 28, 31, 35, 40, 42, 45, 47
};
int range_bishop[16] = {
    -6, -3, 0, 2, 4, 6, 8, 10, 12, 14, 15, 16, 17, 18, 19, 20
};

/* For scaling passed pawns depending on the row */
int passed_pawn_white[7] = {90, 75, 55, 25, 17, 10, 0};
int passed_pawn_black[7] = {0, 10, 17, 25, 55, 75, 90};

/* For storing pawns' ranks */
int whitePawnsRanks[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int blackPawnsRanks[10] = {7, 7, 7, 7, 7, 7, 7, 7, 7, 7};

/* For scaling pawn number in fun factor */
int num_pawns_funfac[16] = {0, 0, 0, 0, 0, 0, 0, 0, -5, -5, -10, -10, -12, -12, -15, -15};

/* Kings' safety */
int posWhiteKing = 0;
int colWhiteKing = 0;
int posBlackKing = 0;
int colBlackKing = 0;

/* To count the material */
int whitePawns = 0;
int whiteKnights = 0;
int whiteBishops = 0;
int whiteRooks = 0;
int whiteQueens = 0;
int blackPawns = 0;
int blackKnights = 0;
int blackBishops = 0;
int blackRooks = 0;
int blackQueens = 0;

/* Pawn's info */
int whitePawnsInfo[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int blackPawnsInfo[8] = {0, 0, 0, 0, 0, 0, 0, 0};

/* The evaluation function */
int Eval(alpha, beta)
{
    /* A traditional counter */
    int i;

    /* Set some values to 0 */

    /* Pawn's info */
    for (i=0; i<8; ++i)
    {
        whitePawnsInfo[i] = 0;
        blackPawnsInfo[i] = 0;
    }
    for (i = 0; i < 10; ++i)
    {
        whitePawnsRanks[i] = 0;
        blackPawnsRanks[i] = 7;
    }

    /* The fun factor */
    int ff = 0;

    /* The vars for counting the material */
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
//                whitePawnsInfo[(int)Col(i)] += 1<<Row(63 - i);
                getWhitePawnRank(i);
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
                blackPawns++;
//                blackPawnsInfo[(int)Col(i)] += 1<<Row(i);
                getBlackPawnRank(i);
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
            case KING:
                posBlackKing = i;
                colBlackKing = Col(i);
                break;
            }
        }
    }

    printf("\nBlack pawns: ");
    for (i=0; i<10; ++i)
    {
        printf("%4d", blackPawnsRanks[i]);
    }
    puts("");

    printf ("White pawns: ");
    for (i=0; i<10; ++i)
    {
        printf("%4d", whitePawnsRanks[i]);
    }
    puts("");


    /* After counting the material we update the score */
//    score = piece_mat[WHITE] - piece_mat[BLACK];
    score = (whitePawns - blackPawns) * value_piece[PAWN] +
            (whiteKnights - blackKnights) * value_piece[KNIGHT] +
            (whiteBishops - blackBishops) * value_piece[BISHOP] +
            (whiteRooks - blackRooks) * value_piece[ROOK] +
            (whiteQueens - blackQueens) * value_piece[QUEEN];

    /* Is there enough material to keep on playing? */
    if (NoMaterial()) return 0;

    /* Anyone has the pair of bishops? */
    if (whiteBishops==2 && blackBishops!=2)
        score += PAIR_BISHOPS;
    else if (blackBishops==2 && whiteBishops!=2)
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
        if ( blackPawnsInfo[Col(sq)] == 0 &&
             (blackPawnsInfo[Col(sq+1)] >= 1<<Row(sq) || blackPawnsInfo[Col(sq) + 1] == 0) )
            return 1;
    }
    /* Special case, pawn in H row */
    else if (Col(sq) == 7)
    {
        if ( blackPawnsInfo[Col(sq)] == 0 &&
           (blackPawnsInfo[Col(sq-1)] >= 1<<Row(sq) || blackPawnsInfo[Col(sq) - 1] == 0) )
            return 1;
    }
    else
    {
        if ( blackPawnsInfo[Col(sq)] == 0 &&
             (blackPawnsInfo[Col(sq) - 1] >= 1<<Row(sq) || blackPawnsInfo[Col(sq) - 1] == 0) &&
             (blackPawnsInfo[Col(sq) + 1] >= 1<<Row(sq) || blackPawnsInfo[Col(sq) + 1] == 0) )
            return 1;
    }
    return 0;
}

int isPassedPawnBlack(int sq)
{
    /* Special case, pawn in A row */
    if (Col(sq) == 0)
    {
        if ( whitePawnsInfo[Col(sq)] == 0 &&
             (whitePawnsInfo[Col(sq+1)] <= 1<<Row(sq) || whitePawnsInfo[Col(sq) + 1] == 0) )
            return 1;
    }
    /* Special case, pawn in H row */
    else if (Col(sq) == 7)
    {
        if ( whitePawnsInfo[Col(sq)] == 0 &&
             (whitePawnsInfo[Col(sq-1)] <= 1<<Row(sq) || whitePawnsInfo[Col(sq) - 1] == 0) )
            return 1;
    }
    else
    {
//        int i;
//        for (i=0; i<8; i++)
//            printf("whitePawnsInfo col %d: %d\n", i+1, whitePawnsInfo[i]);
//        puts("-----------------------");
//        for (i=0; i<8; i++)
//            printf("blackPawnsInfo col %d: %d\n", i+1, blackPawnsInfo[i]);

        if ( whitePawnsInfo[Col(sq)] == 0 &&
             (whitePawnsInfo[Col(sq) - 1] <= 1<<Row(sq) || whitePawnsInfo[Col(sq) - 1] == 0) &&
             (whitePawnsInfo[Col(sq) + 1] <= 1<<Row(sq) || whitePawnsInfo[Col(sq) + 1] == 0) )
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
        if (whitePawnsInfo[COLD] == 0)
        {
            safety -= 15;
            if (blackPawnsInfo[COLD] == 0)
            {
                safety -= 35;
            }
        }
        if (whitePawnsInfo[COLE] == 0)
        {
            safety -= 15;
            if (blackPawnsInfo[COLE] == 0)
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
        if (blackPawnsInfo[COLD] == 0)
        {
            safety += 15;
            if (whitePawnsInfo[COLD] == 0)
            {
                safety += 35;
            }
        }
        if (blackPawnsInfo[COLE] == 0)
        {
            safety += 15;
            if (whitePawnsInfo[COLE] == 0)
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
        if (whitePawnsInfo[COLA] == 64) whiteKingSafety += 12;
        else if (whitePawnsInfo[COLA] == 32) whiteKingSafety += 6;
        else if (whitePawnsInfo[COLA] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;
        // Col B
        if (whitePawnsInfo[COLB] == 64) whiteKingSafety +=12;
        else if (whitePawnsInfo[COLB] == 32) whiteKingSafety += 6;
        else if (whitePawnsInfo[COLB] == 0) whiteKingSafety += 2*MISSING_PAWN_CASTLE_MALUS;
        // Col C
        if (whitePawnsInfo[COLC] == 64) whiteKingSafety +=12;
        else if (whitePawnsInfo[COLC] == 32) whiteKingSafety += 6;
        else if (whitePawnsInfo[COLC] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;

        /* Doubled pawns on castle */
        if (isDoubledPawnWhite(COLA)) whiteKingSafety += 3*DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLB)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
        if (isDoubledPawnWhite(COLC)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;

        /* Semiopen cols by the oponent */
        if (blackPawnsInfo[COLA] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }
        if (blackPawnsInfo[COLB] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }
        if (blackPawnsInfo[COLC] == 0) whiteKingSafety -= 25;
        {
            if (isOpp) whiteKingSafety -= 105;
        }

        /* Open cols close to the king are more important in case
            of opposite castles*/
        if (isOpp)
        {
            if (whitePawnsInfo[COLA] == 0 && blackPawnsInfo[COLA] == 0) whiteKingSafety -= 35;
            if (whitePawnsInfo[COLB] == 0 && blackPawnsInfo[COLB] == 0) whiteKingSafety -= 35;
            if (whitePawnsInfo[COLC] == 0 && blackPawnsInfo[COLC] == 0) whiteKingSafety -= 35;
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
    if (blackPawnsInfo[COLA] == 2) blackKingSafety -= 12;
    else if (blackPawnsInfo[COLA] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLA] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;
    // Col B
    if (blackPawnsInfo[COLB] == 2) blackKingSafety -= 12;
    else if (blackPawnsInfo[COLB] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLB] == 0) blackKingSafety -= 2*MISSING_PAWN_CASTLE_MALUS;
    // Col C
    if (blackPawnsInfo[COLC] == 2) blackKingSafety -= 12;
    else if (blackPawnsInfo[COLC] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLC] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnBlack(COLA)) blackKingSafety -= 3*DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLB)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLC)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (whitePawnsInfo[COLA] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (whitePawnsInfo[COLB] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (whitePawnsInfo[COLC] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (whitePawnsInfo[COLA] == 0 && blackPawnsInfo[COLA] == 0) blackKingSafety += 35;
        if (whitePawnsInfo[COLB] == 0 && blackPawnsInfo[COLB] == 0) blackKingSafety += 35;
        if (whitePawnsInfo[COLC] == 0 && blackPawnsInfo[COLC] == 0) blackKingSafety += 35;
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
    if (whitePawnsInfo[COLF] == 64) whiteKingSafety += 12;
    else if (whitePawnsInfo[COLF] == 32) whiteKingSafety += 6;
    else if (whitePawnsInfo[COLF] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;
    // Col G
    if (whitePawnsInfo[COLG] == 64) whiteKingSafety +=12;
    else if (whitePawnsInfo[COLG] == 32) whiteKingSafety += 6;
    else if (whitePawnsInfo[COLG] == 0) whiteKingSafety += 2*MISSING_PAWN_CASTLE_MALUS;
    // Col H
    if (whitePawnsInfo[COLH] == 64) whiteKingSafety +=12;
    else if (whitePawnsInfo[COLH] == 32) whiteKingSafety += 6;
    else if (whitePawnsInfo[COLH] == 0) whiteKingSafety += MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnWhite(COLF)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnWhite(COLG)) whiteKingSafety += DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnWhite(COLH)) whiteKingSafety += 3*DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (blackPawnsInfo[COLF] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }
    if (blackPawnsInfo[COLG] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }
    if (blackPawnsInfo[COLH] == 0) whiteKingSafety -= 25;
    {
        /* It's more dangerous in case of opposite castles */
        if (isOpp) whiteKingSafety -= 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (whitePawnsInfo[COLF] == 0 && blackPawnsInfo[COLF] == 0) whiteKingSafety -= 135;
        if (whitePawnsInfo[COLG] == 0 && blackPawnsInfo[COLG] == 0) whiteKingSafety -= 135;
        if (whitePawnsInfo[COLH] == 0 && blackPawnsInfo[COLH] == 0) whiteKingSafety -= 235;
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
    if (blackPawnsInfo[COLF] == 2) blackKingSafety -= 12;
    else if (blackPawnsInfo[COLF] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLF] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;
    // Col G
    if (blackPawnsInfo[COLG] == 2) blackKingSafety -=12;
    else if (blackPawnsInfo[COLG] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLG] == 0) blackKingSafety -= 2*MISSING_PAWN_CASTLE_MALUS;
    // Col H
    if (blackPawnsInfo[COLH] == 2) blackKingSafety -=12;
    else if (blackPawnsInfo[COLH] == 4) blackKingSafety -= 6;
    else if (blackPawnsInfo[COLH] == 0) blackKingSafety -= MISSING_PAWN_CASTLE_MALUS;

    /* Doubled pawns on castle */
    if (isDoubledPawnBlack(COLF)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLG)) blackKingSafety -= DOUBLED_PAWN_CASTLE_MALUS;
    if (isDoubledPawnBlack(COLH)) blackKingSafety -= 3*DOUBLED_PAWN_CASTLE_MALUS;

    /* Semiopen cols by the oponent */
    if (whitePawnsInfo[COLF] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (whitePawnsInfo[COLG] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }
    if (whitePawnsInfo[COLH] == 0) blackKingSafety += 25;
    {
        if (isOpp) blackKingSafety += 105;
    }

    /* Open cols close to the king are more important in case
        of opposite castles*/
    if (isOpp)
    {
        if (whitePawnsInfo[COLF] == 0 && blackPawnsInfo[COLF] == 0) blackKingSafety += 135;
        if (whitePawnsInfo[COLG] == 0 && blackPawnsInfo[COLG] == 0) blackKingSafety += 135;
        if (whitePawnsInfo[COLH] == 0 && blackPawnsInfo[COLH] == 0) blackKingSafety += 135;
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
    if (Row(sq) > whitePawnsRanks[tmpCol])
    {
        whitePawnsRanks[tmpCol] = Row(sq);
    }
}

void getBlackPawnRank(sq)
{
int tmpCol = Col(sq) + 1;
    if (Row(sq) < blackPawnsRanks[tmpCol])
    {
        blackPawnsRanks[tmpCol] = Row(sq);
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
    if (whiteQueens==0 || blackQueens==0)
        return 1;
    if (whitePawns + blackPawns < 8)
        return 1;
    return 0;
}

/* Returns 0 if sq is on an open col */
inline int isOnAnOpenCol(int sq)
{
    return (whitePawnsRanks[Col(sq) + 1] == 0 && blackPawnsRanks[Col(sq) + 1] == 7);
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
    if (whitePawns == 0 && blackPawns == 0)
        if (whiteRooks == 0 && blackRooks == 0)
            if (whiteQueens == 0 && blackQueens == 0)
                if (whiteBishops <= 1 && blackBishops <= 1)
                    if (whiteKnights <= 1 && blackKnights <= 1)
                        if (whiteKnights == 0 || whiteBishops == 0)
                            if (blackKnights == 0 || blackBishops == 0)
                                return 1;
    return 0;
}

/* Function to check if x is power of 2. It's used to find out
   doubled pawns */
inline int isDoubledPawnWhite(int col)
{
  int tmp = whitePawnsInfo[col];

//  printf("whitePawnsInfo[col] is: %d\n", tmp);

  /* First x in the below expression is for the case when x is 0 */
  return !(tmp && (!(tmp & (tmp - 1))));
}

inline int isDoubledPawnBlack(int col)
{
  int tmp = blackPawnsInfo[col];

//  printf("blackPawnsInfo[col] is: %d\n", tmp);

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
    if (whiteQueens >= 1 || blackQueens >= 1)
        funfa += 10;
    /* Taking into account the number of pawns on the board */
    funfa += num_pawns_funfac[whitePawns + blackPawns];
    /* No queens at all? That doesn't rule */
//    if (whiteQueens == 0 && blackQueens == 0)
//        funfa -= 10;
    /* Encouraging the exchange (I hope) */
    if (whiteRooks != blackRooks)
        funfa += 10;
    /* Unbalanced material is welcome */
    if (whiteQueens != blackQueens)
        funfa += 10;
    if ( (whiteBishops + whiteKnights) != (blackBishops + blackKnights) )
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
                whitePawnsInfo[j] = 0;
                blackPawnsInfo[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    blackPawnsInfo[(int)Col(j)] += 1<<Row(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    whitePawnsInfo[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("whitePawnsInfo col %d: %d\n", j+1, whitePawnsInfo[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("blackPawnsInfo col %d: %d\n", j+1, blackPawnsInfo[j]);
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
                whitePawnsInfo[j] = 0;
                blackPawnsInfo[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    blackPawnsInfo[(int)Col(j)] += 1<<Row(j);
                if (piece[j] == PAWN && color[j] == WHITE)
                    whitePawnsInfo[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("whitePawnsInfo col %d: %d\n", j+1, whitePawnsInfo[j]);
//            }
//            puts("-----------------------");
//            for (j=0; j<8; j++)
//            {
//                printf("blackPawnsInfo col %d: %d\n", j+1, blackPawnsInfo[j]);
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
                whitePawnsInfo[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == WHITE)
                    whitePawnsInfo[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("whitePawnsInfo col %d: %d\n", j+1, whitePawnsInfo[j]);
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
                whitePawnsInfo[j] = 0;
            }
            for (j=0; j<64; j++)
            {
                if (piece[j] == PAWN && color[j] == BLACK)
                    blackPawnsInfo[(int)Col(j)] += 1<<Row(j);
            }
//            for (j=0; j<8; j++)
//            {
//                printf("whitePawnsInfo col %d: %d\n", j+1, whitePawnsInfo[j]);
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
        whitePawnsRanks[j] = 0;
        blackPawnsRanks[j] = 7;
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
