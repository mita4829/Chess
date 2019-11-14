#include "Board.hpp"

/*
 Function: BoardInit
 Parameters:
    - Board* Board
 Return:
 Notes:
    This function sets up the board for a new game
 */
void BoardInit(Board* board)
{
    memset(board, 0, sizeof(Board));
    
    board->White.Pawns   = RANK_2;
    board->White.Rooks   = a1 | h1;
    board->White.Knights = b1 | g1;
    board->White.Bishops = c1 | f1;
    board->White.Queen   = d1;
    board->White.King    = e1;
    board->White.Color   = WHITE_PIECE;
    
    board->Black.Pawns   = RANK_7;
    board->Black.Rooks   = a8 | h8;
    board->Black.Knights = b8 | g8;
    board->Black.Bishops = c8 | f8;
    board->Black.Queen   = d8;
    board->Black.King    = e8;
    board->Black.Color   = BLACK_PIECE;
}

/*
 Function: BoardZeroInit
 Parameters:
    - Board* Board
 Return:
 Notes:
    This function sets up the board with no pieces,
    and all castling flags are off.
 */
void BoardZeroInit(Board* board)
{
    memset(board, 0, sizeof(Board));
    
    board->White.Color   = WHITE_PIECE;
    board->White.State.Castle  = (KING_HAS_MOVED | KING_ROOK_HAS_MOVED | QUEEN_ROOK_HAS_MOVED);
    
    board->Black.Color   = BLACK_PIECE;
    board->Black.State.Castle  = (KING_HAS_MOVED | KING_ROOK_HAS_MOVED | QUEEN_ROOK_HAS_MOVED);
}


/*
 Function: BoardZeroInit
 Parameters:
    - Board* Board
    - Pieces* A
    - Pieces* B
 Return:
 Notes:
    This function sets up the board given both side's pieces
 */
void BoardInitWithPieces(Board* Board, Pieces* A, Pieces* B)
{
    if (A->Color == WHITE_PIECE)
    {
        memcpy(&Board->White, A, sizeof(Pieces));
        memcpy(&Board->Black, B, sizeof(Pieces));
    }
    else
    {
        memcpy(&Board->Black, A, sizeof(Pieces));
        memcpy(&Board->White, B, sizeof(Pieces));
    }
}

/*
 Function: BoardCheckMoveIsLegalByPieceEx
 Parameters:
    - Pieces* A. The moving side
    - Pieces* B. The non-moving side
    - PieceType PieceType. The kind of piece being moved
    - Move Move. Contains the start and end squares for the moving piece
 Return:
    True if the move was successful, false otherwise.
 Notes:
    This function checks if the move is legal.
 */
bool BoardCheckMoveIsLegalByPieceEx(Pieces* A, Pieces* B, PieceType PieceType, Move Move)
{
    bool   isMoveLegal;
    Pieces tmpPieces;
    
    isMoveLegal = false;
    memcpy(&tmpPieces, A, sizeof(Pieces));
    
    switch (PieceType) {
        case PAWN:
            tmpPieces.Reserved2 = tmpPieces.Pawns;
            tmpPieces.Pawns &= Move.StartSquare;
            isMoveLegal = (PiecesPawnMove(&tmpPieces, B) & Move.EndSquare);
            break;
        case KNIGHT:
            tmpPieces.Reserved2 = tmpPieces.Knights;
            tmpPieces.Knights &= Move.StartSquare;
            isMoveLegal = (PiecesKnightMove(&tmpPieces, B) & Move.EndSquare);
            break;
        case BISHOP:
            tmpPieces.Reserved2 = tmpPieces.Bishops;
            tmpPieces.Bishops &= Move.StartSquare;
            isMoveLegal = (PiecesBishopMove(&tmpPieces, B) & Move.EndSquare);
            break;
        case ROOK:
            tmpPieces.Reserved2 = tmpPieces.Rooks;
            tmpPieces.Rooks &= Move.StartSquare;
            isMoveLegal = (PiecesRookMove(&tmpPieces, B) & Move.EndSquare);
            break;
        case QUEEN:
            tmpPieces.Reserved2 = tmpPieces.Queen;
            tmpPieces.Queen &= Move.StartSquare;
            isMoveLegal = (PiecesQueenMove(&tmpPieces, B) & Move.EndSquare);
            break;
        case KING:
            tmpPieces.King &= Move.StartSquare;
            isMoveLegal = (PiecesKingMove(&tmpPieces, B) & Move.EndSquare);
            break;
        default:
            break;
    }
    
    return isMoveLegal;
}

