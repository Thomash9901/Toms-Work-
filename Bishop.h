// Bishop.h
#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

class Bishop : public Piece {
public:
    Bishop(bool b);

    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;
};

#endif
