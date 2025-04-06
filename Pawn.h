// Pawn.h
#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece {
public:
    Pawn(bool b);
    bool isPawn() const override { return true; }
    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;
};

#endif