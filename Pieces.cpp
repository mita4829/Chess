#include "Pieces.hpp"

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
 Function: PiecesPawnMoveFast
 Parameters:
    - Pieces* A. The moving side pieces
    - Pieces* B. The non-moving side pieces
 Return:
    UInt64. The squares where the pawn attacks "may" happen
 Notes:
    This function return squares that pawns "may" attack
    if the opposite color happen to place a piece there.
 */
UInt64 PiecesPawnMoveFast(Pieces* A, Pieces* B)
{
    UInt64 aMoves = 0;
    
    if (A->Pawns == 0)
    {
        return 0;
    }
    
    if (A->Color == WHITE_PIECE)
    {
        aMoves  = Intersect((A->Pawns << 0x9), FILE_A);
        aMoves |= Intersect((A->Pawns << 0x7), FILE_H);
    }
    else
    {
        aMoves  = Intersect((A->Pawns >> 0x9), FILE_H);
        aMoves |= Intersect((A->Pawns >> 0x7), FILE_A);
    }
    
    return aMoves;
}

/*
 Function: PiecesPawnAttack
 Parameters:
    - Piece* A. Attacking side
    - Piece* B. Non-attacking side
 Return:
    UInt64. The attack squares for the pawns
 Notes:

 */
UInt64 PiecesPawnAttack(Pieces* A, Pieces* B)
{
    UInt64 aMoves = 0;
    UInt64 bPLocation;
    
    if (A->Pawns == 0)
    {
        return 0;
    }
    
    bPLocation = Union(B);
    
    if (A->Color == WHITE_PIECE)
    {
        aMoves  = Intersect((bPLocation & (A->Pawns << 0x9)), FILE_A);
        aMoves |= Intersect((bPLocation & (A->Pawns << 0x7)), FILE_H);
    }
    else
    {
        aMoves  = Intersect((bPLocation & (A->Pawns >> 0x9)), FILE_H);
        aMoves |= Intersect((bPLocation & (A->Pawns >> 0x7)), FILE_A);
    }
    
    return aMoves;
}

/*
 Function: PiecesWhitePawnMove
 Parameters:
     - Pieces* A. The moving side pieces
     - Pieces* B. The non-moving side pieces
 Return:
     UInt64. The squares where white the pawns can go.
 Notes:
 */
UInt64 PiecesWhitePawnMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 attacks;
    UInt64 firstMove;
    UInt64 forward;
    UInt64 aPLocation, bPLocation;
    UInt64 enPassantSquares, enPassantEscapeSquare;
    
    aPLocation = Union(A);
    bPLocation = Union(B);
    
    attacks = PiecesPawnAttack(A, B);
    
    // Pawns on their first move.
    // Get all the pawns on their 2nd rank
    firstMove       = (A->Pawns & RANK_2);
    // Then remove the pawn moves where there are pieces blocking on the 3rd rank
    firstMove       = Intersect((firstMove << 8), (aPLocation | bPLocation));
    // Then condition out the moves where pieces are blocking the 4th rank
    firstMove      |= Intersect((firstMove << 8), (aPLocation | bPLocation));
    
    // Non-starting pawns
    forward       = Intersect((Intersect(A->Pawns, RANK_2) << 8), (aPLocation | bPLocation));
    
    aMoves = forward | firstMove | attacks;
    
    // Check for En Passants
    // The following checks
    // - If Black's last moved piece is a pawn, and
    // - There exist a white pawn on the 5th rank, and
    // - Black's pawn moved two squares on its first move
    if (B->State.LastMovedPiece == PAWN &&
        (A->Pawns & RANK_5) &&
        (B->State.LastMove.StartSquare & RANK_7) &&
        (B->State.LastMove.EndSquare   & RANK_5))
    {
        enPassantSquares  = Intersect(A->Pawns << 7, FILE_H);
        enPassantSquares |= Intersect(A->Pawns << 9, FILE_A);
        enPassantEscapeSquare = B->State.LastMove.EndSquare << 8;
        
        enPassantSquares &= enPassantEscapeSquare;
        aMoves |= enPassantSquares;
    }
    
    return aMoves;
}

/*
 Function: PiecesBlackPawnMove
 Parameters:
    - Pieces* A. The moving side pieces
    - Pieces* B. The non-moving side pieces
 Return:
    UInt64. The squares where black the pawns can go.
 Notes:
 */
