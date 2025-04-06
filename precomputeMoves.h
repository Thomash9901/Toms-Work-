#ifndef PRECOMPUTEMOVES_H
#define PRECOMPUTEMOVES_H

#include <cstdint>

class PrecomputeMoves {
private:

    uint64_t bishopMoves[64];
    uint64_t rookMoves[64];


public:
    PrecomputeMoves();
    void generatePawnMoves();
    void generateKnightMoves();
    void generateBishopMoves();
    void generateRookMoves();
    void generateQueenMoves();
    void generateKingMoves();
    uint64_t getPawnSingleMove(bool isWhite, int square);
    uint64_t getKnightMove(int square);
    uint64_t getBishopMove(int square);
    uint64_t getRookMove(int square);
    uint64_t getQueenMove(int square);
    uint64_t getKingMove(int square);

    uint64_t pawnSingleMovesBlack[64];
    uint64_t pawnDoubleMovesBlack[64];
    uint64_t pawnLeftAttackBlack[64];
    uint64_t pawnRightAttackBlack[64];
    uint64_t pawnRightAttackWhite[64];
    uint64_t pawnLeftAttackWhite[64];
    uint64_t pawnDoubleMovesWhite[64];
    uint64_t pawnSingleMovesWhite[64];

uint64_t kingUpMovesWhite[64];
    uint64_t kingUpMovesBlack[64];
    uint64_t kingDownMovesWhite[64];
    uint64_t kingLeftMovesWhite[64];
    uint64_t kingRightMovesWhite[64];
    uint64_t kingDownMovesBlack[64];
    uint64_t kingLeftMovesBlack[64];
    uint64_t kingRightMovesBlack[64];
    uint64_t kingUpRightMovesWhite[64];
    uint64_t kingUpLeftMovesWhite[64];
    uint64_t kingDownRightMovesWhite[64];
    uint64_t kingDownLeftMovesWhite[64];
    uint64_t kingUpRightMovesBlack[64];
    uint64_t kingUpLeftMovesBlack[64];
    uint64_t kingDownRightMovesBlack[64];
    uint64_t kingDownLeftMovesBlack[64];


    uint64_t rookUpMoves[64];
    uint64_t rookDownMoves[64];
    uint64_t rookLeftMoves[64];
    uint64_t rookRightMoves[64];

    uint64_t BishopUpRightMoves[64];
    uint64_t BishopUpLeftMoves[64];
    uint64_t BishopDownRightMoves[64];
    uint64_t BishopDownLeftMoves[64];

    uint64_t knightMoves[64];

    uint64_t queenMoves[64];
};

#endif
