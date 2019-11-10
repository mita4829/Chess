#ifndef PIECES_HPP
#define PIECES_HPP

#include "Foundation.hpp"

#define WHITE_PIECE 0
#define BLACK_PIECE 1

struct Pieces {
    UInt64 Pawns;
    UInt64 Knights;
    UInt64 Bishops;
    UInt64 Rooks;
    UInt64 Queen;
    UInt64 King;
    UInt64 Reserved;
    UInt64 Reserved2;
    UInt8  Color;
};

struct PlayingState {
    UInt8 Castle;
    // 0x1 = King has moved
    // 0x2 = King Rook has moved
    // 0x4 = Queen Rook has moved
};

enum PieceType {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    NONE
};

UInt64 PiecesPawnMove(Pieces*, Pieces*);
UInt64 PiecesKnightMove(Pieces*, Pieces*);
UInt64 PiecesRookMove(Pieces*, Pieces*);
UInt64 PiecesBishopMove(Pieces*, Pieces*);
UInt64 PiecesQueenMove(Pieces*, Pieces*);
UInt64 PiecesKingMove(Pieces*, Pieces*);
PieceType PiecesMapSquareToPiece(Pieces*, UInt64);
UInt64 PiecesGetAttackSquares(Pieces*, Pieces*);

#endif // PIECES_HPP
