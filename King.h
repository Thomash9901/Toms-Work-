// King.h
#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece {
public:
    King(bool b);

    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;

    bool isKing() const;
};

#endif
