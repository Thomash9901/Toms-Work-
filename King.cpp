#include <iostream>
#include "King.h"
#include "chessBoard.h"
#include "precomputeMoves.h"
#include "PrecomputeMovesGlobal.h"

bool King::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    uint64_t possibleMoves = 0;


    if (isWhite)
    {
        possibleMoves |= precomputedMoves.kingUpMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingUpLeftMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingUpRightMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingDownMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingDownLeftMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingDownRightMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingLeftMovesWhite[fromIndex];
        possibleMoves |= precomputedMoves.kingRightMovesWhite[fromIndex];
    } else
    {
        possibleMoves |= precomputedMoves.kingUpMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingUpLeftMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingUpRightMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingDownMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingDownLeftMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingDownRightMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingLeftMovesBlack[fromIndex];
        possibleMoves |= precomputedMoves.kingRightMovesBlack[fromIndex];
    }

    if (possibleMoves & toSquare)
    {
        if (board.isOccupied(toIndex) && !board.isOccupiedByOpponent(toIndex, isWhite))
        {
            return false;
        }
        int enemyKingIndex;
        if (isWhite)
        {
            enemyKingIndex = __builtin_ctzll(board.black_king);
        }
        else
        {
            enemyKingIndex = __builtin_ctzll(board.white_king);
        }
        int enemyKingRow = enemyKingIndex / 8;
        int enemyKingCol = enemyKingIndex % 8;
        int toRow = toIndex / 8;
        int toCol = toIndex % 8;
        if (abs(enemyKingRow - toRow) <= 1 && abs(enemyKingCol - toCol) <= 1)
        {
            return false;
        }
        return true;
    }

    int fromRow = fromIndex / 8;
    int fromCol = fromIndex % 8;
    int toRow = toIndex / 8;
    int toCol = toIndex % 8;

    if (fromRow == toRow)
    {
        if (isWhite && fromIndex == 4)
        {
            if (toIndex == 7 && board.getPieceSymbolAt(toRow, toCol) == 'R') {
                if (!board.isOccupied(5) && !board.isOccupied(6))
                {
                    return true;
                }
            }
            else if (toIndex == 0 && board.getPieceSymbolAt(toRow, toCol) == 'R')
            {
                if (!board.isOccupied(1) && !board.isOccupied(2) && !board.isOccupied(3))
                {
                    return true;
                }
            }
        }
        else if (!isWhite && fromIndex == 60)
        {
            if (toIndex == 63 && board.getPieceSymbolAt(toRow, toCol) == 'r')
            {
                if (!board.isOccupied(61) && !board.isOccupied(62))
                {
                    return true;
                }
            }
            else if (toIndex == 56 && board.getPieceSymbolAt(toRow, toCol) == 'r')
            {
                if (!board.isOccupied(57) && !board.isOccupied(58) && !board.isOccupied(59))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool King::isKing() const {
    return true;
}

King::King(bool b) {

}
