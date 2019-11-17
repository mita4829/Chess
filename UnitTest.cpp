#include "UnitTest.hpp"
#include "Board.hpp"

#define abs(X) ((X) < 0 ? -(X) : (X))

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

bool PawnEnPassantWhite()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Pawns = e5;
    board.Black.Pawns = d5;
    board.Black.State.LastMove = {d7, d5};
    board.Black.State.LastMovedPiece = PAWN;
    
    result = PiecesPawnMove(&board.White, &board.Black);
    
    return (result == 0x180000000000);
}

bool PawnEnPassantBlack()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);
    
    board.White.Pawns = f4;
    board.Black.Pawns = e4;
    board.White.State.LastMove = {f2, f4};
    board.White.State.LastMovedPiece = PAWN;
    
    result = PiecesPawnMove(&board.Black, &board.White);
    
    return (result == 0x300000);
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

bool RookHorizontalAttack()
{
    Board board;
    UInt64 result;
    BoardZeroInit(&board);

    board.Black.Rooks = f7;
    board.Black.Pawns = d7 | c7 | b7;
    
    result = PiecesRookMove(&board.Black, &board.White);

    return (result == 0x20D0202020202020);
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
    
    result = BoardAttemptMove(&board, move, BLACK_PIECE, false);
    
    move.StartSquare = h7;
    move.EndSquare   = h6;
    result = result || BoardAttemptMove(&board, move, BLACK_PIECE, false);
    
    move.StartSquare = g7;
    move.EndSquare   = g6;
    result = result || BoardAttemptMove(&board, move, BLACK_PIECE, false);
    
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
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, false);
    
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
    
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, false);
    
    return (isMoveLegal == false);
}

bool BoardSimplePawnPush()
{
    Board board;
    bool isMoveLegal;
    BoardInit(&board);
    
    Move move;
    move.StartSquare = d2;
    move.EndSquare   = d4;
    
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = g8;
    move.EndSquare = f6;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = d4;
    move.EndSquare   = d5;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = e7;
    move.EndSquare   = e5;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = d5;
    move.EndSquare   = e6;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = f8;
    move.EndSquare   = b4;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = b1;
    move.EndSquare   = c3;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = e8;
    move.EndSquare   = g8;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = c1;
    move.EndSquare   = f4;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = d7;
    move.EndSquare   = e6;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = d1;
    move.EndSquare   = d2;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = b8;
    move.EndSquare   = c6;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = e1;
    move.EndSquare   = c1;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    return isMoveLegal;
}

bool BoardCheckmateIterator()
{
    Board board;
    bool  isCheckmated;
    BoardZeroInit(&board);
    
    
    board.White.King  = g1;
    board.White.Rooks = a8;
    board.Black.King  = g8;
    board.Black.Pawns = f7 | g7 | h7;
    
    isCheckmated = BoardCheckmated(&board.White, &board.Black);
    
    return (isCheckmated == true);
}

bool BoardKnightCheckmate()
{
    Board board;
    bool  isCheckmated;
    BoardZeroInit(&board);
    
    
    board.White.King  = g1;
    board.White.Knights = f7;
    board.Black.King  = h8;
    board.Black.Rooks = g8;
    board.Black.Pawns = g7 | h7;
    
    isCheckmated = BoardCheckmated(&board.White, &board.Black);
    
    return (isCheckmated == true);
}

bool BoardFoolsMate()
{
    Board board;
    bool  isCheckmated;
    bool  isMoveLegal;
    BoardInit(&board);
    
    
    Move move;
    move.StartSquare = e2;
    move.EndSquare   = e4;
    
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = f7;
    move.EndSquare = f6;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);

    move.StartSquare = d2;
    move.EndSquare = d4;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = g7;
    move.EndSquare = g5;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = d1;
    move.EndSquare = h5;
    isMoveLegal = isMoveLegal && BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    isCheckmated = BoardCheckmated(&board.White, &board.Black);
    
    return (isCheckmated == true);
}

bool BoardPromotedQueen()
{
    Board board;
    bool  isCheckmated;
    bool  isMoveLegal;
    BoardInit(&board);
    
    
    Move move;
    move.StartSquare = e2;
    move.EndSquare   = e4;
    
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = f7;
    move.EndSquare = f5;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = e4;
    move.EndSquare = f5;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = g7;
    move.EndSquare = g6;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = f5;
    move.EndSquare = g6;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = b8;
    move.EndSquare = c6;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = g6;
    move.EndSquare = g7;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = c6;
    move.EndSquare = b4;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = g7;
    move.EndSquare = h8;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    move.StartSquare = d7;
    move.EndSquare = d6;
    isMoveLegal = BoardAttemptMove(&board, move, BLACK_PIECE, true);
    
    move.StartSquare = h8;
    move.EndSquare = c3;
    isMoveLegal = BoardAttemptMove(&board, move, WHITE_PIECE, true);
    
    BoardPrint(&board);
    return (isMoveLegal == true);
}