/*
 Function: BoardPromotePawnEx
 Parameters:
    - Pieces* A. Promote side
    - Move Move. Move object info about the pawn
 Return:
 Notes:
    This function gets the promotion choice via
    stdin. Open-source users should modify their
    input choice here.
 */
void BoardPromotePawnEx(Pieces* A, Move Move)
{
    string userInput;
    bool   userInputIsValid = false;
    
    while (userInputIsValid == false)
    {
        cout << "Pawn promotion" << endl;
        cout << "Queen (Q), Rook (R), Bishop (B), Knight (N): ";
        getline(cin, userInput);
        userInputIsValid = true;
        
        if (userInput == "Q")
        {
            A->Queen |= Move.EndSquare;
        }
        else if (userInput == "R")
        {
            A->Rooks |= Move.EndSquare;
        }
        else if (userInput == "B")
        {
            A->Bishops |= Move.EndSquare;
        }
        else if (userInput == "N")
        {
            A->Knights |= Move.EndSquare;
        }
        else
        {
            cout << "Invalid promotion choice." << endl;
            userInputIsValid = false;
        }
        A->Pawns  = Intersect(A->Pawns, Move.EndSquare);
    }
}

/*
 Function: BoardCompleteMoveEx
 Parameters:
    - Pieces* A. The moving side
    - PieceType AType. The kind of piece being moved by A
    - Pieces* B. The non-moving side
    - PieceType BType. The kind of piece not being moved by B
    - Move Move. Contains the start and end squares for A's moving piece
 Return:
 Notes:
    This function updates the bit boards and also performs en passant,
    castling, and promotions when needed.
 */