UInt64 PiecesBlackPawnMove(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 attacks;
    UInt64 firstMove;
    UInt64 forward;
    UInt64 aPLocation, bPLocation;
    UInt64 enPassantSquares, enPassantEscapeSquare;
    
    aPLocation = Union(A);
    bPLocation = Union(B);

    attacks = PiecesPawnAttack(A, B);
    
    // Pawns on their first move.
    // Get all the pawns on their 7th rank
    firstMove       = (A->Pawns & RANK_7);
    // Then remove the pawn moves where there are pieces blocking on the 6rd rank
    firstMove       = Intersect((firstMove >> 8), (aPLocation | bPLocation));
    // Then condition out the moves where pieces are blocking the 5th rank
    firstMove      |= Intersect((firstMove >> 8), (aPLocation | bPLocation));
    
    // Non-starting pawns
    forward       = Intersect((Intersect(A->Pawns, RANK_7) >> 8), (aPLocation | bPLocation));
    
    aMoves = forward | firstMove | attacks;
    
    // Check for En Passants
    // The following checks
    // - If White's last moved piece is a pawn, and
    // - There exist a black pawn on the 4th rank, and
    // - White's pawn moved two squares on its first move
    if (B->State.LastMovedPiece == PAWN &&
        (A->Pawns & RANK_4) &&
        (B->State.LastMove.StartSquare & RANK_2) &&
        (B->State.LastMove.EndSquare   & RANK_4))
    {
        enPassantSquares  = Intersect(A->Pawns >> 7, FILE_A);
        enPassantSquares |= Intersect(A->Pawns >> 9, FILE_H);
        enPassantEscapeSquare = B->State.LastMove.EndSquare >> 8;
        
        enPassantSquares &= enPassantEscapeSquare;
        aMoves |= enPassantSquares;
    }
    
    return aMoves;
}

/*
 Function: PiecesPawnMoveEx
 Parameters:
    - Pieces* A. The moving side pieces
    - Pieces* B. The non-moving side pieces
 Return:
    UInt64. The squares where the pawns can go.
 Notes:
    This function assumes there's only one pawn on the board.
 */
UInt64 PiecesPawnMoveEx(Pieces* A, Pieces* B)
{
    UInt64 aMoves = 0;
    
    if (A->Color == WHITE_PIECE)
    {
        aMoves = PiecesWhitePawnMove(A, B);
    }
    else
    {
        aMoves = PiecesBlackPawnMove(A, B);
    }
    
    return aMoves;
}

/*
 Function: PiecesPawnMove
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the bishop can go.
 Notes:
 
 */
UInt64 PiecesPawnMove(Pieces* A, Pieces* B)
{
    Pieces movingSide;
    UInt64 aMoves;
    
    UInt64 pawn   = A->Pawns;
    UInt64 square  = 0x1;
    aMoves = 0;
    
    // Special case when there are no pawns on the board
    if (pawn == 0)
    {
        return 0;
    }
    
    memcpy(&movingSide, A, sizeof(Pieces));
    // There may be more than one pawn. Iterate the board
    // and calculate the moves for each pawn.
    for (UInt64 i = 0; i < 63; i++, square <<= 1)
    {
        if (pawn & square)
        {
            // Found a pawn. Move all other pawns to reserved,
            // so it doesn't interfere with the pawns move calculation
            movingSide.Pawns     = square;
            movingSide.Reserved  = pawn & ~square;
            aMoves |= PiecesPawnMoveEx(&movingSide, B);
            
            movingSide.Pawns      = pawn;
            movingSide.Reserved   = 0;
        }
        
    }
    
    return aMoves;
}

/*
 Function: PiecesKnightMove
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
 Function: PiecesBishopMoveEx
 Parameters:
    - Pieces A. The moving side pieces
    - Pieces B. The non-moving side pieces
 Return:
    UInt64. The squares where the bishop can go.
 Notes:
    This is an private function under the assumption
    that there's only one bishop.
 */
UInt64 PiecesBishopMoveEx(Pieces* A, Pieces* B)
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
    UInt64. The squares where the bishop can go.
 Notes:

 */
