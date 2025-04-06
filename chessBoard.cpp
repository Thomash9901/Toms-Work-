#include "chessBoard.h"
#include <iostream>
#include <bitset>
#include <sstream>
#include "Pawn.h"
#include "Rook.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include <vector>
#include <algorithm>
#include "Move.h"
#include <chrono>
#include "PrecomputeMoves.h"

extern PrecomputeMoves precomputedMoves;


ChessBoard::ChessBoard() {
    initialiseBoard();
}


void ChessBoard::initialiseBoard() {
    white_pawns = 0x000000000000FF00ULL;
    white_knights = 0x0000000000000042ULL;
    white_bishops = 0x0000000000000024ULL;
    white_rooks = 0x0000000000000081ULL;
    white_queen = 0x0000000000000008ULL;
    white_king = 0x0000000000000010ULL;

    black_pawns = 0x00FF000000000000ULL;
    black_knights = 0x4200000000000000ULL;
    black_bishops = 0x2400000000000000ULL;
    black_rooks = 0x8100000000000000ULL;
    black_queen = 0x0800000000000000ULL;
    black_king = 0x1000000000000000ULL;
    enPassantTarget = -1;
}


void ChessBoard::bitBoardSquares()
{
    uint64_t bitBoards[64];
    for (int i = 0; i < 64; i++)
    {
        bitBoards[i] = 1ULL << i;
    }
}


Piece* ChessBoard::getPieceAt(int index) {
    uint64_t mask = 1ULL << index;

    if (white_pawns & mask)
    {
        return new Pawn(true);
    }
    if (black_pawns & mask)
    {
        return new Pawn(false);
    }
    if (white_rooks & mask) {
        return new Rook(true);
    }
    if (black_rooks & mask)
    {
        return new Rook(false);
    }
    if (white_bishops & mask)
    {
        return new Bishop(false);
    }
    if (black_bishops & mask)
    {
        return new Bishop(true);
    }
    if (black_king & mask)
    {
        return new King(true);
    }
    if (white_king & mask)
    {
        return new King(false);
    }
    if (black_queen & mask)
    {
        return new Queen(true);
    }
    if (white_queen & mask)
    {
        return new Queen(false);
    }
    if (black_knights & mask)
    {
        return new Knight(true);
    }
    if (white_knights & mask)
    {
        return new Knight(false);
    }

    return nullptr;
}



void ChessBoard::printBoard() const {
    std::cout << "  a b c d e f g h\n";
    for (int rows = 7; rows >= 0; --rows)
    {
        std::cout << rows + 1 << " ";
        for (int columns = 0; columns < 8; ++columns)
        {
            int square = rows * 8 + columns;
            if ((white_pawns >> square) & 1)
            {
                std::cout << "P ";
            }
            else if ((black_pawns >> square) & 1)
            {
                std::cout << "p ";
            }
            else if ((white_rooks >> square) & 1)
            {
                std::cout << "R ";
            }
            else if ((black_rooks >> square) & 1)
            {
                std::cout << "r ";
            }
            else if ((white_knights >> square) & 1)
            {
                std::cout << "N ";
            }
            else if ((black_knights >> square) & 1)
            {
                std::cout << "n ";
            }
            else if ((white_bishops >> square) & 1)
            {
                std::cout << "B ";
            }
            else if ((black_bishops >> square) & 1)
            {
                std::cout << "b ";
            }
            else if ((white_queen >> square) & 1)
            {
                std::cout << "Q ";
            }
            else if ((black_queen >> square) & 1)
            {
                std::cout << "q ";
            }
            else if ((white_king >> square) & 1)
            {
                std::cout << "K ";
            }
            else if ((black_king >> square) & 1)
            {
                std::cout << "k ";
            }
            else
            {
                std::cout << ". ";
            }
        }
        std::cout << rows + 1 << "\n";
    }
    std::cout << "  a b c d e f g h\n";
}


uint64_t ChessBoard::getOccupiedSquares() const {
    return white_pawns | white_knights | white_bishops | white_rooks | white_queen | white_king |
           black_pawns | black_knights | black_bishops | black_rooks | black_queen | black_king;
}


bool ChessBoard::isOccupiedByOpponent(int index, bool isWhite) const {
    uint64_t mask = 1ULL << index;
    if (isWhite) {
        return (black_pawns | black_knights | black_bishops | black_rooks | black_queen | black_king) & mask;
    }
    else
    {
        return (white_pawns | white_knights | white_bishops | white_rooks | white_queen | white_king) & mask;
    }
}