void BoardCompleteMoveEx(Pieces* A, PieceType AType, Pieces* B, PieceType BType, Move Move)
{
    bool isEnPassantMove = false;
    
    switch (AType) {
        case PAWN:
            A->Pawns |= Move.EndSquare;
            A->Pawns  = Intersect(A->Pawns, Move.StartSquare);
            // Check if move is an en passant move or promotion
            if (A->Color == WHITE_PIECE)
            {
                if (B->State.LastMovedPiece == PAWN &&
                    (B->State.LastMove.EndSquare & RANK_5) &&
                    (B->State.LastMove.StartSquare & RANK_7) &&
                    ((Move.EndSquare >> 8) & B->State.LastMove.EndSquare))
                {
                    isEnPassantMove = true;
                    break;
                }
                
                if (Move.EndSquare & RANK_8)
                {
                    BoardPromotePawnEx(A, Move);
                    break;
                }
            }
            else
            {
                if (B->State.LastMovedPiece == PAWN &&
                    (B->State.LastMove.EndSquare & RANK_4) &&
                    (B->State.LastMove.StartSquare & RANK_2) &&
                    ((Move.EndSquare << 8) & B->State.LastMove.EndSquare))
                {
                    isEnPassantMove = true;
                    break;
                }
                
                if (Move.EndSquare & RANK_1)
                {
                    BoardPromotePawnEx(A, Move);
                    break;
                }
            }
            break;
        case KNIGHT:
            A->Knights |= Move.EndSquare;
            A->Knights  = Intersect(A->Knights, Move.StartSquare);
            break;
        case BISHOP:
            A->Bishops |= Move.EndSquare;
            A->Bishops  = Intersect(A->Bishops, Move.StartSquare);
            break;
        case ROOK:
            A->Rooks |= Move.EndSquare;
            A->Rooks  = Intersect(A->Rooks, Move.StartSquare);
            if (A->Color == WHITE_PIECE)
            {
                if (Move.StartSquare == a1)
                {
                    A->State.Castle |= QUEEN_ROOK_HAS_MOVED;
                }
                else if (Move.StartSquare == h1)
                {
                    A->State.Castle |= KING_ROOK_HAS_MOVED;
                }
            }
            else
            {
                if (Move.StartSquare == a8)
                {
                    A->State.Castle |= QUEEN_ROOK_HAS_MOVED;
                }
                else if (Move.StartSquare == h8)
                {
                    A->State.Castle |= KING_ROOK_HAS_MOVED;
                }
            }
            break;
        case QUEEN:
            A->Queen |= Move.EndSquare;
            A->Queen  = Intersect(A->Queen, Move.StartSquare);
            break;
        case KING:
            A->King |= Move.EndSquare;
            A->King  = Intersect(A->King, Move.StartSquare);
            // Castle logic
            if ((A->State.Castle & KING_HAS_MOVED) == 0x0)
            {
                if (A->Color == WHITE_PIECE)
                {
                    if (Move.EndSquare == g1 &&
                        (A->State.Castle & KING_ROOK_HAS_MOVED) == 0x0)
                    {
                        // White king is castling short
                        A->State.Castle |= KING_ROOK_HAS_MOVED;
                        A->Rooks  = Intersect(A->Rooks, h1);
                        A->Rooks |= f1;
                    }
                    else if (Move.EndSquare == c1 &&
                             (A->State.Castle & QUEEN_ROOK_HAS_MOVED) == 0x0)
                    {
                        // White king is castling long
                        A->State.Castle |= QUEEN_ROOK_HAS_MOVED;
                        A->Rooks  = Intersect(A->Rooks, a1);
                        A->Rooks |= d1;
                    }
                }
                else
                {
                    if (Move.EndSquare == g8 &&
                        (A->State.Castle & KING_ROOK_HAS_MOVED) == 0x0)
                    {
                        // Black king is castling short
                        A->State.Castle |= KING_ROOK_HAS_MOVED;
                        A->Rooks  = Intersect(A->Rooks, h8);
                        A->Rooks |= f8;
                    }
                    else if (Move.EndSquare == c8 &&
                             (A->State.Castle & QUEEN_ROOK_HAS_MOVED) == 0x0)
                    {
                        // White king is castling long
                        A->State.Castle |= QUEEN_ROOK_HAS_MOVED;
                        A->Rooks  = Intersect(A->Rooks, a8);
                        A->Rooks |= d8;
                    }
                }
            }
            A->State.Castle |= KING_HAS_MOVED;
            break;
        default:
            break;
    }
    
    switch (BType) {
        case PAWN:
            B->Pawns &= ~Move.EndSquare;
            break;
        case KNIGHT:
            B->Knights &= ~Move.EndSquare;
            break;
        case BISHOP:
            B->Bishops &= ~Move.EndSquare;
            break;
        case ROOK:
            B->Rooks &= ~Move.EndSquare;
            break;
        case QUEEN:
            B->Queen &= ~Move.EndSquare;
            break;
        case KING:
            B->King &= ~Move.EndSquare;
            break;
        case NONE:
            // If the last move was an en passant move,
            // the captured pawn must be removed.
            if (isEnPassantMove == true)
            {
                if (A->Color == WHITE_PIECE)
                {
                    B->Pawns &= ~(Move.EndSquare >> 8);
                }
                else
                {
                    B->Pawns &= ~(Move.EndSquare << 8);
                }
            }
            break;
        default:
            break;
    }
}

/*
 Function: BoardAttemptMove
 Parameters:
    - Board* Board. The current chess board
    - Move Move. The move to be attempted
    - UInt64 Color. The color of the side making the move.
    - bool ReturnPosition. When true, Board will be updated on exit.
 Return:
    bool - If the attempted move is legal, this function returns true,
           false otherwise.
 Notes:
    If ReturnPosition is true, the board object passed in will be updated
    with the legal move on exit.
 */
