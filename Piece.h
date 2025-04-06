// Piece.h
#ifndef PIECE_H
#define PIECE_H

class ChessBoard;

class Piece {
public:
    virtual ~Piece() = default;
    virtual bool isValidMove(int fromIndex, int toIndex, const ChessBoard& board, bool isWhite) const = 0;
    virtual bool isPawn() const
    {
        return false;
    }
    virtual bool isKing() const
    {
        return false;
    }
    virtual bool isRook() const
    {
        return false;
    }
    virtual bool isBishop() const
    {
        return false;
    }
    virtual bool isQueen() const
    {
        return false;
    }
    virtual bool isKnight() const
    {
        return false;
    }
};

#endif
