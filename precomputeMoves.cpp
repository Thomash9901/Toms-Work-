#include "PrecomputeMoves.h"

PrecomputeMoves::PrecomputeMoves() {
    generatePawnMoves();
    generateKnightMoves();
    generateBishopMoves();
    generateRookMoves();
    generateQueenMoves();
    generateKingMoves();
}

void PrecomputeMoves::generatePawnMoves()
{
    for (int i = 0; i < 64; i++)
    {
        pawnSingleMovesWhite[i] = 0;
        pawnDoubleMovesWhite[i] = 0;
        pawnLeftAttackWhite[i] = 0;
        pawnRightAttackWhite[i] = 0;
        pawnSingleMovesBlack[i] = 0;
        pawnDoubleMovesBlack[i] = 0;
        pawnLeftAttackBlack[i] = 0;
        pawnRightAttackBlack[i] = 0;
        uint64_t square = (1ULL << i);
        int row = i / 8;
        int col = i % 8;

        if (row > 0 && row < 7)
        {
            pawnSingleMovesWhite[i] = (1ULL << (i + 8));
            if (row == 1 )
            {
                pawnDoubleMovesWhite[i] = (1ULL << (i + 16));
            }
            if (col > 0)
            {
                pawnLeftAttackWhite[i] = (1ULL << (i + 7));
            }
            if (col < 7)
            {
                pawnRightAttackWhite[i] = (1ULL << (i + 9));
            }
        }

        if (row > 1 && row <8 )
        {
            pawnSingleMovesBlack[i] = (1ULL << (i - 8));

            if (row == 6)
            {
                pawnDoubleMovesBlack[i] = (1ULL << (i - 16));
            }
            if (col > 0)
            {
                pawnLeftAttackBlack[i] = (1ULL << (i - 9));
            }
            if (col < 7)
            {
                pawnRightAttackBlack[i] = (1ULL << (i - 7));
            }
        }
    }
}



void PrecomputeMoves::generateKingMoves() {
    for (int i = 0; i < 64; i++)
    {
        kingUpMovesWhite[i] = 0;
        kingDownMovesWhite[i] = 0;
        kingLeftMovesWhite[i] = 0;
        kingRightMovesWhite[i] = 0;
        kingUpRightMovesWhite[i] = 0;
        kingUpLeftMovesWhite[i] = 0;
        kingDownRightMovesWhite[i] = 0;
        kingDownLeftMovesWhite[i] = 0;
        kingUpMovesBlack[i] = 0;
        kingDownMovesBlack[i] = 0;
        kingLeftMovesBlack[i] = 0;
        kingRightMovesBlack[i] = 0;
        kingUpRightMovesBlack[i] = 0;
        kingUpLeftMovesBlack[i] = 0;
        kingDownRightMovesBlack[i] = 0;
        kingDownLeftMovesBlack[i] = 0;

        uint64_t square = (1ULL << i);
        int row = i / 8;
        int col = i % 8;

        if (row < 7)
        {
            kingUpMovesWhite[i] = square << 8;
            kingUpMovesBlack[i] = square << 8;
        }
        if (row > 0)
        {
            kingDownMovesWhite[i] = square >> 8;
            kingDownMovesBlack[i] = square >> 8;
        }
        if (col > 0)
        {
            kingLeftMovesWhite[i] = square >> 1;
            kingLeftMovesBlack[i] = square >> 1;
        }
        if (col < 7)
        {
            kingRightMovesWhite[i] = square << 1;
            kingRightMovesBlack[i] = square << 1;
        }
        if (row < 7 && col < 7)
        {
            kingUpRightMovesWhite[i] = square << 9;
            kingUpRightMovesBlack[i] = square << 9;
        }
        if (row < 7 && col > 0)
        {
            kingUpLeftMovesWhite[i] = square << 7;
            kingUpLeftMovesBlack[i] = square << 7;
        }
        if (row > 0 && col < 7)
        {
            kingDownRightMovesWhite[i] = square >> 7;
            kingDownRightMovesBlack[i] = square >> 7;
        }
        if (row > 0 && col > 0)
        {
            kingDownLeftMovesWhite[i] = square >> 9;
            kingDownLeftMovesBlack[i] = square >> 9;
        }
    }
}

void PrecomputeMoves::generateRookMoves() {
    for (int i = 0; i < 64; ++i)
    {
        rookUpMoves[i] = 0;
        rookDownMoves[i] = 0;
        rookLeftMoves[i] = 0;
        rookRightMoves[i] = 0;
        int row = i / 8;
        int col = i % 8;

        for (int r = row + 1; r < 8; ++r)
        {
            rookUpMoves[i] |= (1ULL << (r * 8 + col));
        }
        for (int r = row - 1; r >= 0; --r)
        {
            rookDownMoves[i] |= (1ULL << (r * 8 + col));
        }
        for (int c = col - 1; c >= 0; --c)
        {
            rookLeftMoves[i] |= (1ULL << (row * 8 + c));
        }
        for (int c = col + 1; c < 8; ++c)
        {
            rookRightMoves[i] |= (1ULL << (row * 8 + c));
        }
    }
}

void PrecomputeMoves::generateBishopMoves() {
    for (int i = 0; i < 64; ++i)
    {
        BishopUpRightMoves[i] = 0;
        BishopUpLeftMoves[i] = 0;
        BishopDownRightMoves[i] = 0;
        BishopDownLeftMoves[i] = 0;

        int row = i / 8;
        int col = i % 8;

        for (int r = row + 1, c = col + 1; r < 8 && c < 8; ++r, ++c)
        {
            BishopUpRightMoves[i] |= (1ULL << (r * 8 + c));
        }

        for (int r = row + 1, c = col - 1; r < 8 && c >= 0; ++r, --c)
        {
            BishopUpLeftMoves[i] |= (1ULL << (r * 8 + c));
        }

        for (int r = row - 1, c = col + 1; r >= 0 && c < 8; --r, ++c)
        {
            BishopDownRightMoves[i] |= (1ULL << (r * 8 + c));
        }

        for (int r = row - 1, c = col - 1; r >= 0 && c >= 0; --r, --c)
        {
            BishopDownLeftMoves[i] |= (1ULL << (r * 8 + c));
        }
    }
}

void PrecomputeMoves::generateKnightMoves() {
    for (int i = 0; i < 64; ++i)
    {
        knightMoves[i] = 0;
        int row = i / 8;
        int col = i % 8;

        int possibleKnightMoves[8][2] = {
                {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
                {1, -2},  {1, 2},  {2, -1},  {2, 1}
        };
        for (const auto& move : possibleKnightMoves)
        {
            int newRow = row + move[0];
            int newCol = col + move[1];

            if (newRow >= 0 && newRow < 8 && newCol >= 0 && newCol < 8)
            {
                knightMoves[i] |= (1ULL << (newRow * 8 + newCol));
            }
        }
    }
}

void PrecomputeMoves::generateQueenMoves() {
    for (int i = 0; i < 64; ++i)
    {
        queenMoves[i] = 0;
        queenMoves[i] |= rookUpMoves[i];
        queenMoves[i] |= rookDownMoves[i];
        queenMoves[i] |= rookLeftMoves[i];
        queenMoves[i] |= rookRightMoves[i];

        queenMoves[i] |= BishopUpRightMoves[i];
        queenMoves[i] |= BishopUpLeftMoves[i];
        queenMoves[i] |= BishopDownRightMoves[i];
        queenMoves[i] |= BishopDownLeftMoves[i];
    }
}