bool ChessBoard::isOccupied(int index) const {
    uint64_t mask = 1ULL << index;
    if ((white_pawns | black_pawns | white_knights | black_knights | white_bishops | black_bishops | white_rooks | black_rooks | white_queen | black_queen | white_king | black_king) & mask)
    {
        return true;
    }
    return false;
}


bool ChessBoard::makeMove(int fromIndex, int toIndex, bool whitesTurn){
    uint64_t fromMask = 1ULL << fromIndex;
    uint64_t toMask = 1ULL << toIndex;
    Piece* piece = getPieceAt(fromIndex);


    if (!piece || !piece->isValidMove(fromIndex, toIndex, *this, whitesTurn)) {
        std::cout << fromIndex;
        std::cout << toIndex;
        std::cout << "Not a valid move\n";
        delete piece;
        return false;
    }

    if (toIndex == enPassantTarget && piece->isPawn())
    {
        if (whitesTurn)
        {
            int capturedPawnIndex = toIndex - 8;
            black_pawns &= ~(1ULL << capturedPawnIndex);
        }
        else
        {
            int capturedPawnIndex = toIndex + 8;
            white_pawns &= ~(1ULL << capturedPawnIndex);
        }
    }

    enPassantTarget = -1;

    if (piece->isPawn())
    {
        int fromRow = fromIndex / 8;
        int toRow = toIndex / 8;
        if (std::abs(toRow - fromRow) == 2)
        {
            enPassantTarget = fromIndex + (toIndex - fromIndex) / 2;
        }
    }

    bool castling = false;
    if (piece->isKing())
    {
        if (whitesTurn && fromIndex == 4)
        {
            if (toIndex == 7)
            {
                if ((white_rooks & (1ULL << 7)) && !isOccupied(5) && !isOccupied(6))
                {
                    // Had to add this line as the AI wouldnt understand that it could evade check with castling
                    if (!isKingInCheck(true) && !isKingInCheckAtSquare(5, true) && !isKingInCheckAtSquare(6, true))
                    {
                        castling = true;
                    }
                }
            }
            else if (toIndex == 0)
            {
                if ((white_rooks & (1ULL << 0)) && !isOccupied(1) && !isOccupied(2) && !isOccupied(3))
                {
                    // Had to add this line as the AI wouldnt understand that it could evade check with castling
                    if (!isKingInCheck(true) && !isKingInCheckAtSquare(3, true) && !isKingInCheckAtSquare(2, true))
                    {
                        castling = true;
                    }
                }
            }
        }
        else if (!whitesTurn && fromIndex == 60)
        {
            if (toIndex == 63)
            {
                if ((black_rooks & (1ULL << 63)) && !isOccupied(61) && !isOccupied(62))
                {
                    // Had to add this line as the AI wouldnt understand that it could evade check with castling
                    if (!isKingInCheck(false) && !isKingInCheckAtSquare(61, false) && !isKingInCheckAtSquare(62, false))
                    {
                        castling = true;
                    }
                }
            } else if (toIndex == 56)
            {
                if ((black_rooks & (1ULL << 56)) && !isOccupied(57) && !isOccupied(58) && !isOccupied(59))
                {
                    // Had to add this line as the AI wouldnt understand that it could evade check with castling
                    if (!isKingInCheck(false) && !isKingInCheckAtSquare(59, false) && !isKingInCheckAtSquare(58, false))
                    {
                        castling = true;
                    }
                }
            }
        }
    }


    uint64_t* pieces[] = {&white_pawns, &white_knights, &white_bishops, &white_rooks, &white_queen, &white_king,
                          &black_pawns, &black_knights, &black_bishops, &black_rooks, &black_queen, &black_king
    };

    int start;
    int end;
    if (whitesTurn)
    {
        start = 0;
        end = 6;
    }
    else
    {
        start = 6;
        end = 12;
    }

    int Start2;
    int End2;
    if (whitesTurn)
    {
        Start2 = 6;
        End2 = 12;
    }
    else
    {
        Start2 = 0;
        End2 = 6;
    }

    for (int i = start; i < end; ++i)
    {
        if (*pieces[i] & fromMask)
        {
            *pieces[i] &= ~fromMask;
            *pieces[i] |= toMask;
            for (int j = Start2; j < End2; ++j)
            {
                if (*pieces[j] & toMask)
                {
                    *pieces[j] &= ~toMask;
                    break;
                }
            }
            if (castling) {
                if (whitesTurn) {
                    if (toIndex == 7)
                    {
                        white_king &= ~(1ULL << 7);
                        white_king |= (1ULL << 6);
                        white_rooks &= ~(1ULL << 7);
                        white_rooks |= (1ULL << 5);
                    }
                    else if (toIndex == 0)
                    {
                        white_king &= ~(1ULL << 0);
                        white_king |= (1ULL << 2);
                        white_rooks &= ~(1ULL << 0);
                        white_rooks |= (1ULL << 3);
                    }
                } else {
                    if (toIndex == 63)
                    {
                        black_king &= ~(1ULL << 63);
                        black_king |= (1ULL << 62);
                        black_rooks &= ~(1ULL << 63);
                        black_rooks |= (1ULL << 61);
                    }
                    else if (toIndex == 56)
                    {
                        black_king &= ~(1ULL << 56);
                        black_king |= (1ULL << 58);
                        black_rooks &= ~(1ULL << 56);
                        black_rooks |= (1ULL << 59);
                    }
                }
            }
            if (piece->isPawn()) {
                if (piecePromotion(fromIndex, toIndex, whitesTurn) == true) {
                    delete piece;
                    return true;
                }
            }
            delete piece;
            return true;
        }
    }

    std::cout << "Not a correct move. Try again.\n";
    delete piece;
    return false;
}

