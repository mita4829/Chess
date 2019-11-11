#ifndef BOARD_HPP
#define BOARD_HPP

#include "Foundation.hpp"
#include "Pieces.hpp"

struct Board {
    Pieces White;
    Pieces Black;
};

void BoardInit(Board*);
void BoardZeroInit(Board* board);
bool BoardIsMoveLegal(Board*, Move, UInt64, bool);

void DebugBoard(Board* board);
bool BoardCompare(Board* A, Board* B);

#endif // BOARD_HPP