bool BoardAttemptMove(Board* Board, Move Move, UInt64 Color, bool ReturnPosition = false)
{
    bool      isMoveLegal;
    UInt64    attackedSquares;
    PieceType movingPieceType, nonMovingPieceType;
    Pieces    movingSide, nonMovingSide;
    
    isMoveLegal = true;
    
    if (Color == WHITE_PIECE)
    {
        memcpy(&movingSide, &Board->White, sizeof(Pieces));
        memcpy(&nonMovingSide, &Board->Black, sizeof(Pieces));
    }
    else if (Color == BLACK_PIECE)
    {
        memcpy(&movingSide, &Board->Black, sizeof(Pieces));
        memcpy(&nonMovingSide, &Board->White, sizeof(Pieces));
    }
    else
    {
        // Unknown color
        isMoveLegal = false;
        goto End;
    }
    

    movingPieceType = PiecesMapSquareToPiece(&movingSide, Move.StartSquare);
    if (movingPieceType == NONE)
    {
        // Trying to move a piece on a square that color does not have.
        isMoveLegal = false;
        goto End;
    }


    // A corresponding piece was found. Now find out if it's a legal move
    // for that piece.
    isMoveLegal = BoardCheckMoveIsLegalByPieceEx(&movingSide, &nonMovingSide, movingPieceType, Move);
    if (isMoveLegal != true)
    {
        goto End;
    }
    
    
    // Make the move
    // First find out if the ending square is occupied with the opposite color
    nonMovingPieceType = PiecesMapSquareToPiece(&nonMovingSide, Move.EndSquare);
    
    // Flip the bits to represent the move
    BoardCompleteMoveEx(&movingSide, movingPieceType, &nonMovingSide, nonMovingPieceType, Move);
    
    // Now validate if the moving side is under check.
    // Get the attacked squares of the opposite color
    attackedSquares = PiecesGetAttackSquares(&nonMovingSide, &movingSide);
    if (movingSide.King & attackedSquares)
    {
        // King is in check, previous move is not legal
        isMoveLegal = false;
        goto End;
    }
    
    // The resulting move is legal
    isMoveLegal = true;
    
    // Update the Board if the caller requested
    if (ReturnPosition == true)
    {
        if (Color == WHITE_PIECE)
        {
            memcpy(&Board->White, &movingSide, sizeof(Pieces));
            memcpy(&Board->Black, &nonMovingSide, sizeof(Pieces));
            // Update the last move to be this move
            Board->White.State.LastMove       = Move;
            Board->White.State.LastMovedPiece = movingPieceType;
            memset(&Board->Black.State.LastMove, 0, sizeof(Move));
            Board->Black.State.LastMovedPiece = NONE;
        }
        else
        {
            memcpy(&Board->Black, &movingSide, sizeof(Pieces));
            memcpy(&Board->White, &nonMovingSide, sizeof(Pieces));
            // Update the last move to be this move
            Board->Black.State.LastMove       = Move;
            Board->Black.State.LastMovedPiece = movingPieceType;
            memset(&Board->White.State.LastMove, 0, sizeof(Move));
            Board->White.State.LastMovedPiece = NONE;
        }
    }
    
End:
    return isMoveLegal;
}

/*
 Function: BoardFindNextPieceEx
 Parameters:
    - Pieces* A. The side we're looking at.
    - PieceType PieceType. The kind of piece
    - UInt64 StartSquare. Starting square on the board.
 Return:
    UInt64 - The next square which has a piece of PieceType. 0 otherwise if there's no more
 Note:
     This is a helper function for BoardPieceTypeHasLegalMovesEx.
     It returns, the next piece on the board that's of type
     PieceType starting from StartSquare.
 */

inline UInt64 BoardFindNextMoveEx(UInt64 LegalMoves, UInt64 StartSquare)
{
    UInt64 nextMoveLocation;
    UInt64 aMoves;
    UInt64 mask;
    
    if (StartSquare == NO_SQUARE)
    {
        nextMoveLocation = 0;
        goto End;
    }
    
    aMoves  = StartSquare;
    mask    = ~(aMoves - 1);
    aMoves  = mask & LegalMoves;
    aMoves  = LeastSigBit(aMoves);
    
    nextMoveLocation = aMoves;
    
End:
    return nextMoveLocation;
}

/*
 Function: BoardFindNextPieceEx
 Parameters:
    - Pieces* A. The side we're looking at.
    - PieceType PieceType. The kind of piece
    - UInt64 StartSquare. Starting square on the board.
 Return:
    UInt64 - The next square which has a piece of PieceType. 0 otherwise if there's no more
 Note:
    This is a helper function for BoardPieceTypeHasLegalMovesEx.
    It returns, the next piece on the board that's of type
    PieceType starting from StartSquare.
 */
inline UInt64 BoardFindNextPieceEx(Pieces* A, PieceType PieceType, UInt64 StartSquare)
{
    UInt64 nextPiece;
    UInt64 aPiece;
    UInt64 mask;
    
    if (StartSquare == NO_SQUARE)
    {
        nextPiece = 0;
        goto End;
    }
    
    nextPiece = NO_SQUARE;
    
    switch (PieceType) {
        case PAWN:
            aPiece = A->Pawns;
            break;
        case KNIGHT:
            aPiece = A->Knights;
            break;
        case BISHOP:
            aPiece = A->Bishops;
            break;
        case ROOK:
            aPiece = A->Rooks;
            break;
        case QUEEN:
            aPiece = A->Queen;
            break;
        case KING:
            aPiece = A->King;
            break;
        default:
            aPiece = 0x0;
            break;
    }
    
    mask    = (StartSquare - 1);
    aPiece &= ~mask;
    aPiece  = LeastSigBit(aPiece);
    
    nextPiece = aPiece;
    
End:
    return nextPiece;
}