bool BoardStalemate()
{
    Board board;
    bool  isStalemated;
    BoardZeroInit(&board);
    
    
    board.White.King  = e6;
    board.Black.King  = e8;
    board.White.Pawns = e7;
    
    isStalemated = BoardStalemated(&board.White, &board.Black);
    
    return (isStalemated == true);
}

bool BoardNotStalemate()
{
    Board board;
    bool  isStalemated;
    BoardZeroInit(&board);
    
    
    board.White.King  = f1;
    board.Black.King  = h8;
    board.White.Rooks = g1 | b6;
    
    isStalemated = BoardStalemated(&board.White, &board.Black);
    
    return (isStalemated == false);
}

bool BoardMateralDraw()
{
    Board board;
    BoardZeroInit(&board);
    bool isDraw;
    
    board.White.King = e4;
    board.Black.King = c5;
    board.White.Knights = a1;
    board.Black.Bishops = c1 | d2;
    
    isDraw = BoardIsMaterialDraw(&board.White, &board.Black);
    
    return (isDraw == true);
}

bool PerfSimpleGamePerf()
{
    clock_t start;
    double  duration;
    bool    isPerfGood;
    
    start = clock();
    BoardSimplePawnPush();
    duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    
    isPerfGood = ((duration - (double)0.000040) <= (double)0.000001);
    
    if (isPerfGood == false)
    {
        cout << YELLOW << "    Baseline" << WHITE << ": " << "40μs" << endl;
        cout << YELLOW << "    Result  " << WHITE << ": " << duration * 1000000 << "μs" << endl;
        cout << YELLOW << "    Delta   " << WHITE << ": +" << (duration - (double)0.000040) * 1000000 << "μs" << endl;
    }
    
    return isPerfGood;
}

bool PerfCheckmateIterator()
{
    clock_t start;
    double  duration;
    bool    isPerfGood;
    
    start = clock();
    BoardCheckmateIterator();
    duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    
    isPerfGood = ((duration - (double)0.000020) <= (double)0.000001);
    
    if (isPerfGood == false)
    {
        cout << YELLOW << "    Baseline" << WHITE << ": " << "20μs" << endl;
        cout << YELLOW << "    Result  " << WHITE << ": " << duration * 1000000 << "μs" << endl;
        cout << YELLOW << "    Delta   " << WHITE << ": +" << (duration - (double)0.000020) * 1000000 << "μs" << endl;
    }

    
    return isPerfGood;
}

bool PerfCheckmateFoolsMate()
{
    clock_t start;
    double  duration;
    bool    isPerfGood;
    
    start = clock();
    BoardFoolsMate();
    duration = (clock() - start) / (double)CLOCKS_PER_SEC;
    
    isPerfGood = ((duration - (double)0.000280) <= (double)0.000001);
    
    if (isPerfGood == false)
    {
        cout << YELLOW << "    Baseline" << WHITE << ": " << "280μs" << endl;
        cout << YELLOW << "    Result  " << WHITE << ": " << duration * 1000000 << "μs" << endl;
        cout << YELLOW << "    Delta   " << WHITE << ": +" << (duration - (double)0.000280) * 1000000 << "μs" << endl;
    }
    
    
    return isPerfGood;
}

bool (*PawnTests[])() = {PawnsFirstMove, PawnsFirstMoveBlocking, PawnMiddleGame, PawnEdgeAttacks, PawnEnPassantWhite, PawnEnPassantBlack};
bool (*KnightTests[])() = {KnightMovement, KnightEdge, KnightZeroKnights};
bool (*RookTests[])() = {RookMovement, RookCapture, RookEmptyBoard, RookMultipleRooks, RookMiddleGame, RookHorizontalAttack};
bool (*BishopTests[])() = {BishopMovement, BishopCapture, BishopMultipleBishops};
bool (*QueenTests[])() = {QueenMovement, QueenMultipleQueens, QueenMultipleCapture};
bool (*KingTests[])() = {KingMovement, KingIsCheckmated, KingCastle,};
bool (*BoardTests[])() = {BoardFirstMove, BoardPieceCollision, BoardSimplePawnPush, BoardCheckmateIterator, BoardKnightCheckmate, BoardFoolsMate, /*BoardPromotedQueen*/ BoardStalemate, BoardNotStalemate, BoardMateralDraw};
bool (*PerfTests[])() = {PerfSimpleGamePerf, PerfCheckmateIterator, PerfCheckmateFoolsMate};

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
    TestIterator(PerfTests, sizeof(PerfTests)/sizeof(void*), "Perf Tests ");
    cout << "========= Testing complete ========" << endl << endl;
}
