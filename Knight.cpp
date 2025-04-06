// Knight.cpp
#include <iostream>
#include "Knight.h"
#include "chessBoard.h"
#include "precomputeMoves.h"
#include "PrecomputeMovesGlobal.h"

bool Knight::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    uint64_t possibleMoves = precomputedMoves.knightMoves[fromIndex];


    if (!(possibleMoves & toSquare))
    {
        return false;
    }
    else if (board.isOccupied(toIndex))
    {
        return board.isOccupiedByOpponent(toIndex, isWhite);
    }
    else
    {
        return true;
    }
}

Knight::Knight(bool b) {

}