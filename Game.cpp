#include "Game.hpp"

void GamePrintInfo(string message)
{
    cout << GREEN << "Chess" << WHITE << ": " << message;
}

void GamePrintError(string message)
{
    cout << RED << "Error" << WHITE << ": " << message;
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

GameResult GameGetGameResult(Board* board, UInt64 Color)
{
    GameResult gameResult = Progressing;
    bool boardState = false;
    Pieces* attackingSide, *nonAttackingSide;
    
    if (Color == WHITE_PIECE)
    {
        attackingSide    = &board->White;
        nonAttackingSide = &board->Black;
    }
    else
    {
        attackingSide    = &board->Black;
        nonAttackingSide = &board->White;
    }

    boardState = BoardCheckmated(attackingSide, nonAttackingSide);
    if (boardState == true)
    {
        gameResult = Checkmated;
        goto End;
    }
    
    boardState = BoardIsMaterialDraw(attackingSide, nonAttackingSide);
    if (boardState == true)
    {
        gameResult = Draw;
        goto End;
    }
    
    boardState = BoardStalemated(attackingSide, nonAttackingSide);
    if (boardState == true)
    {
        gameResult = Stalemated;
        goto End;
    }
    
End:
    return gameResult;
}

void GamePlayAlone()
{
    string userInput;
    string startSquare, endSquare, winner;
    Move   move;
    UInt64 color = WHITE_PIECE;
    
    Board board;
    BoardInit(&board);
    GameResult gameResult = Progressing;
    bool isMoveLegal = false;
    
    while (gameResult == Progressing)
    {
        cout << endl;
        if (color == WHITE_PIECE)
        {
            GamePrintInfo("White's move\n");
        }
        else
        {
            GamePrintInfo("Black's move\n");
        }
        
        BoardPrint(&board);
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
            string message = "Cannot move " + startSquare + " to " + endSquare + "\n";
            GamePrintError(message);
            continue;
        }
        
        gameResult = GameGetGameResult(&board, color);
        if (color == WHITE_PIECE)
        {
            color = BLACK_PIECE;
        }
        else
        {
            color = WHITE_PIECE;
        }
        cout << endl;
    }
    
    BoardPrint(&board);
    if (color == WHITE_PIECE)
    {
        winner = "Black";
    }
    else
    {
        winner = "White";
    }
    
    switch (gameResult) {
        case Checkmated:
            winner = winner + " won by checkmate!\n";
            GamePrintInfo(winner);
            break;
        case Stalemated:
            GamePrintInfo("Draw!");
        default:
            GamePrintError("Unknown game result.\n");
            break;
    }
}

void StartMenu()
{
    cout << BLUE << " ===========================" << WHITE << endl;
    cout << BLUE << " *     Chess Beta v1.0     *" << WHITE << endl;
    cout << BLUE << " ===========================" << WHITE << endl;
    string userInput = "";
    
    while (userInput != "E")
    {
        GamePrintInfo("Play Alone (A), Exit (E): ");
        getline(cin, userInput);
        if (userInput == "A")
        {
            GamePlayAlone();
        }
    }
}
