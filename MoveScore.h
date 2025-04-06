#ifndef MOVESCORE_H
#define MOVESCORE_H

#include <unordered_map>
#include "chessBoard.h"

struct MoveScore
{
    int score;
    int fromIndex;
    int toIndex;
};

MoveScore minimaxCalculation(ChessBoard board, int depth, bool isMaximizingPlayer, int beta, int alpha);

#endif