UInt64 PiecesBishopMove(Pieces* A, Pieces* B)
{
    Pieces movingSide;
    UInt64 aMoves;
    
    UInt64 bishop  = A->Bishops;
    UInt64 square  = 0x1;
    aMoves = 0;
    
    // Special case when there are no bishops on the board
    if (bishop == 0)
    {
        return 0;
    }
    
    memcpy(&movingSide, A, sizeof(Pieces));
    // There may be more than one bishop. Iterate the board
    // and calculate the moves for each rook.
    for (UInt64 i = 0; i < 63; i++, square <<= 1)
    {
        if (bishop & square)
        {
            // Found a rook. Move all other bishops to reserved,
            // so it doesn't interfere with the bishops move calculation
            movingSide.Bishops   = square;
            movingSide.Reserved  = bishop & ~square;
            aMoves |= PiecesBishopMoveEx(&movingSide, B);
            
            movingSide.Bishops    = bishop;
            movingSide.Reserved   = 0;
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
    This is a private function that assumes
    there's only one rook.
 */
UInt64 PiecesRookMoveEx(Pieces* A, Pieces* B)
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
    lsb = minz(LeastSigBit(aPLocation & umask & rookFile),
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
    Pieces movingSide;
    UInt64 aMoves;
    
    UInt64 rooks  = A->Rooks;
    UInt64 square = 0x1;
    aMoves = 0;
    
    // Special case when there are no rooks on the board
    if (rooks == 0)
    {
        return 0;
    }
    
    memcpy(&movingSide, A, sizeof(Pieces));
    // There may be more than one rook. Iterate the board
    // and calculate the moves for each rook.
    for (UInt64 i = 0; i < 63; i++, square <<= 1)
    {
        if (rooks & square)
        {
            // Found a rook. Move all other rooks to reserved,
            // so it doesn't interfere with the rook move calculation
            movingSide.Rooks    = square;
            movingSide.Reserved = rooks & ~square;
            aMoves |= PiecesRookMoveEx(&movingSide, B);
            
            movingSide.Rooks    = rooks;
            movingSide.Reserved = 0;
        }
        
    }
    
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
    This function combines the logic of the rook and bishop.
    This is a private function with the assumption
    there's only one queen.
 */
UInt64 PiecesQueenMoveEx(Pieces* A, Pieces* B)
{
    UInt64 aMoves;
    UInt64 aPLocation, bPLocation;
    UInt64 row, col, square;
    UInt64 queenRow, queenCol;
    UInt64 horizontalMoves, verticalMoves;
    UInt64 lsb, msb;
    UInt64 lmask, umask;
    
    UInt64 queenFile;
    UInt64 queenRank;
    
    queenRow = RowIndex(A->Queen);
    queenCol = ColIndex(A->Queen);
    
    aPLocation = Union(A);
    bPLocation = Union(B);
    
    aMoves = 0;
    
    // Get the moves going NE
    for (row = queenRow, col = queenCol, square = A->Queen;
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
    for (row = queenRow, col = queenCol, square = A->Queen;
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
    for (row = queenRow, col = queenCol, square = A->Queen;
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
    for (row = queenRow, col = queenCol, square = A->Queen;
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
    
    queenRank   = Rank[queenRow];
    queenFile   = File[queenCol];
    lmask      = (A->Queen-1);
    umask      = ~(A->Queen-1) << 1;
    
    aPLocation &= (queenRank | queenFile);
    bPLocation &= (queenRank | queenFile);
    
    // First calculate the horizontal rank moves
    // Find the closest piece east of the queen.
    // If it's the same color, subtract that square, else
    // include it in the row.
    lsb = minz(MostSigBit(aPLocation & umask & queenRank),
               MostSigBit(bPLocation & umask & queenRank));
    
    if (lsb & bPLocation)
    {
        // If the east most piece closest to the queen is the not the
        // same color, it is attackable, so include it.
        lsb <<= 1;
    }
    // Fill out the row with legal moves by doing msb - 1.
    // This will cause all the remaining bits to be 1 due to
    // msb being a 2^n integer.
    horizontalMoves = (lsb - 1);
    
    // Find the west most piece closest to queen
    msb = max(LeastSigBit(aPLocation & lmask & queenRank),
              LeastSigBit(bPLocation & lmask & queenRank));
    
    // Now either include or exclude the west most piece depending on color
    if (msb & aPLocation)
    {
        msb <<= 1;
    }
    // There can be a case where there are no pieces west of the queen. This will
    // cause msb = 0 => msb = (UInt64)-1 which will erase everything when Intersected.
    // Because of this, keep msb = 0.
    msb = msb != 0 ? msb - 1 : msb;
    
    horizontalMoves = Intersect(horizontalMoves, msb) & queenRank;
    
    // Now calculate the vertical moves similar to how the horizontal moves where
    // calculated
    lsb = minz(LeastSigBit(aPLocation & umask & queenFile),
               LeastSigBit(bPLocation & umask & queenFile));
    
    if (lsb & bPLocation)
    {
        lsb <<= 1;
    }
    verticalMoves = (lsb - 1);
    msb = max(MostSigBit(aPLocation & lmask & queenFile),
              MostSigBit(bPLocation & lmask & queenFile));
    if (msb & aPLocation)
    {
        msb <<= 1;
    }
    msb = msb != 0 ? msb - 1 : msb;
    
    verticalMoves = Intersect(verticalMoves, msb) & queenFile;
    
    aMoves |= Intersect(horizontalMoves | verticalMoves, A->Queen);
    
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
 */
UInt64 PiecesQueenMove(Pieces* A, Pieces* B)
{
    Pieces movingSide;
    UInt64 aMoves;
    
    UInt64 queens  = A->Queen;
    UInt64 square  = 0x1;
    aMoves = 0;
    
    // Special case when there are no queens on the board
    if (queens == 0)
    {
        return 0;
    }
    
    memcpy(&movingSide, A, sizeof(Pieces));
    // There may be more than one queen. Iterate the board
    // and calculate the moves for each queen.
    for (UInt64 i = 0; i < 63; i++, square <<= 1)
    {
        if (queens & square)
        {
            // Found a rook. Move all other queens to reserved,
            // so it doesn't interfere with the queens move calculation
            movingSide.Queen    = square;
            movingSide.Reserved = queens & ~square;
            aMoves |= PiecesQueenMoveEx(&movingSide, B);
            
            movingSide.Queen    = queens;
            movingSide.Reserved = 0;
        }
        
    }
    
    return aMoves;
}

/*
 Function: PiecesKingMoveFast
 Parameters:
    - Pieces* A. The moving side pieces
    - Pieces* B. The non-moving side pieces
 Return:
 UInt64. The squares where the king occupy
 Notes:
    This function does not check if the calculated
    squares causes the king to be in check. i.e.
    it's just a "fast" calculation to see the opponent
    king's attacks.
 */
UInt64 PiecesKingMoveFast(Pieces* A, Pieces* B)
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
    
    return aMoves;
}

/*
 Function: PiecesKingMove
 Parameters:
    - Pieces* A. The moving side pieces
    - Pieces* B. The non-moving side pieces
    - Bool FastSearch. When true, only the squares that the
        King can attack are calculated, and the castling moves
        and check verification are excluded.
 Return:
    UInt64. The squares where the king can go.
 Notes:

 */
UInt64 PiecesKingMove(Pieces* A, Pieces* B, bool FastSearch)
{
    UInt64 aMoves;
    UInt64 attackedSquares;
    UInt64 king;
    
    king   = A->King;
    aMoves = PiecesKingMoveFast(A, B);
    
    if (FastSearch == true)
    {
        goto End;
    }
    
    attackedSquares = PiecesGetAttackSquares(B, A);
    aMoves = Intersect(aMoves, attackedSquares);
    
    if ((A->State.Castle & KING_HAS_MOVED) == false &&
        (king & attackedSquares) == 0x0)
    {
        if ((A->State.Castle & KING_ROOK_HAS_MOVED) == false)
        {
            if (A->Color == WHITE_PIECE &&
                (attackedSquares & (f1 | g1)) == 0x0)
            {
                // White can castle short
                aMoves |= g1;
            }
            else if (A->Color == BLACK_PIECE &&
                     (attackedSquares & (f8 | g8)) == 0x0)
            {
                // Black can castle short
                aMoves |= g8;
            }
        }
        if ((A->State.Castle & QUEEN_ROOK_HAS_MOVED) == false)
        {
            if (A->Color == WHITE_PIECE &&
                (attackedSquares & (d1 | c1)) == 0x0)
            {
                // White can castle long
                aMoves |= c1;
            }
            else if (A->Color == BLACK_PIECE &&
                     (attackedSquares & (d8 | c8)) == 0x0)
            {
                // Black can castle long
                aMoves |= c8;
            }
        }
    }

End:
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
    UInt64 aSquares = 0;
    aSquares  = PiecesPawnMoveFast(A, B);
    aSquares |= PiecesKnightMove(A, B);
    aSquares |= PiecesBishopMove(A, B);
    aSquares |= PiecesRookMove(A, B);
    aSquares |= PiecesQueenMove(A, B);
    aSquares |= PiecesKingMove(A, B, true);
    
    return aSquares;
}

bool PiecesIsKingInCheck(Pieces* A, Pieces* B)
{
    UInt64 attackedSquares;
    attackedSquares = PiecesGetAttackSquares(B, A);
    
    if (attackedSquares & A->King)
    {
        return true;
    }
    
    return false;
}

PieceType PiecesMapSquareToPiece(Pieces* A, UInt64 Square)
{
    PieceType type;
    
    type = NONE;
    if (A->Pawns & Square)
    {
        type = PAWN;
    }
    else if (A->Knights & Square)
    {
        type = KNIGHT;
    }
    else if (A->Bishops & Square)
    {
        type = BISHOP;
    }
    else if (A->Rooks & Square)
    {
        type = ROOK;
    }
    else if (A->Queen & Square)
    {
        type = QUEEN;
    }
    else if (A->King & Square)
    {
        type = KING;
    }
    return type;
}
