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

bool PawnMiddleGame()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Pawns = d3 | e4 | f5;
    board.Black.Pawns = d5 | e5 | e6;
    
    result = PiecesPawnMove(&board.White, &board.Black);
    
    return (result == 0x300808000000);
}

bool PawnEdgeAttacks()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Pawns = g4 | a4 | b4 | a5;
    board.Black.Pawns = h5;
    
    result = PiecesPawnMove(&board.Black, &board.White);
    
    return (result == 0xC0000000);
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

bool KnightEdge()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Knights = a1 | e4 | h8;
    
    result = PiecesKnightMove(&board.White, &board.Black);
    
    return (result == 0x20684400462C00);
}

bool KnightZeroKnights()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Knights = 0;
    
    result = PiecesKnightMove(&board.White, &board.Black);
    
    return (result == 0x0);
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

bool RookMiddleGame()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Rooks = e1 | e4 | e8;
    board.Black.Pawns = e3 | h7 | b4;
    
    result = PiecesRookMove(&board.White, &board.Black);
    
    return (result == 0xEF101010EE1010EF);
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

bool BishopMultipleBishops()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Bishops = a1 | b2 | c3 | d4 | e5 | f6 | g7 | h8;
    
    result = PiecesBishopMove(&board.White, &board.Black);
    return (result == 0x2A158A45A251A854);
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

bool QueenMultipleCapture()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Queen = a2;
    board.Black.Queen = g2 | b1 | f7 | a7;
    
    result = PiecesQueenMove(&board.White, &board.Black);
    
    return (result == 0x21110905037E03);
}

bool KingMovement()
{
    UInt64 result;

    Board board;
    BoardZeroInit(&board);
    
    board.White.King = d4;
    board.White.Bishops = d5;
    
    result = PiecesKingMove(&board.White, &board.Black);
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
    
    result = BoardIsMoveLegal(&board, move, BLACK_PIECE, false);
    
    move.StartSquare = h7;
    move.EndSquare   = h6;
    result = result || BoardIsMoveLegal(&board, move, BLACK_PIECE, false);
    
    move.StartSquare = g7;
    move.EndSquare   = g6;
    result = result || BoardIsMoveLegal(&board, move, BLACK_PIECE, false);
    
    return (result == false);
}

bool KingCastle()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.State.Castle = 0;
    board.White.King  = e1;
    board.White.Rooks = a1 | h1;
    
    result = PiecesKingMove(&board.White, &board.Black);
    
    return (result == 0x386C);
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
    
    isMoveLegal = BoardIsMoveLegal(&board, move, WHITE_PIECE, false);
    
    return (isMoveLegal == false);
}

bool (*PawnTests[])() = {PawnsFirstMove, PawnsFirstMoveBlocking, PawnMiddleGame, PawnEdgeAttacks};
bool (*KnightTests[])() = {KnightMovement, KnightEdge, KnightZeroKnights};
bool (*RookTests[])() = {RookMovement, RookCapture, RookEmptyBoard, RookMultipleRooks, RookMiddleGame};
bool (*BishopTests[])() = {BishopMovement, BishopCapture, BishopMultipleBishops};
bool (*QueenTests[])() = {QueenMovement, QueenMultipleQueens, QueenMultipleCapture};
bool (*KingTests[])() = {KingMovement, KingIsCheckmated, KingCastle,};
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
