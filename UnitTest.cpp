#include "UnitTest.hpp"
#include "Pieces.hpp"

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

bool (*PawnTests[])() = {PawnsFirstMove, PawnsFirstMoveBlocking};
bool (*KnightTests[])() = {KnightMovement};
bool (*RookTests[])() = {RookMovement, RookCapture, RookEmptyBoard};
bool (*BishopTests[])() = {BishopMovement, BishopCapture};
bool (*QueenTests[])() = {QueenMovement};
bool (*KingTests[])() = {KingMovement};

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
    cout << "========= Testing complete ========" << endl << endl;
}
