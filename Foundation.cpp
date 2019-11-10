#include "Foundation.hpp"

UInt64 Rank[8] = {RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8};
UInt64 File[8] = {FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H};

inline UInt64 Mirror(UInt64 x)
{
    x = ((x & 0x5555555555555555) << 1) | ((x & 0xAAAAAAAAAAAAAAAA) >> 1);
    x = ((x & 0x3333333333333333) << 2) | ((x & 0xCCCCCCCCCCCCCCCC) >> 2);
    x = ((x & 0x0F0F0F0F0F0F0F0F) << 4) | ((x & 0xF0F0F0F0F0F0F0F0) >> 4);
    x = ((x & 0x00FF00FF00FF00FF) << 8) | ((x & 0xFF00FF00FF00FF00) >> 8);
    x = ((x & 0x0000FFFF0000FFFF) << 16) | ((x & 0xFFFF0000FFFF0000) >> 16);
    x = ((x & 0x00000000FFFFFFFF) << 32) | ((x & 0xFFFFFFFF00000000) >> 32);
    return x;
}

void DebugBoard(UInt64 board)
{
    board = Flip(board);
    UInt64 rank = 8;
    for (UInt64 i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            cout << endl;
            cout << BLUE << rank << " " << WHITE;
            rank--;
        }
        cout << ((board & (0x1ULL << i)) != 0) << " ";
    }
    cout << endl;
    cout << BLUE << "  A B C D E F G H" << WHITE << endl;
}