char ChessBoard::getPieceSymbolAt(int row, int col) const {
    int square = row * 8 + col;
    if ((white_pawns >> square) & 1)
    {
        return 'P';
    }
    if ((black_pawns >> square) & 1)
    {
        return 'p';
    }
    if ((white_rooks >> square) & 1)
    {
        return 'R';
    }
    if ((black_rooks >> square) & 1)
    {
        return 'r';
    }
    if ((white_knights >> square) & 1)
    {
        return 'N';
    }
    if ((black_knights >> square) & 1)
    {
        return 'n';
    }
    if ((white_bishops >> square) & 1)
    {
        return 'B';
    }
    if ((black_bishops >> square) & 1)
    {
        return 'b';
    }
    if ((white_queen >> square) & 1)
    {
        return 'Q';
    }
    if ((black_queen >> square) & 1)
    {
        return 'q';
    }
    if ((white_king >> square) & 1)
    {
        return 'K';
    }
    if ((black_king >> square) & 1)
    {
        return 'k';
    }
    return '.';
}

std::vector<Move> ChessBoard::getLegalMoves(bool isWhite)
{
    std::vector<Move> legalMoves;
    int start;
    int end;
    if (isWhite)
    {
        start = 0;
        end = 6;
    }
    else
    {
        start = 6;
        end = 12;
    }

    uint64_t* pieces[] ={&white_pawns, &white_knights, &white_bishops, &white_rooks, &white_queen, &white_king,
                         &black_pawns, &black_knights, &black_bishops, &black_rooks, &black_queen, &black_king
    };

    std::vector<Piece*> pieceTypes = {
            new Pawn(true), new Knight(true), new Bishop(true), new Rook(true), new Queen(true), new King(true),
            new Pawn(false), new Knight(false), new Bishop(false), new Rook(false), new Queen(false), new King(false)
    };


    for (int i = start; i < end; ++i)
    {
        uint64_t pieceBitboard = *pieces[i];
        Piece* piece = pieceTypes[i];
        while (pieceBitboard)
        {
            int fromIndex = 0;
            while ((pieceBitboard & (1ULL << fromIndex)) == 0)
            {
                fromIndex++;
            }
            pieceBitboard &= pieceBitboard - 1;
            for (int toIndex = 0; toIndex < 64; ++toIndex)
            {
                if (isOccupiedByOpponent(toIndex, isWhite) || !isOccupied(toIndex))
                {
                    if (piece != nullptr && piece->isValidMove(fromIndex, toIndex, *this, isWhite))
                    {
                        legalMoves.push_back({ fromIndex, toIndex });
                    }
                }
            }
        }
    }
    for(Piece* p : pieceTypes)
    {
        delete p;
    }
    if (isWhite)
    {
        if (white_king & (1ULL << 4))
        {
            if ((white_rooks & (1ULL << 7)) && !isOccupied(5) && !isOccupied(6))
            {
                legalMoves.push_back({ 4, 7 });
            }
            if ((white_rooks & (1ULL << 0)) && !isOccupied(1) && !isOccupied(2) && !isOccupied(3))
            {
                legalMoves.push_back({ 4, 0 });
            }
        }
    }
    else {
        if (black_king & (1ULL << 60))
        {
            if ((black_rooks & (1ULL << 63)) && !isOccupied(61) && !isOccupied(62))
            {
                legalMoves.push_back({ 60, 63 });
            }
            if ((black_rooks & (1ULL << 56)) && !isOccupied(57) && !isOccupied(58) && !isOccupied(59))
            {
                legalMoves.push_back({ 60, 56 });
            }
        }
    }

    return legalMoves;
}

