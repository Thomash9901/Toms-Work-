#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include "Piece.h"
#include "Move.h"
#include "MoveScore.h"

class ChessBoard {
private:
    uint64_t white_pawns, white_knights, white_bishops, white_queen;
    uint64_t black_pawns, black_knights, black_bishops;
    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookMovedKingside = false;
    bool whiteRookMovedQueenside = false;
    bool blackRookMovedKingside = false;
    bool blackRookMovedQueenside = false;


public:
    ChessBoard();
    void initialiseBoard();
    void printBoard() const;
    bool makeMove(const std::string& move, bool whitesTurn);
    char getPieceSymbolAt(int row, int col) const;
    static int chessInputToIndex(const std::string& input);
    bool isOccupied(int index) const;

    Piece *getPieceAt(int index);

    bool isOccupiedByOpponent(int index, bool isWhite) const;

    uint64_t getOccupiedSquares() const;

    std::string indexToString(int col, int row);

    std::vector<Move> getLegalMoves(bool isWhite);

    bool isKingInCheck(bool isWhite);

    void bitBoardSquares();

    uint64_t getOccupiedSquaresYourself(int index, bool isWhite) const;

    bool makeMove(const std::string &move, bool whitesTurn, bool simulate);

    bool isCheckmate(bool isWhite);

    bool isKingInCheckAtSquare(int squareIndex, bool isWhite) const;

    int enPassantTarget = -1;

    bool makeMove(const std::string &fromSquare, const std::string &toSquare, bool whitesTurn);

    bool makeMove(int fromIndex, int toIndex, bool whitesTurn);

    bool isStaleMate(int squareIndex, bool isWhite) const;

    bool isStaleMate(int squareIndex, bool isWhite);

    bool isStaleMate(bool isWhite);

    bool piecePromotion(bool isWhite);

    bool piecePromotion(int fromIndex, int toIndex, bool whitesTurn);

    uint64_t white_king;
    uint64_t black_king;

    int pieceValueBoard() const;

    bool white_castled;
    bool black_castled;


    uint64_t white_rooks;
    uint64_t black_queen;

    int countPassedPawns(bool isWhite);

    uint64_t black_rooks;


    std::string toFEN(bool whitesTurn) const;

    static int transformIndexForBlack(int index);

};

#endif // CHESSBOARD_H
