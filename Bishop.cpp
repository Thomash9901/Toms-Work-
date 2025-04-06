// Bishop.cpp
#include "Piece.h"
#include "chessBoard.h"
#include "Bishop.h"
#include "precomputeMoves.h"
#include <cmath>
#include "PrecomputeMovesGlobal.h"

bool Bishop::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    int fromRow = fromIndex / 8;
    int fromCol = fromIndex % 8;
    int toRow = toIndex / 8;
    int toCol = toIndex % 8;
    int rowDifferencePos = std::abs(fromRow - toRow);
    int colDifferencePos = std::abs(fromCol - toCol);
    uint64_t possibleMoves = 0;
    if (rowDifferencePos != colDifferencePos)
    {
        return false;
    }
    int rowDir;
    if (toRow > fromRow)
    {
        rowDir = 1;
    }
    else
    {
        rowDir = -1;
    }
    int colDir;
    if(toCol > fromCol)
    {
        colDir = 1;
    }
    else
    {
        colDir = -1;
    }
    if (rowDir == 1 && colDir == 1)
    {
        possibleMoves = precomputedMoves.BishopUpRightMoves[fromIndex];
    }
    else if (rowDir == 1 && colDir == -1)
    {
        possibleMoves = precomputedMoves.BishopUpLeftMoves[fromIndex];
    }
    else if (rowDir == -1 && colDir == 1)
    {
        possibleMoves = precomputedMoves.BishopDownRightMoves[fromIndex];
    }
    else if (rowDir == -1 && colDir == -1)
    {
        possibleMoves = precomputedMoves.BishopDownLeftMoves[fromIndex];
    }

    int currentRow = fromRow + rowDir;
    int currentCol = fromCol + colDir;
    while (currentRow != toRow) {
        if (board.isOccupied(currentRow * 8 + currentCol))
        {
            return false;
        }
        currentRow = currentRow + rowDir;
        currentCol = currentCol + colDir;
    }
    return (possibleMoves & toSquare) && (!board.isOccupied(toIndex) || board.isOccupiedByOpponent(toIndex, isWhite));
}

Bishop::Bishop(bool b) {

}
