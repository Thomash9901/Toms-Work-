// Queen.cpp
#include <iostream>
#include "Queen.h"
#include "chessBoard.h"
#include "precomputeMoves.h"
#include "PrecomputeMovesGlobal.h"

bool Queen::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    uint64_t possibleMoves = precomputedMoves.queenMoves[fromIndex];

    int fromRow = fromIndex / 8, fromCol = fromIndex % 8;
    int toRow = toIndex / 8, toCol = toIndex % 8;
    int rowDir = 0;
    if (toRow > fromRow)
    {
        rowDir = 1;
    } else if (toRow < fromRow)
    {
        rowDir = -1;
    }

    int colDir = 0;
    if (toCol > fromCol)
    {
        colDir = 1;
    } else if (toCol < fromCol)
    {
        colDir = -1;
    }

    int currentRow = fromRow + rowDir;
    int currentCol = fromCol + colDir;
    int currentIndex = currentRow * 8 + currentCol;
    if (!(possibleMoves & toSquare))
    {
        return false;
    }
    while (currentRow != toRow || currentCol != toCol)
    {
        currentIndex = currentRow * 8 + currentCol;
        if (board.isOccupied(currentIndex))
        {
            return false;
        }
        currentRow += rowDir;
        currentCol += colDir;
    }
    if (board.isOccupied(toIndex))
    {
        return board.isOccupiedByOpponent(toIndex, isWhite);
    }
    return true;
}


Queen::Queen(bool b) {

}