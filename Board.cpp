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

void BoardCompleteMoveEx(Pieces* A, PieceType AType, Pieces* B, PieceType BType, Move Move)
{
    switch (AType) {
        case PAWN:
            A->Pawns |= Move.EndSquare;
            A->Pawns  = Intersect(A->Pawns, Move.StartSquare);
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
            break;
        case QUEEN:
            A->Queen |= Move.EndSquare;
            A->Queen  = Intersect(A->Queen, Move.StartSquare);
            break;
        case KING:
            A->King |= Move.EndSquare;
            A->King  = Intersect(A->King, Move.StartSquare);
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
        }
        else
        {
            memcpy(&Board->Black, &movingSide, sizeof(Pieces));
            memcpy(&Board->White, &nonMovingSide, sizeof(Pieces));
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