bool ChessBoard::isKingInCheck(bool isWhite)
{
    int kingIndex = -1;
    for (int i = 0; i < 64; i++)
    {
        if (isWhite && (white_king >> i) &1)
        {
            kingIndex = i;
            break;
        }
        if (!isWhite && (black_king >> i) &1)
        {
            kingIndex = i;
            break;
        }
    }
    uint64_t opponentPieces;
    if (isWhite == true)
    {
        opponentPieces = (black_pawns | black_knights | black_bishops | black_rooks | black_queen | black_king);
    }
    else
    {
        opponentPieces = (white_pawns | white_knights | white_bishops | white_rooks | white_queen | white_king);
    }
    for (int i = 0; i < 64; i++)
    {
        if ((opponentPieces >> i) & 1)
        {
            Piece* opponentPiece = getPieceAt(i);
            if (opponentPiece && opponentPiece->isValidMove(i, kingIndex, *this, !isWhite))
            {
                delete opponentPiece;
                return true;
            }
            delete opponentPiece;
        }
    }
    return false;
}

bool ChessBoard::isCheckmate(bool isWhite) {
    if (!isKingInCheck(isWhite))
    {
        return false;
    }

    std::vector<Move> legalMoves = getLegalMoves(isWhite);

    for (const Move& move : legalMoves)
    {
        ChessBoard simulatedBoard = *this;
        int fromIndex = move.from;
        int toIndex = move.to;

        simulatedBoard.makeMove(fromIndex, toIndex, isWhite);

        if (!simulatedBoard.isKingInCheck(isWhite))
        {
            return false;
        }
    }

    return true;
}

bool ChessBoard::isKingInCheckAtSquare(int squareIndex, bool isWhite) const {
    ChessBoard simulatedBoard = *this;
    if (isWhite)
    {
        simulatedBoard.white_king = (1ULL << squareIndex);
    }
    else
    {
        simulatedBoard.black_king = (1ULL << squareIndex);
    }

    return simulatedBoard.isKingInCheck(isWhite);
}

bool ChessBoard::isStaleMate(bool isWhite){
    std::vector<Move>  OpplegalMoves = getLegalMoves(!isWhite);
    if (OpplegalMoves.empty() && !isKingInCheck(isWhite))
    {
        return true;
    }
    return false;
}

const int pawnTableEarly[8][8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        98, 134,  61,  95,  68, 126,  34, -11,
        -6,   7,  26,  31,  65,  56,  25, -20,
        -14,  13,   6,  21,  23,  12,  17, -23,
        -27,  -2,  -5,  12,  17,   6,  10, -25,
        -26,  -4,  -4, -10,   3,   3,  33, -12,
        -35,  -1, -20, -23, -15,  24,  38, -22,
        0,   0,   0,   0,   0,   0,   0,   0
};

const int knightTableEarly[8][8] = {
        -167, -89, -34, -49,  61, -97, -15, -107,
        -73, -41,  72,  36,  23,  62,   7, -17,
        -47,  60,  37,  65,  84, 129,  73,  44,
        -9,  17,  19,  53,  37,  69,  18,  22,
        -13,   4,  16,  13,  28,  19,  21,  -8,
        -23,  -9,  12,  10,  19,  17,  25, -16,
        -29, -53, -12,  -3,  -1,  18, -14, -19,
        -105, -21, -58, -33, -17, -28, -19, -23
};

const int bishopTableEarly[8][8] = {
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -16,  37,  43,  40,  35,  50,  37,  -2,
        -4,   5,  19,  50,  37,  37,   7,  -2,
        -6,  13,  13,  26,  34,  12,  10,   4,
        0,  15,  15,  15,  14,  27,  18,  10,
        4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21
};

const int rookTableEarly[8][8]= {
        32,  42,  32,  51, 63,   9,  31,  43,
        27,  32,  58,  62, 80,  67,  26,  44,
        -5,  19,  26,  36, 17,  45,  61,  16,
        -24, -11,   7,  26, 24,  35,  -8, -20,
        -36, -26, -12,  -1,  9,   -7,   6, -23,
        -45, -25, -16, -17,  3,    0,  -5, -33,
        -44, -16, -20,  -9, -1,   11,  -6, -71,
        -19, -13,   1,  17, 16,    7, -37, -26
};

