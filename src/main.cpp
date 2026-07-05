#include "game/Game.hpp"
#include "board/Board.hpp"
#include "types/Common.hpp"
#include "ui/ChessUI.hpp"
#include "engine/Engine.hpp"
#include <array>

// Code by André Santos
// Main game instance holds a history stack, accessible through g.getHistory().

int main()
{
    Game g;

    ChessUI::println("What depth will the Engine go to? (from 1 to 6)");
    std::string strdepth = ChessUI::getInput();
    while (strdepth[0] < '1' || strdepth[0] > '6')
        strdepth = ChessUI::getInput();
    const int depth = strdepth[0] - '1';

    ChessUI::println("Welcome! Which color will you play as? (w for White and b for Black)");
    std::string color = ChessUI::getInput();
    while (color[0] != 'w' && color[0] != 'b')
        color = ChessUI::getInput();

    Color playerColor = color[0] == 'w'? Color::WHITE : Color::BLACK;

    bool running = true;
    while (running)
    {
        // Looking for checkmate or stalemate.
        if (g.hasMoves(g.getTurn()) && !g.staleMateByMaterial())
        {
            // No checkmate or stalemate found.
            if (g.getTurn() != playerColor)
            {
                // AI turn.
                std::array<Coord, 2> bestMove = Engine::getBestMove(g, playerColor == Color::WHITE ? Color::BLACK : Color::WHITE, depth);
                ChessUI::printMovePiece(g.getBoard().getPiece(bestMove[0]), bestMove[0], bestMove[1]);
                g.makeMove(bestMove[0], bestMove[1]);
            }
            else
            {
                // Player turn.
                ChessUI::printBoard(g.getBoard(), g.getTurn());
                // Piece selection.
                ChessUI::println("What piece do you want to move?");
                std::string input;
                Coord pieceCoord = {8, 8};
                // Array of possible moves
                std::array<Coord, 27> posb;
                // Making sure the user inputs a valid piece that is on the board.
                do
                {
                    input = ChessUI::getInput();
                    Piece piece = ChessUI::translatePiece(input);
                    if (piece.t != PieceType::ERROR)
                    {
                        pieceCoord = g.getBoard().getCoordinates(piece);
                        // Stops the user from selecting a piece that can't move (Detects pins).
                        // Not checking for validity before calling because the function will always return sentinel values for invalid input.
                        posb = g.possibleMoves(pieceCoord);
                    }
                } while (!isValidCoord(posb[0]));

                // Move selection.
                ChessUI::println("Where do you want to move it to? (type 'undo' to go to previous step)");
                ChessUI::printMoves(posb);
                Coord to = {8, 8};
                bool isPossible = false;
                // Validating user input, must be a valid coordinate and be in the possible moves array.
                do
                {
                    input = ChessUI::getInput();
                    if (input == "undo")
                        break;
                    to = ChessUI::translateCoordinates(input);
                    if (!isValidCoord(to))
                        continue;
                    for (int i = 0; i < 27; i++)
                    {
                        if (!isValidCoord(posb[i]))
                            break;

                        if (to == posb[i])
                        {
                            isPossible = true;
                            break;
                        }
                    }
                } while (!isPossible);
                // MakeMove() returns true or false depending on if the move was possible to be done, but doing a check
                // here is redundant due to previous input validations.
                g.makeMove(pieceCoord, to);
            }
        }
        // Found checkmate or stalemate.
        else
        {
            ChessUI::printBoard(g.getBoard(), g.getTurn());
            // Looking if in check.
            if (g.isKingInCheck(g.getTurn()))
            {
                std::string pieceWin = g.getTurn() == Color::WHITE ? "Black" : "White";
                std::string winningText = "The " + pieceWin + " pieces have won by checkmate!";
                ChessUI::println(winningText);
                running = false;
            }
            else
            {
                ChessUI::println("Draw by stalemate!");
                running = false;
            }
        }
    }
    ChessUI::println("Press any key to end...");
    ChessUI::getInput();
}
