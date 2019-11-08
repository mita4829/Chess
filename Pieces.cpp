#include "Pieces.hpp"

#define Union(pieces) (pieces->Pawns   | \
                       pieces->Knights | \
                       pieces->Bishops | \
                       pieces->Rooks   | \
                       pieces->Queen   | \
                       pieces->King    )

#define Intersect(A, B) ((A) & (~(B)))
#define ColEx(X) (((RANK_8 & X) >> 56) | \
                    ((RANK_7 & X) >> 48) | \
                    ((RANK_6 & X) >> 40) | \
                    ((RANK_5 & X) >> 32) | \
                    ((RANK_4 & X) >> 24) | \
                    ((RANK_3 & X) >> 16) | \
                    ((RANK_2 & X) >> 8)  | \
                    ((RANK_1 & X)))        \

#define LeastSigBit(X) ((X) & (~(X) + 1))
#define minz(X,Y) ((X < Y && X != 0 ? X : (Y != 0 ? Y : X)))
#define min(X,Y) (X < Y ? X : Y)
#define max(X,Y) (X > Y ? X : Y)

UInt64 MostSigBit(UInt64 value)
{
    UInt64 msb = 0x8000000000000000;
    if (value == 0)
    {
        return 0;
    }
    
    while ((msb & value) == 0)
    {
        msb >>= 1;
    }
    return msb;
}

inline UInt64 RowIndex(UInt64 piece)
{
    UInt64 row = 0;
    for (row = 0; row < 8; row++)
    {
        if ((piece & (0xFFULL << (row*8))) != 0)
        {
            break;
        }
    }
    return row;
}

inline UInt64 ColIndex(UInt64 piece)
{
    UInt64 bit = ColEx(piece);
    UInt64 col = 0;
    while (bit >>= 1)
        col++;
    return col;
}

UInt64 PiecesPawnMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 upLeftAttack;
    UInt64 uprightAttack;
    UInt64 firstMove;
    UInt64 forward;
    UInt64 aPLocation;
    UInt64 bPLocation;
    
    aPLocation = Union(A);
    bPLocation = Union(B);
    
    upLeftAttack  = Intersect((bPLocation & (A->Pawns << 0x9)), FILE_A);
    uprightAttack = Intersect((bPLocation & (A->Pawns << 0x7)), FILE_H);
    
    // Pawns on their first move.
    // Get all the pawns on their 2nd rank
    firstMove       = (A->Pawns & RANK_2);
    // Then remove the pawn moves where there are pieces blocking on the 3rd rank
    firstMove       = Intersect((firstMove << 8), (aPLocation | bPLocation));
    // Then condition out the moves where pieces are blocking the 4th rank
    firstMove      |= Intersect((firstMove << 8), (aPLocation | bPLocation));
    
    // Non-starting pawns
    forward       = Intersect(((A->Pawns ^ RANK_2) << 8), (aPLocation | bPLocation));
    
    aMoves = forward | firstMove | upLeftAttack | uprightAttack;
    
    // TODO: Add logic for en passant
    return aMoves;
}

UInt64 PiecesKnightMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 aPLocation = Union(A);
    UInt64 knights = A->Knights;
    
    aMoves = (Intersect(knights << 17, FILE_A) |
              Intersect(knights << 10, FILE_A | FILE_B) |
              Intersect(knights >> 6,  FILE_A | FILE_B) |
              Intersect(knights >> 15, FILE_A) |
              Intersect(knights << 15, FILE_H) |
              Intersect(knights << 6,  FILE_G | FILE_H) |
              Intersect(knights >> 10, FILE_G | FILE_H) |
              Intersect(knights >> 17, FILE_H));
    
    return aMoves;
}