const int queenTableEarly[8][8] = {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
        -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
        -1, -18,  -9,  10, -15, -25, -31, -50
};

const int kingTableEarly[8][8] = {
        -65,  23,  16, -15, -56, -34,   2,  13,
        29,  -1, -20,  -7,  -8,  -4, -38, -29,
        -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
        1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14
};


const int pawnTableLate[8][8] = {
        0,   0,   0,   0,   0,   0,   0,   0,
        178, 173, 158, 134, 147, 132, 165, 187,
        94, 100,  85,  67,  56,  53,  82,  84,
        32,  24,  13,   5,  -2,   4,  17,  17,
        13,   9,  -3,  -7,  -7,  -8,   3,  -1,
        4,   7,  -6,   1,   0,  -5,  -1,  -8,
        13,   8,   8,  10,  13,   0,   2,  -7,
        0,   0,   0,   0,   0,   0,   0,   0
};

const int knightTableLate[8][8] = {
        -58, -38, -13, -28, -31, -27, -63, -99,
        -25,  -8, -25,  -2,  -9, -25, -24, -52,
        -24, -20,  10,   9,  -1,  -9, -19, -41,
        -17,   3,  22,  22,  22,  11,   8, -18,
        -18,  -6,  16,  25,  16,  17,   4, -18,
        -23,  -3,  -1,  15,  10,  -3, -20, -22,
        -42, -20, -10,  -5,  -2, -20, -23, -44,
        -29, -51, -23, -15, -22, -18, -50, -64
};

const int bishopTableLate[8][8] = {
        -14, -21, -11,  -8,  -7,  -9, -17, -24,
        -8,  -4,   7, -12,  -3, -13,  -4, -14,
        2,  -8,   0,  -1,  -2,   6,   0,   4,
        -3,   9,  12,   9,  14,  10,   3,   2,
        -6,   3,  13,  19,   7,  10,  -3,  -9,
        -12,  -3,   8,  10,  13,   3,  -7, -15,
        -14, -18,  -7,  -1,   4,  -9, -15, -27,
        -23,  -9, -23,  -5,  -9, -16,  -5, -17
};

const int rookTableLate[8][8] = {
        13, 10, 18, 15, 12,  12,   8,   5,
        11, 13, 13, 11,  -3,   3,   8,   3,
        7,  7,  7,   5,   4,  -3,  -5,  -3,
        4,  3, 13,   1,   2,   1,  -1,   2,
        3,  5,  8,   4,  -5,  -6,  -8, -11,
        -4,  0, -5,  -1,  -7, -12,  -8, -16,
        -6, -6,  0,   2,  -9,  -9, -11,  -3,
        -9,  2,  3,  -1,  -5, -13,   4, -20
};

const int queenTableLate[8][8] = {
        -9,  22,  22,  27,  27,  19,  10,  20,
        -17,  20,  32,  41,  58,  25,  30,   0,
        -20,   6,   9,  49,  47,  35,  19,   9,
        3,  22,  24,  45,  57,  40,  57,  36,
        -18,  28,  19,  47,  31,  34,  39,  23,
        -16, -27,  15,   6,   9,  17,  10,   5,
        -22, -23, -30, -16, -16, -23, -36, -32,
        -33, -28, -22, -43,  -5, -32, -20, -41
};

const int kingTableLate[8][8] = {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
        10,  17,  23,  15,  20,  45,  44,  13,
        -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  27,  23,   9, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
};

int evaluatePawnStructure(ChessBoard& board)
{
    int pawnStructureScore = 0;
    for (int file = 0; file < 8; file++)
    {
        int whitePawns = 0;
        int blackPawns = 0;
        for (int rank = 0; rank < 8; rank++)
        {
            char piece = board.getPieceSymbolAt(rank, file);
            if (piece == 'P')
            {
                whitePawns++;
            }
            else if (piece == 'p')
            {
                blackPawns++;
            }
        }
        if (whitePawns > 1)
        {
            pawnStructureScore -= (whitePawns - 1) * 20;
        }
        if (blackPawns > 1)
        {
            pawnStructureScore += (blackPawns - 1) * 20;
        }
    }
    return pawnStructureScore;
}


