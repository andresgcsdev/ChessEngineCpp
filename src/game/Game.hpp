#pragma once
#include <array>
#include <stack>
#include "../board/Board.hpp"
#include "../types/Common.hpp"

// Controller Class.
// Manages all chess logic, rules, and game state.
// Operates on the Board through moves, validation, and turn management.
class Game
{
public:
    // Sets board and game state
    Game();

    // Returns a copy of the board.
    Board getBoard() const;

    // Returns a copy of the turn color.
    Color getTurn() const;

    // Validates and makes the move, returning true if the move was done and false if the move was illegal and couldn't be done.
    // Also saves the last board state in the history.
    // Used for player moves that access the ui.
    bool makeMove(Coord from, Coord to);

    // Validates and applies the move.
    // Does not save the last board state in the history.
    // Tracking and storing the previous states are responsibility of the caller.
    void applyMove(Coord from, Coord to);

    // Returns an array of all possible moves the piece can make from the given position.
    // Possible moves are the ones that follow chess rules and do not result on checks of oneself's king.
    // Returns an array of invalid positions if no moves are available for that piece.
    std::array<Coord, 27> possibleMoves(const Coord& piece);

    // True if king of color `c` is in check, False otherwise.
    bool isKingInCheck(Color c);

    // Calculates if the pieces of the matching `c` color have any possible moves.
    // Returns true if any move is found, false if not.
    bool hasMoves(Color c);

    // Reverts the board and game states to the previous state in the history.
    // Used for undoing player moves.
    void undo();

    // Reverts the board and game states to the SnapShot's.
    // Overall state validity becomes the caller's responsibility.
    // Use with getSnap() to have predictable behavior.
    void revertState(const SnapShot &snap);

    // Reverts the board and game states to the SnapShot's.
    // Only valid when passed the result of getCheapSnap() from the immediately preceding move.
    // Behavior is undefined for arbitrary CheapSnap values.
    void revertState(const CheapSnap &snap);

    // Returns a SnapShot of current board and game states.
    SnapShot getSnap() const;

    // Returns a CheapSnap of the current game state and previous move.
    CheapSnap getCheapSnap() const;

    // Returns a copy of the current game history.
    std::stack<SnapShot> getHistory() const;

    // Returns true when there are only kings on the board.
    bool staleMateByMaterial() const;

    GameState getCurrentState() const;

private:
    std::stack<SnapShot> history;
    Board board;
    GameState gameState;
    Coord blackKing;
    Coord whiteKing;

    // Describes the previous state of the board.
    // Used with getCheapSnap() and reverseState() to undo a move.
    // Cheaper than using a full SnapShot of the board.
    CheapSnap lastCheapSnap;

    // Takes a snapshot of the board, copies the current game state and adds it to the stack.
    // Use only just before making a change on the board or game state.
    void setHistory();

    // Execute a move on the board. ASSUMES move has been validated by caller.
    // Does not check legality or if move leaves king in check.
    bool move(Coord from, Coord to);

    // Invert the turn.
    void changeTurn();

    // Returns true when move results in a check to the self king
    bool testForCheck(Coord from, Coord to);
};