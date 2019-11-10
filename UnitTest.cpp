#include "UnitTest.hpp"
#include "Board.hpp"

bool PawnsFirstMove()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Pawns = RANK_2;
    
    result = PiecesPawnMove(&white, &black);
    return (result == (RANK_3 | RANK_4));
}

bool PawnsFirstMoveBlocking()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Pawns = RANK_2;
    black.Pawns = 0x0F000000;
    
    result = PiecesPawnMove(&white, &black);
    return (result == (RANK_3 | (RANK_4 & 0xF0000000)));
}

bool KnightMovement()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Knights = c5;

    result = PiecesKnightMove(&white, &black);
    return (result == 0xA1100110A0000);
}

bool RookMovement()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Rooks = d4;
    result = PiecesRookMove(&white, &black);

    return (result == 0x8080808F7080808);
}

bool RookCapture()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Rooks = d4;
    white.Pawns = d1 | d6 | h4 | b4;
    black.Pawns = d2 | d7 | g4 | a4;
    
    result = PiecesRookMove(&white, &black);

    return (result == 0x874080800);
}

bool RookEmptyBoard()
{
    UInt64 result;
    Pieces white = {0};
    Pieces black = {0};
    
    result = PiecesRookMove(&white, &black);
    return (result == 0);
}

bool RookMultipleRooks()
{
    UInt64 result;
    Pieces white = {0};
    Pieces black = {0};
    
    white.Rooks  = a1 | h1;
    
    result = PiecesRookMove(&white, &black);
    return (result == 0x818181818181817E);
}

bool BishopMovement()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Bishops = d4;
    
    result = PiecesBishopMove(&white, &black);
    
    return (result == 0x8041221400142241);
}

bool BishopCapture()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Bishops = d4;
    
    white.Pawns = f6 | c3;
    black.Pawns = b6;
    
    result = PiecesBishopMove(&white, &black);
    return (result == 0x21400102040);
}

bool QueenMovement()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Queen = d4;
    
    result = PiecesQueenMove(&white, &black);

    return (result == 0x88492A1CF71C2A49);
}

bool QueenMultipleQueens()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.Queen = d4 | e4;
    
    result = PiecesQueenMove(&white, &black);
    
    return (result == 0x99DB7E3CE73C7EDB);
}

bool KingMovement()
{
    UInt64 result;
    
    Pieces white = {0};
    Pieces black = {0};
    white.King = d4;
    white.Bishops = d5;
    
    result = PiecesKingMove(&white, &black);
    return (result == 0x14141C0000);
}

bool KingIsCheckmated()
{
    Board board = {0};
    UInt64 result;
    Move move;
    
    move.StartSquare = h8;
    move.EndSquare   = g8;
    
    board.Black.King = h8;
    board.Black.Pawns= h7 | g7;
    board.Black.Color= BLACK_PIECE;
    
    board.White.Rooks= a8;
    // King should have no legal moves, as it's black-ranked mated.
    result = BoardIsMoveLegal(&board, move, BLACK_PIECE, true);
    
    return (result == false);
}

bool BoardFirstMove()
{
    Board board;
    BoardInit(&board);
    
    Move move;
    move.StartSquare = d2;
    move.EndSquare   = d4;
    
    bool isMoveLegal;
    isMoveLegal = BoardIsMoveLegal(&board, move, WHITE_PIECE, false);
    
    return isMoveLegal;
}

bool BoardPieceCollision()
{
    bool isMoveLegal;
    Board board = {0};
    
    board.White.Rooks = a1 | h1;
    
    Move move;
    move.StartSquare = a1;
    move.EndSquare   = h1;
    
    isMoveLegal = BoardIsMoveLegal(&board, move, WHITE_PIECE, true);
    
    return (isMoveLegal == false);
}

bool (*PawnTests[])() = {PawnsFirstMove, PawnsFirstMoveBlocking};
bool (*KnightTests[])() = {KnightMovement};
bool (*RookTests[])() = {RookMovement, RookCapture, RookEmptyBoard, RookMultipleRooks};
bool (*BishopTests[])() = {BishopMovement, BishopCapture};
bool (*QueenTests[])() = {QueenMovement, QueenMultipleQueens};
bool (*KingTests[])() = {KingMovement, KingIsCheckmated};
bool (*BoardTests[])() = {BoardFirstMove, BoardPieceCollision};

void TestIterator(bool (*UnitTest[])(), UInt64 Count, string Description = "")
{
    for (UInt64 i = 0; i < Count; i++)
    {
        if (UnitTest[i]() != true)
        {
            cout << RED << "Failed" << WHITE << ": Test " << i << ". " << Description << endl;
        }
        else
        {
            cout << GREEN << "Passed" << WHITE << ": Test " << i << ". " << Description << endl;
        }
    }
}

void RunAllTests()
{
    cout << endl << "===== Starting all unit tests =====" << endl;
    TestIterator(PawnTests, sizeof(PawnTests)/sizeof(void*), "Pawns ");
    TestIterator(KnightTests, sizeof(KnightTests)/sizeof(void*), "Knights ");
    TestIterator(RookTests, sizeof(RookTests)/sizeof(void*), "Rooks ");
    TestIterator(BishopTests, sizeof(BishopTests)/sizeof(void*), "Bishops ");
    TestIterator(QueenTests, sizeof(QueenTests)/sizeof(void*), "Queens ");
    TestIterator(KingTests, sizeof(KingTests)/sizeof(void*), "Kings ");
    TestIterator(BoardTests, sizeof(BoardTests)/sizeof(void*), "Board Tests ");
    cout << "========= Testing complete ========" << endl << endl;
}