int ChessBoard::countPassedPawns(bool isWhite) {
    int count = 0;
    uint64_t pawns;
    uint64_t opponentPawns;
    if (isWhite)
    {
        pawns = white_pawns;
        opponentPawns = black_pawns;
    }
    else
    {
        pawns = black_pawns;
        opponentPawns = white_pawns;
    }

    while (pawns) {
        int index = __builtin_ctzll(pawns);
        int col = index % 8;
        int row = index / 8;

        bool isPassed = true;
        if (isWhite)
        {
            for (int r = row + 1; r < 8; r++)
            {
                int index = r * 8 + col;
                if (opponentPawns & (1ULL << index))
                {
                    isPassed = false;
                    break;
                }
            }
        }
        else
        {
            for (int r = row - 1; r >= 0; r--)
            {
                int index = r * 8 + col;
                if (opponentPawns & (1ULL << index))
                {
                    isPassed = false;
                    break;
                }
            }
        }
        if (isPassed)
        {
            count++;
        }
        pawns &= pawns - 1;
    }
    return count;
}


bool isFileOpen(ChessBoard& board, int file) {
    for (int rank = 0; rank < 8; rank++)
    {
        char piece = board.getPieceSymbolAt(rank, file);
        // I decided that an open file should be the case if there are no pawns. on the rank as top engines have done that
        if (piece == 'P' || piece == 'p')
        {
            return false;
        }
    }
    return true;
}


int evaluateRookActivity(ChessBoard& board) {
    int score = 0;
    const int rookOpenBonus = 50;

    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            char piece = board.getPieceSymbolAt(rank, file);
            if (piece == 'R')
            {
                if (isFileOpen(board, file))
                {
                    score += rookOpenBonus;
                }
            }
            else if (piece == 'r')
            {
                if (isFileOpen(board, file))
                {
                    score -= rookOpenBonus;
                }
            }
        }
    }
    return score;
}



int evaluateBoard(ChessBoard board)
{
    int pawn = 100;
    int knight = 320;
    int bishop = 330;
    int rook = 500;
    int queen = 900;
    int king = 20000;
    int evalRating = 0;

    int pieceCount = board.pieceValueBoard();

    const int (*pawnTable)[8];
    const int (*knightTable)[8];
    const int (*bishopTable)[8];
    const int (*rookTable)[8];
    const int (*queenTable)[8];
    const int (*kingTable)[8];

    if (pieceCount > 20) {
        pawnTable = pawnTableEarly;
        knightTable = knightTableEarly;
        bishopTable = bishopTableEarly;
        rookTable = rookTableEarly;
        queenTable = queenTableEarly;
        kingTable = kingTableEarly;
    }
    else
    {
        pawnTable = pawnTableLate;
        knightTable = knightTableLate;
        bishopTable = bishopTableLate;
        rookTable = rookTableLate;
        queenTable = queenTableLate;
        kingTable = kingTableLate;
    }

    for (int i = 0; i <= 63; i++)
    {
        int row = i / 8;
        int col = i % 8;
        char pieceSymbol = board.getPieceSymbolAt(row, col);
        switch (pieceSymbol)
        {
            case 'P':
                evalRating = evalRating + pawn;
                evalRating = evalRating + pawnTable[7 - row][col];
                if (row == 7)
                {
                    evalRating += 900;
                }
                break;
            case 'p':
                evalRating = evalRating - pawn;
                evalRating = evalRating - pawnTable[row][col];
                if (row == 0)
                {
                    evalRating += 900;
                }
                break;
            case 'N':
                evalRating = evalRating + knight;
                evalRating = evalRating + knightTable[row][col];
                break;
            case 'n':
                evalRating = evalRating - knight;
                evalRating = evalRating - knightTable[7 - row][col];
                break;
            case 'B':
                evalRating = evalRating + bishop;
                evalRating = evalRating + bishopTable[row][col];
                break;
            case 'b':
                evalRating = evalRating - bishop;
                evalRating = evalRating - bishopTable[7 - row][col];
                break;
            case 'R':
                evalRating = evalRating + rook;
                evalRating = evalRating + rookTable[row][col];
                break;
            case 'r':
                evalRating = evalRating - rook;
                evalRating = evalRating - rookTable[7 - row][col];
                break;
            case 'Q':
                evalRating = evalRating + queen;
                evalRating = evalRating + queenTable[row][col];
                // Queen was moving too much in the early game
                if (board.pieceValueBoard() > 24)
                {
                    evalRating -= 70;
                }
                break;
            case 'q':
                evalRating = evalRating - queen;
                evalRating = evalRating - queenTable[7 - row][col];
                // Queen was moving too much in the early game
                if (board.pieceValueBoard() > 24)
                {
                    evalRating += 70;
                }
                break;
            case 'K':
                evalRating = evalRating + king;
                evalRating = evalRating + kingTable[row][col];
                break;
            case 'k':
                evalRating = evalRating - king;
                evalRating = evalRating - kingTable[7 - row][col];
                break;
        }
    }
    const int castlingBonus = 100;

    if (board.white_king & (1ULL << 6))
    {
        evalRating = evalRating + castlingBonus;
    }
    if (board.white_king & (1ULL << 2))
    {
        evalRating = evalRating + castlingBonus;
        evalRating = evalRating - 30;
    }


    if (board.black_king & (1ULL << 62))
    {
        evalRating = evalRating - castlingBonus;
    }
    if (board.black_king & (1ULL << 58))
    {
        evalRating = evalRating - castlingBonus;
        evalRating = evalRating + 30;
    }

    std::vector<Move> whiteMoves = board.getLegalMoves(true);
    std::vector<Move> blackMoves = board.getLegalMoves(false);
    int mobilityBonus = 10 * (static_cast<int>(whiteMoves.size()) - static_cast<int>(blackMoves.size()));
    evalRating += mobilityBonus;

    if (pieceCount <= 4)
    {
        int whiteKingIndex = __builtin_ctzll(board.white_king);
        int blackKingIndex = __builtin_ctzll(board.black_king);
        int whiteKingRow = whiteKingIndex / 8, whiteKingCol = whiteKingIndex % 8;
        int blackKingRow = blackKingIndex / 8, blackKingCol = blackKingIndex % 8;
        int whiteKingDistanceToCenter = std::max(3 - whiteKingRow, whiteKingRow - 4) + std::max(3 - whiteKingCol, whiteKingCol - 4);
        int blackKingDistanceToCenter = std::max(3 - blackKingRow, blackKingRow - 4) + std::max(3 - blackKingCol, blackKingCol - 4);

        evalRating += 10 * (blackKingDistanceToCenter - whiteKingDistanceToCenter);
        evalRating += 80 * (board.countPassedPawns(true) - board.countPassedPawns(false));

        if (pieceCount == 3)
        {
            uint64_t rooks = board.white_rooks | board.black_rooks;
            if (rooks)
            {
                int rookIndex = __builtin_ctzll(rooks);
                int rookCol = rookIndex % 8;
                if (std::abs(rookCol - blackKingCol) <= 1)
                {
                    evalRating += 50;
                }
            }
        }
    }

    // stacked pawns
    int pawnStructure = evaluatePawnStructure(board);
    evalRating = evalRating + pawnStructure;

    // open file rook stuff
    int rookOpenFile =  evaluateRookActivity(board);
    evalRating = evalRating + rookOpenFile;
    return evalRating;
}