/*
 Function: BoardGetPieceMoveCallbackEx
 Parameters:
    - PieceType PieceType. The kind of piece being moved.
 Return:
    (Piece*, Piece*) -> UInt64. The associated Pieces*Move callback
        for the piece.
 Note:
 */
UInt64 (*BoardGetPieceMoveCallbackEx(PieceType PieceType))(Pieces*, Pieces*)
{
    switch (PieceType) {
        case PAWN:
            return PiecesPawnMove;
        case KNIGHT:
            return PiecesKnightMove;
        case BISHOP:
            return PiecesBishopMove;
        case ROOK:
            return PiecesRookMove;
        case QUEEN:
            return PiecesQueenMove;
        case KING:
            return PiecesKingMove;
        default:
            break;
    }
    
    return nullptr;
}

/*
 Function: BoardPieceHasLegalMovesEx
 Parameters:
    - Pieces* A. Pieces for attacking side
    - Pieces* B. Pieces for attacked side
    - UInt64 LegalMoves. The legal moves for the piece at PieceLocation
    - UInt64 PieceLocation. The square the piece is located on the board
    - PieceType MovingSidePieceType. The kind of piece being moved.
 Return:
    bool - True if A has checkmated B, false
           otherwise.
 Note:
    This is a helper function for BoardCheckmated. This function
    iterates through all the moves for the piece at PieceLocation.
    It then validates if this moves stop B from being in check.
 */
bool BoardPieceHasLegalMovesEx(Pieces* A,
                               Pieces* B,
                               UInt64 LegalMoves,
                               UInt64 PieceLocation,
                               PieceType MovingSidePieceType)
{
    Move   move;
    bool   bIsCheckmated;
    UInt64 endSquare;
    UInt64 attackedSquares;
    PieceType nonMovingPieceType;
    Pieces nonMovingSide, movingSide;
    
    bIsCheckmated = true;
    endSquare = a1;
    move.StartSquare = PieceLocation;
    
    memcpy(&movingSide, B, sizeof(Pieces));
    memcpy(&nonMovingSide, A, sizeof(Pieces));
    
    endSquare = BoardFindNextMoveEx(LegalMoves, endSquare);
    
    while (endSquare != NO_SQUARE)
    {
        move.EndSquare = endSquare;
        nonMovingPieceType = PiecesMapSquareToPiece(A, endSquare);
        
        BoardCompleteMoveEx(&movingSide, MovingSidePieceType, &nonMovingSide, nonMovingPieceType, move);
        
        // See if the attacked king is still in check
        if (PiecesIsKingInCheck(&movingSide, &nonMovingSide) == false)
        {
            bIsCheckmated = false;
            goto End;
        }
        
        // Increment to the next square to find the next legal move
        endSquare <<= 1;
        // Find the next legal move and see if that move leads
        // B king to no longer be in check
        endSquare = BoardFindNextMoveEx(LegalMoves, endSquare);
    }
End:
    return bIsCheckmated;
}

/*
 Function: BoardPieceTypeHasLegalMovesEx
 Parameters:
    - Pieces* A. Pieces for attacking side
    - Pieces* B. Pieces for attacked side
    - PieceType. The kind of piece B will move to see if legal
 Return:
    bool - True if A has checkmated B, false
           otherwise.
 Note:
    This is a helper function for BoardCheckmated. This function
    iterates through all the pieces for a given PieceType and then
    iterates through each of their moves to see if legal.
 */
bool BoardPieceTypeHasLegalMovesEx(Pieces* A, Pieces* B, PieceType PieceType)
{
    UInt64 startSquare = a1;
    UInt64 pieceLocation, legalMoves;
    UInt64 (*PieceMoveCallback)(Pieces*, Pieces*);
    bool   isBCheckmated;
    
    isBCheckmated = true;
    
    // While there are still pieces of this piece type
    // get that piece location and the routine for finding its moves.
    pieceLocation     = BoardFindNextPieceEx(B, PieceType, startSquare);
    PieceMoveCallback = BoardGetPieceMoveCallbackEx(PieceType);
    
    // Get the legal moves for this piece type. If there are none, return.
    if ((legalMoves = PieceMoveCallback(B, A)) == 0)
    {
        goto End;
    }
    
    while (pieceLocation != NO_SQUARE)
    {
        isBCheckmated = BoardPieceHasLegalMovesEx(A, B, legalMoves, pieceLocation, PieceType);
        if (isBCheckmated == false)
        {
            goto End;
        }
        
        startSquare   = (pieceLocation << 1);
        pieceLocation = BoardFindNextPieceEx(B, PieceType, startSquare);
    }
    
End:
    return isBCheckmated;
}

