// Rook.h
#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece {
public:
    Rook(bool b);
    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;
    bool isRook() const;
};

#endif