int ChessBoard::pieceValueBoard() const{
    int pieceCount = 0;
    for (int index = 0; index < 64; index++)
    {
        if (isOccupied(index))
        {
            pieceCount++;
        }
    }
    return pieceCount;
}


int evaluateMove(const Move& move, ChessBoard& board) {
    int score = 0;
    int row = move.to / 8;
    int col = move.to % 8;
    char capturedPiece = board.getPieceSymbolAt(row, col);
    if (board.getPieceSymbolAt(row, col) != '.')
        switch (capturedPiece)
        {
            case 'q':
                score += 900;
                break;
            case 'Q':
                score += 900;
                break;
            case 'r':
                score += 500;
                break;
            case 'R':
                score += 500;
                break;
            case 'b':
                score += 330;
                break;
            case 'B':
                score += 330;
                break;
            case 'n':
                score += 320;
                break;
            case 'N':
                score += 320;
                break;
            case 'p':
                score += 100;
                break;
            case 'P':
                score += 100;
                break;
        }
    if (board.getPieceSymbolAt(row, col) == 'P' && row == 7)
    {
        score += 500;
    }
    return score;
}



MoveScore minimaxCalculation(ChessBoard board, int depth, bool isWhite, int beta, int alpha)
{
    std::vector<Move> legalMoves = board.getLegalMoves(isWhite);

    if (depth == 0 || legalMoves.empty())
    {

        if (legalMoves.empty())
        {
            if (board.isKingInCheck(isWhite))
            {
                int score;
                if (isWhite)
                {
                    score = -1000000;
                }
                else
                {
                    score = 1000000;
                }
                return MoveScore { score, -2, -2 };
            }
            else
            {
                return MoveScore { 0, -2, -2 };
            }
        }
        return MoveScore { evaluateBoard(board), -2, -2 };
    }

    MoveScore bestMove;
    bestMove.fromIndex = -1;
    bestMove.toIndex = -1;

    if (isWhite)
    {
        bestMove.score = -99999;
    }
    else
    {
        bestMove.score = 99999;
    }

    std::sort(legalMoves.begin(), legalMoves.end(),[&](const Move& firstMove, const Move& secondMove)
              {
                  int firstScore = evaluateMove(firstMove, board);
                  int secondScore = evaluateMove(secondMove, board);
                  if (firstScore > secondScore)
                  {
                      return true;
                  }
                  else
                  {
                      return false;
                  }
              }
    );

    for (int i = 0; i < legalMoves.size(); i++)
    {
        Move move = legalMoves[i];
        ChessBoard newBoard = board;
        bool moveSuccess = newBoard.makeMove(move.from, move.to, isWhite);
        if (!moveSuccess)
        {
            continue;
        }
//        // LMR Will change maybe. (may make the depth -1 so i dont mis good moves)
        int newDepth = depth - 1;
//        if (i >= 3 && depth >= 5 && board.getPieceSymbolAt(move.to / 8, move.to % 8) == '.') {
//            if (depth - 2 >= 2) {
//                newDepth = depth - 2;
//            }
//        }


        MoveScore currentMove = minimaxCalculation(newBoard, newDepth, !isWhite, beta, alpha);
        if (isWhite)
        {
            if (currentMove.score > bestMove.score)
            {
                bestMove.score = currentMove.score;
                bestMove.fromIndex = move.from;
                bestMove.toIndex = move.to;
            }
            alpha = std::max(alpha, bestMove.score);
            if (beta <= alpha)
            {
                break;
            }
        }
        else
        {
            if (currentMove.score < bestMove.score)
            {
                bestMove.score = currentMove.score;
                bestMove.fromIndex = move.from;
                bestMove.toIndex = move.to;
            }
            beta = std::min(beta, bestMove.score);
            if (beta <= alpha)
            {
                break;
            }
        }
    }

    return bestMove;
}


