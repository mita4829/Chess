#include "Board.hpp"

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

void BoardZeroInit(Board* board)
{
    memset(board, 0, sizeof(Board));
    
    board->White.Color   = WHITE_PIECE;
    board->White.State.Castle  = 0xFF;
    
    board->Black.Color   = BLACK_PIECE;
    board->Black.State.Castle  = 0xFF;
}

bool BoardIsMoveLegalByPieceEx(Pieces* A, Pieces* B, PieceType PieceType, Move Move)
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

bool BoardIsMoveLegal(Board* Board, Move Move, UInt64 Color, bool ReturnPosition = false)
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
    isMoveLegal = BoardIsMoveLegalByPieceEx(&movingSide, &nonMovingSide, movingPieceType, Move);
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

bool BoardCompare(Board* A, Board* B)
{
    return BoardComparePieces(&A->White, &B->White) &&
           BoardComparePieces(&A->Black, &B->Black);
}

