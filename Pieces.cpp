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

/*
 Function: MostSigBit
 Parameters:
    - UInt64 value. A value which represents the board.
 Return:
    UInt64. The most significant bit.
 Notes:
 */
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

/*
 Function: RowIndex
 Parameters:
    - UInt64 piece. A piece on a board
 Return:
    UInt64. The row/Rank of where the piece is located.
 Notes:
    This function assumes there's only one piece on the board.
    The return value is 0th index.
 */
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

/*
 Function: ColIndex
 Parameters:
    - UInt64 piece. A piece on a board
 Return:
    UInt64. The column/File of where the piece is located.
 Notes:
    This function assumes there's only one piece on the board.
    The return value is 0th index.
 */
inline UInt64 ColIndex(UInt64 piece)
{
    UInt64 bit = ColEx(piece);
    UInt64 col = 0;
    while (bit >>= 1)
        col++;
    return col;
}

/*
 Function: PiecesPawnMove
 Parameters:
     - Pieces A. The moving side pieces
     - Pieces B. The non-moving side pieces
 Return:
     UInt64. The squares where the pawns can go.
 Notes:
     This function assume all A pawns are going
     south to north.
 */
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

/*
 Function: PiecesPawnMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the knights can go.
 Notes:
 */
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

/*
 Function: PiecesBishopMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the bishop can go.
 Notes:
 */
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
    
    // Get the moves going NE
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
    
    // Get the moves going NW
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
    
    // Get the moves going SW
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
    
    // Get the moves going SE
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

/*
 Function: PiecesBishopMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the rook can go.
 Notes:
 */
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

/*
 Function: PiecesQueenMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the queen can go.
 Notes:
    This function combines the logic of the rook and bishop
 */
UInt64 PiecesQueenMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    
    A->Rooks |= A->Queen;
    aMoves = PiecesRookMove(A, B);
    A->Rooks = Intersect(A->Rooks, A->Queen);
    
    A->Bishops |= A->Queen;
    aMoves |= PiecesBishopMove(A, B);
    A->Bishops = Intersect(A->Bishops, A->Queen);
    
    return aMoves;
}

/*
 Function: PiecesKingMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the king can go.
 Notes:
    this function does not account if the king
    is/will be in check.
 */
UInt64 PiecesKingMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 aPLocation;
    UInt64 king;
    
    king        = A->King;
    aPLocation  = Union(A);
    
    aMoves = (Intersect(king << 7, FILE_H) |
              king << 8 |
              Intersect(king << 9, FILE_A) |
              Intersect(king << 1, FILE_A) |
              Intersect(king >> 1, FILE_H) |
              Intersect(king >> 7, FILE_A) |
              king >> 8 |
              Intersect(king >> 9, FILE_H));
    
    aMoves = Intersect(aMoves, aPLocation);
    
    // TODO: Castling logic
    
    return aMoves;
}

/*
 Function: PiecesGetAttackSquares
 Parameters:
    - Pieces A. Attacking side
    - Pieces B. Non-attacking side
 Return:
    UInt64. The squares that side A attacks.
 Notes:
 */
UInt64 PiecesGetAttackSquares(Pieces* A, Pieces* B)
{
    UInt64 aSquares;
    
    aSquares  = PiecesPawnMove(A, B);
    aSquares |= PiecesKnightMove(A, B);
    aSquares |= PiecesBishopMove(A, B);
    aSquares |= PiecesRookMove(A, B);
    aSquares |= PiecesQueenMove(A, B);
    aSquares |= PiecesKingMove(A, B);
    
    return aSquares;
}
