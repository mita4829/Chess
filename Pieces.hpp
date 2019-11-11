#ifndef PIECES_HPP
#define PIECES_HPP

#include "Foundation.hpp"

#define WHITE_PIECE 0
#define BLACK_PIECE 1

#define KING_HAS_MOVED 0x1
#define KING_ROOK_HAS_MOVED 0x2
#define QUEEN_ROOK_HAS_MOVED 0x4

enum PieceType {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

struct PlayingState {
    UInt8 Castle;
    // 0x1 = King has moved
    // 0x2 = King Rook has moved
    // 0x4 = Queen Rook has moved
    
    // The 
    Move        LastMove;
    PieceType   LastMovedPiece;
};

struct Pieces {
    UInt64 Pawns;
    UInt64 Knights;
    UInt64 Bishops;
    UInt64 Rooks;
    UInt64 Queen;
    UInt64 King;
    UInt8  Color;
    PlayingState State;
    UInt64 Reserved;  // Used as placeholder for multiple type pieces
    UInt64 Reserved2; // Used as placeholder for legal move check
};

UInt64 PiecesPawnMove(Pieces*, Pieces*);
UInt64 PiecesKnightMove(Pieces*, Pieces*);
UInt64 PiecesRookMove(Pieces*, Pieces*);
UInt64 PiecesBishopMove(Pieces*, Pieces*);
UInt64 PiecesQueenMove(Pieces*, Pieces*);
UInt64 PiecesKingMove(Pieces*, Pieces*, bool = false);
PieceType PiecesMapSquareToPiece(Pieces*, UInt64);
UInt64 PiecesGetAttackSquares(Pieces*, Pieces*);

#endif // PIECES_HPP
