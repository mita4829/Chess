#include "Game.hpp"

void GamePrintInfo(string message)
{
    cout << GREEN << "Chess" << WHITE << ": " << message << endl;
}

void GamePrintError(string message)
{
    cout << RED << "Error" << WHITE << ": " << message << endl;
}

bool ParseMove(Move* Move, string fromSquare, string endSquare)
{
    bool isMoveValid = true;
    
    struct Move move;
    char from[2];
    char to[2];
    
    UInt64 rank;
    UInt64 file;
    
    if (fromSquare.length() < 2 || endSquare.length() < 2 ||
        fromSquare.length() > 2 || endSquare.length() > 2)
    {
        GamePrintError("Moves are invalid. Example moves: a1, e4, d5, h8.");
        isMoveValid = false;
        goto End;
    }
    
    from[0] = fromSquare[0];
    from[1] = fromSquare[1];
    to[0] = endSquare[0];
    to[1] = endSquare[1];
    
    file = (from[0] - '0' - 49);
    if (file < 0 || file > 7)
    {
        GamePrintError("Moves are invalid. File of move must be of range [a,h].");
        isMoveValid = false;
        goto End;
    }
    
    file = 0x1 << file;
    rank = (from[1] - '0' - 1);
    if (rank < 0 || rank > 7)
    {
        GamePrintError("Moves are invalid. Rank of move must be of range [1,8].");
        isMoveValid = false;
        goto End;
    }
    
    move.StartSquare = file << (rank * 8);
    
    file = (to[0] - '0' - 49);
    if (file < 0 || file > 7)
    {
        GamePrintError("Moves are invalid. File of move must be of range [a,h].");
        isMoveValid = false;
        goto End;
    }
    
    file = 0x1 << file;
    rank = (to[1] - '0' - 1);
    if (rank < 0 || rank > 7)
    {
        GamePrintError("Moves are invalid. Rank of move must be of range [1,8].");
        isMoveValid = false;
        goto End;
    }
    
    move.EndSquare = file << (rank * 8);
    
    *Move = move;
End:
    return isMoveValid;
}

void GamePlayAlone()
{
    string userInput;
    string startSquare, endSquare;
    Move   move;
    UInt64 color = WHITE_PIECE;
    
    Board board;
    BoardInit(&board);
    bool isSideCheckmated  = false;
    bool isMoveLegal = false;
    
    while (isSideCheckmated == false)
    {
        if (color == WHITE_PIECE)
        {
            GamePrintInfo("White's move");
        }
        else
        {
            GamePrintInfo("Black's move");
        }
        
        DebugBoard(&board);
        cout << "Start Square: ";
        getline(cin, startSquare);
        cout << "End Square  : ";
        getline(cin, endSquare);
        
        isMoveLegal = ParseMove(&move, startSquare, endSquare);
        if (isMoveLegal == false)
        {
            continue;
        }
        
        isMoveLegal = BoardAttemptMove(&board, move, color, true);
        if (isMoveLegal == false)
        {
            string message = "Cannot move " + startSquare + " to " + endSquare;
            GamePrintError(message);
            continue;
        }
        
        if (color == WHITE_PIECE)
        {
            isSideCheckmated = BoardCheckmated(&board.White, &board.Black);
            color = BLACK_PIECE;
        }
        else
        {
            isSideCheckmated = BoardCheckmated(&board.Black, &board.White);
            color = WHITE_PIECE;
        }
    }
    
    DebugBoard(&board);
    if (color == BLACK_PIECE)
    {
        GamePrintInfo("White won by checkmate!");
    }
    else
    {
        GamePrintInfo("Black won by checkmate!");
    }
}
