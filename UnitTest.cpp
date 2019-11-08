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

bool (*PawnTests[])() = {PawnsFirstMove, PawnsFirstMoveBlocking};
bool (*KnightTests[])() = {KnightMovement};
bool (*RookTests[])() = {RookMovement, RookCapture};
bool (*BishopTests[])() = {BishopMovement, BishopCapture};

void RunAllTests()
{
    cout << endl << "===== Starting all unit tests =====" << endl;
    for (int i = 0; i < sizeof(PawnTests) / sizeof(void *); i++)
    {
        if (PawnTests[i]() != true)
        {
            cout << RED << "Failed" << WHITE << ": Pawn test " << i << endl;
        }
        else
        {
            cout << GREEN << "Passed" << WHITE << ": Pawn test " << i << WHITE << endl;
        }
    }
    
    for (int i = 0; i < sizeof(KnightTests) / sizeof(void*); i++)
    {
        if (KnightTests[i]() != true)
        {
            cout << RED << "Failed" << WHITE << ": Knight test " << i << endl;
        }
        else
        {
            cout << GREEN << "Passed" << WHITE << ": Knight test " << i << WHITE << endl;
        }
    }
    
    for (int i = 0; i < sizeof(RookTests) / sizeof(void*); i++)
    {
        if (RookTests[i]() != true)
        {
            cout << RED << "Failed" << WHITE << ": Rook test " << i << endl;
        }
        else
        {
            cout << GREEN << "Passed" << WHITE << ": Rook test " << i << WHITE << endl;
        }
    }
    
    for (int i = 0; i < sizeof(BishopTests) / sizeof(void*); i++)
    {
        if (BishopTests[i]() != true)
        {
            cout << RED << "Failed" << WHITE << ": Bishop test " << i << endl;
        }
        else
        {
            cout << GREEN << "Passed" << WHITE << ": Bishop test " << i << WHITE << endl;
        }
    }
    cout << "========= Testing complete ========" << endl << endl;
}