UInt64 PiecesBishopMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 aPLocation, bPLocation;
    UInt64 row, col, square;
    UInt64 bishopRow, bishopCol;
    
    bishopRow = RowIndex(A->Bishops);
    bishopCol = ColIndex(A->Bishops);
    
    aPLocation = Union(A);
    bPLocation = Union(B);
    
    aMoves = 0;
    
    for (row = bishopRow, col = bishopCol, square = A->Bishops;
         row < 7 && col < 7;
         row++, col++)
    {
        square <<= 9;
        if (aPLocation & square)
        {
            break;
        }
        aMoves |= square;
        if (bPLocation & square)
        {
            break;
        }
    }
    
    for (row = bishopRow, col = bishopCol, square = A->Bishops;
         row < 7 && col > 0;
         row++, col--)
    {
        square <<= 7;
        if (aPLocation & square)
        {
            break;
        }
        aMoves |= square;
        if (bPLocation & square)
        {
            break;
        }
    }
    
    for (row = bishopRow, col = bishopCol, square = A->Bishops;
         row > 0 && col > 0;
         row--, col--)
    {
        square >>= 9;
        if (aPLocation & square)
        {
            break;
        }
        aMoves |= square;
        if (bPLocation & square)
        {
            break;
        }
    }
    
    for (row = bishopRow, col = bishopCol, square = A->Bishops;
         row > 0 && col < 7;
         row--, col++)
    {
        square >>= 7;
        if (aPLocation & square)
        {
            break;
        }
        aMoves |= square;
        if (bPLocation & square)
        {
            break;
        }
    }
    
    return aMoves;
}

UInt64 PiecesRookMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves, horizontalMoves, verticalMoves;
    UInt64 aPLocation;
    UInt64 bPLocation;
    UInt64 lsb, msb;
    UInt64 lmask, umask;
    
    UInt64 rookFile;
    UInt64 rookRank;
    UInt64 rookRowIndex = RowIndex(A->Rooks);
    UInt64 rookColIndex = ColIndex(A->Rooks);
    
    aPLocation = Union(A);
    bPLocation = Union(B);
    rookRank   = Rank[rookRowIndex];
    rookFile   = File[rookColIndex];
    lmask      = (A->Rooks-1);
    umask      = ~(A->Rooks-1) << 1;
    
    aPLocation &= (rookRank | rookFile);
    bPLocation &= (rookRank | rookFile);
    
    // First calculate the horizontal rank moves
    // Find the closest piece east of the rook.
    // If it's the same color, subtract that square, else
    // include it in the row.
    lsb = minz(MostSigBit(aPLocation & umask & rookRank),
               MostSigBit(bPLocation & umask & rookRank));

    if (lsb & bPLocation)
    {
        // If the east most piece closest to the rook is the not the
        // same color, it is attackable, so include it.
        lsb <<= 1;
    }
    // Fill out the row with legal moves by doing msb - 1.
    // This will cause all the remaining bits to be 1 due to
    // msb being a 2^n integer.
    horizontalMoves = (lsb - 1);
    
    // Find the west most piece closest to rook
    msb = max(LeastSigBit(aPLocation & lmask & rookRank),
              LeastSigBit(bPLocation & lmask & rookRank));
    
    // Now either include or exclude the west most piece depending on color
    if (msb & aPLocation)
    {
        msb <<= 1;
    }
    // There can be a case where there are no pieces west of the rook. This will
    // cause msb = 0 => msb = (UInt64)-1 which will erase everything when Intersected.
    // Because of this, keep msb = 0.
    msb = msb != 0 ? msb - 1 : msb;

    horizontalMoves = Intersect(horizontalMoves, msb) & rookRank;

    // Now calculate the vertical moves similar to how the horizontal moves where
    // calculated
    lsb = min(LeastSigBit(aPLocation & umask & rookFile),
              LeastSigBit(bPLocation & umask & rookFile));

    if (lsb & bPLocation)
    {
        lsb <<= 1;
    }
    verticalMoves = (lsb - 1);
    msb = max(MostSigBit(aPLocation & lmask & rookFile),
              MostSigBit(bPLocation & lmask & rookFile));
    if (msb & aPLocation)
    {
        msb <<= 1;
    }
    msb = msb != 0 ? msb - 1 : msb;
    
    verticalMoves = Intersect(verticalMoves, msb) & rookFile;
    
    aMoves = Intersect(horizontalMoves | verticalMoves, A->Rooks);
    return aMoves;
}