/*
 Function: BoardCheckmated
 Parameters:
    - Pieces* A. Pieces for attacking side
    - Pieces* B. Pieces for attacked side
 Return:
    bool - True if A has checkmated B, false
           otherwise.
 Note:
    This function is fairly expensive.
    Avoid calling it frequently.
 */
bool BoardCheckmated(Pieces* A, Pieces* B)
{
    bool bIsCheckmated = true;
    
    // Check if B is being checked
    if (PiecesIsKingInCheck(B, A) == false)
    {
        // B is not checked. Exit
        bIsCheckmated = false;
        goto End;
    }
    
    // Iterate through all the moves B has,
    // and re-evaluate the board. And if B
    // has no legal moves to escape check,
    // B has been checkmated.
    for (UInt64 pieceType = KING; pieceType > NONE; pieceType--)
    {
        bIsCheckmated = BoardPieceTypeHasLegalMovesEx(A, B, (PieceType)pieceType);
        if (bIsCheckmated == false)
        {
            goto End;
        }
    }
    
End:
    return bIsCheckmated;
}


/*
 Function: DebugBoard
 Parameters:
    - Pieces* A. Board obj
 Return:
 Notes:
    This function prints out the board to stdio
 */
void DebugBoard(Board* board)
{
    Pieces  white, black;
    bool firstPrint = true;
    
    white = board->White;
    black = board->Black;
    
    UInt64 rank = 8;
    UInt64 rankIndex = 0x100000000000000;
    UInt64 square;
    for (UInt64 i = 0; i < 8; i++)
    {
        cout << endl;
        cout << BLUE << rank << " " << WHITE;
        rank--;
        
        square = rankIndex;
        for (UInt64 j = 0; j < 8; j++, square<<=1)
        {
            //DebugPrint(str(square));
            if (white.Pawns & square)
            {
                cout << "P ";
            }
            else if (white.Knights & square)
            {
                cout << "N ";
            }
            else if (white.Bishops & square)
            {
                cout << "B ";
            }
            else if (white.Rooks & square)
            {
                cout << "R ";
            }
            else if (white.Queen & square)
            {
                cout << "Q ";
            }
            else if (white.King & square)
            {
                cout << "K ";
            }
            else if (black.Pawns & square)
            {
                cout << "p ";
            }
            else if (black.Knights & square)
            {
                cout << "n ";
            }
            else if (black.Bishops & square)
            {
                cout << "b ";
            }
            else if (black.Rooks & square)
            {
                cout << "r ";
            }
            else if (black.Queen & square)
            {
                cout << "q ";
            }
            else if (black.King & square)
            {
                cout << "k ";
            }
            else
            {
                cout << ". ";
            }
        }
        rankIndex >>= 8;
    }
    cout << endl;
    cout << BLUE << "  A B C D E F G H" << WHITE << endl;
}

/*
 Function: BoardComparePieces
 Parameters:
    - Pieces* A. First Board
    - Pieces* B. Second Board
 Return:
    bool - True if both piece objs are equal, false
           otherwise.
 Notes:
    BoardComparePieces does not compare Reserved bit values
 */
bool BoardComparePieces(Pieces* A, Pieces* B)
{
    return ((A->Pawns == B->Pawns) &&
            (A->Knights == B->Knights) &&
            (A->Bishops == B->Bishops) &&
            (A->Rooks == B->Rooks) &&
            (A->Queen == B->Queen) &&
            (A->King == B->King) &&
            (A->State.Castle == B->State.Castle));
}


/*
 Function: BoardCompare
 Parameters:
    - Board* A. First Board
    - Board* B. Second Board
 Return:
    bool - True if both boards are equal, false
           otherwise.
 Notes:
    BoardCompare does not compare Reserved bit values
 */
bool BoardCompare(Board* A, Board* B)
{
    return BoardComparePieces(&A->White, &B->White) &&
    BoardComparePieces(&A->Black, &B->Black);
}

