#ifndef BOARD_HPP
#define BOARD_HPP

#include "Foundation.hpp"
#include "Pieces.hpp"

#define WHITE_SQUARES 0x55AA55AA55AA55AA
#define BLACK_SQUARES 0xAA55AA55AA55AA55

struct Board {
    Pieces White;
    Pieces Black;
};

enum GameResult {
    Progressing,
    Checkmated,
    Stalemated,
    Draw = Stalemated,
    Unknown,
};

void BoardInit(Board*);
void BoardZeroInit(Board* board);
bool BoardAttemptMove(Board*, Move, UInt64, bool);
bool BoardCheckmated(Pieces* A, Pieces* B);
bool BoardStalemated(Pieces* A, Pieces* B);
bool BoardIsMaterialDraw(Pieces* A, Pieces* B);

void BoardPrint(Board* board);
bool BoardCompare(Board* A, Board* B);

#endif // BOARD_HPP
