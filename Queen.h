// Queen.h
#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece {
public:
    Queen(bool b);

    bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const override;
};

#endif
