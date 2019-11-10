#ifndef BOARD_HPP
#define BOARD_HPP

#include "Foundation.hpp"
#include "Pieces.hpp"

struct Board {
    Pieces White;
    Pieces Black;
    PlayingState State;
};

struct Move {
    UInt64 StartSquare;
    UInt64 EndSquare;
};

void BoardInit(Board*);
bool BoardIsMoveLegal(Board*, Move, UInt64, bool);
void DebugBoard(Board* board);

#endif // BOARD_HPP
