#ifndef PIECES_HPP
#define PIECES_HPP

#include "Foundation.hpp"

struct Pieces {
    UInt64 Pawns;
    UInt64 Knights;
    UInt64 Bishops;
    UInt64 Rooks;
    UInt64 Queen;
    UInt64 King;
};

struct PlayingState {
    UInt8 Castle;
    // 0x1 = King has moved
    // 0x2 = King Rook has moved
    // 0x4 = Queen Rook has moved
};

UInt64 PiecesPawnMove(Pieces*, Pieces*);
UInt64 PiecesKnightMove(Pieces*, Pieces*);
UInt64 PiecesRookMove(Pieces*, Pieces*);
UInt64 PiecesBishopMove(Pieces*, Pieces*);

#endif // PIECES_HPP
