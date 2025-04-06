// Knight.h
#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece {
public:
    Knight(bool b);

    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;
};

#endif
