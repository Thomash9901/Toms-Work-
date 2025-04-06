// Pawn.cpp
#include "Pawn.h"
#include "chessBoard.h"
#include "precomputeMoves.h"
#include "PrecomputeMovesGlobal.h"

bool Pawn::isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const {
    static PrecomputeMoves precompute;
    uint64_t toSquare = 1ULL << toIndex;
    uint64_t allPieces = board.getOccupiedSquares();
    uint64_t possibleMoves = 0;
    uint64_t attackLeft = precomputedMoves.pawnLeftAttackWhite[fromIndex];
    uint64_t attackRight = precomputedMoves.pawnRightAttackWhite[fromIndex];

    if (isWhite) {
        if (!(allPieces & precompute.pawnSingleMovesWhite[fromIndex]))
        {
            possibleMoves |= precompute.pawnSingleMovesWhite[fromIndex];
        }
        if ((fromIndex / 8 == 1) && !(allPieces & precompute.pawnSingleMovesWhite[fromIndex]) && !(allPieces & precompute.pawnDoubleMovesWhite[fromIndex]))
        {
            possibleMoves |= precompute.pawnDoubleMovesWhite[fromIndex];
        }
        if ((attackLeft & toSquare) && board.isOccupiedByOpponent(toIndex, isWhite))
        {
            possibleMoves |= toSquare;
        }
        if ((attackRight & toSquare) && board.isOccupiedByOpponent(toIndex, isWhite))
        {
            possibleMoves |= toSquare;
        }
        if (board.enPassantTarget != -1) {
            if (precompute.pawnLeftAttackWhite[fromIndex] & (1ULL << board.enPassantTarget))
            {
                possibleMoves |= (1ULL << board.enPassantTarget);
            }
            if (precompute.pawnRightAttackWhite[fromIndex] & (1ULL << board.enPassantTarget))
            {
                possibleMoves |= (1ULL << board.enPassantTarget);
            }
        }
    } else {
        if (!(allPieces & precompute.pawnSingleMovesBlack[fromIndex]))
        {
            possibleMoves |= precompute.pawnSingleMovesBlack[fromIndex];
        }
        if ((fromIndex / 8 == 6) && !(allPieces & precompute.pawnSingleMovesBlack[fromIndex]) && !(allPieces & precompute.pawnDoubleMovesBlack[fromIndex]))
        {
            possibleMoves |= precompute.pawnDoubleMovesBlack[fromIndex];
        }
        uint64_t attackLeft = precompute.pawnLeftAttackBlack[fromIndex];
        if ((attackLeft & toSquare) && board.isOccupiedByOpponent(toIndex, isWhite))
        {
            possibleMoves |= toSquare;
        }
        uint64_t attackRight = precompute.pawnRightAttackBlack[fromIndex];
        if ((attackRight & toSquare) && board.isOccupiedByOpponent(toIndex, isWhite))
        {
            possibleMoves |= toSquare;
        }
        if (board.enPassantTarget != -1) {
            if (precompute.pawnLeftAttackBlack[fromIndex] & (1ULL << board.enPassantTarget))
            {
                possibleMoves |= (1ULL << board.enPassantTarget);
            }
            if (precompute.pawnRightAttackBlack[fromIndex] & (1ULL << board.enPassantTarget)) {

                possibleMoves |= (1ULL << board.enPassantTarget);
            }
        }
    }

    return (possibleMoves & toSquare);
}


Pawn::Pawn(bool b) {

}