bool ChessBoard::piecePromotion(int fromIndex, int toIndex, bool whitesTurn) {
    int row = toIndex / 8;
    uint64_t toMask = 1ULL << toIndex;
    char pieceSymbol;
    if (whitesTurn && row == 7)
    {
        pieceSymbol = 'Q';
        white_pawns &= ~toMask;
        switch (pieceSymbol)
        {
            case 'R':
                white_rooks |= toMask;
                break;
            case 'K':
                white_knights |= toMask;
                break;
            case 'B':
                white_bishops |= toMask;
                break;
            case 'Q':
                white_queen |= toMask;
                break;
        }
    }
    else if (!whitesTurn && row == 0)
    {
        pieceSymbol = 'q';
        black_pawns &= ~toMask;
        switch (pieceSymbol)
        {
            case 'r':
                black_rooks |= toMask;
                break;
            case 'k':
                black_knights |= toMask;
                break;
            case 'b':
                black_bishops |= toMask;
                break;
            case 'q':
                black_queen |= toMask;
                break;
        }
    }
    else
    {
        return false;
    }
    return true;


}

std::string ChessBoard::toFEN(bool whitesTurn) const {
    std::stringstream fenStream;
    for (int row = 7; row >= 0; --row)
    {
        int emptySquares = 0;
        for (int col = 0; col < 8; ++col)
        {
            char pieceSymbol = getPieceSymbolAt(row, col);
            if (pieceSymbol == '.')
            {
                emptySquares++;
            }
            else
            {
                if (emptySquares > 0)
                {
                    fenStream << emptySquares;
                    emptySquares = 0;
                }
                fenStream << pieceSymbol;
            }
        }
        if (emptySquares > 0)
        {
            fenStream << emptySquares;
        }
        if (row > 0)
        {
            fenStream << '/';
        }
    }

    if (whitesTurn)
    {
        fenStream << " w ";
    }
    else
    {
        fenStream << " b ";
    }
    fenStream << "KQkq - 0 1";

    return fenStream.str();
}

int ChessBoard::transformIndexForBlack(int index) {
    return 63 - index;
}

int ChessBoard::chessInputToIndex(const std::string& input) {
    if (input.length() != 2)
    {
        return -1;
    }
    char fileChar = input[0];
    char rankChar = input[1];
    if (fileChar < 'a' || fileChar > 'h' || rankChar < '1' || rankChar > '8') {
        return -1; // Invalid file or rank
    }
    int file = fileChar - 'a';
    int rank = rankChar - '1';
    int index = rank * 8 + file;
    return index; // Calculate the index (rank * 8 + file for row-major order)
}
