// Rook.cpp
#include <iostream>
#include "Rook.h"
#include "chessBoard.h"
#include "precomputeMoves.h"
#include "PrecomputeMovesGlobal.h"

bool Rook::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    uint64_t possibleMoves = precomputedMoves.rookUpMoves[fromIndex]   | precomputedMoves.rookDownMoves[fromIndex] | precomputedMoves.rookLeftMoves[fromIndex]
            | precomputedMoves.rookRightMoves[fromIndex];

    int fromRow = fromIndex / 8;
    int fromCol = fromIndex % 8;
    int toRow = toIndex / 8;
    int toCol = toIndex % 8;
    int rowDir = 0;

    if (!(possibleMoves & toSquare))
    {
        return false;
    }

    if (toRow > fromRow)
    {
        rowDir = 1;
    }
    else if (toRow < fromRow)
    {
        rowDir = -1;
    }
    int colDir = 0;
    if(toCol > fromCol)
    {
        colDir = 1;
    }
    else if (toCol < fromCol)
    {
        colDir = -1;
    }

    if (!(possibleMoves & toSquare))
    {
        return false;
    }

    int currentRow = fromRow + rowDir;
    int currentCol = fromCol + colDir;
    while (currentRow != toRow || currentCol != toCol)
    {
        int currentIndex = currentRow * 8 + currentCol;
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


bool Rook::isRook() const {
    return true;
}

Rook::Rook(bool b) {

}