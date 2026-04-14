#include "core/Game.hpp"
#include "core/Board.hpp"
#include "core/Common.hpp"
#include "ui/ChessUI.hpp"
#include <array>

int main()
{
    Game g;
    ChessUI ui;
    bool running = true;
    while (running)
    {
        ui.cls();
        ui.printBoard(g.getBoard(), g.getTurn());
        if (g.hasMoves(g.getTurn()))
        {
            ui.println("What piece do you want to move?");
            std::string input;
            Piece piece;
            Coord pieceCoord = {8, 8};
            std::array<Coord, 27> posb;
            do
            {
                input = ui.getInput();
                piece = ui.translatePiece(input);
                if (piece.t != PieceType::ERROR)
                {
                    pieceCoord = g.getBoard().getCoordinates(piece);
                    posb = g.possibleMoves(pieceCoord);
                }
            } while (!isValidCoord(posb[0]));
            ui.println("Where do you want to move it to?");
            ui.printMoves(posb);
            Coord to;
            do
            {
                input = ui.getInput();
                to = ui.translateCoordinates(input);
            } while (!isValidCoord(to));

            g.makeMove(pieceCoord, to);
            g.changeTurn();
        }
        else
        {
            if (g.isKingInCheck(g.getTurn()))
            {
                std::string pieceWin = g.getTurn() == Color::WHITE ? "Black" : "White";
                std::string winningText = "The " + pieceWin + " pieces have won by checkmate!";
                ui.println(winningText);
                running = false;
            }
            else
            {
                ui.println("Draw by stalemate!");
            }
        }
    }
}