#pragma once
#include <array>
#include <string>
#include <iostream>
#include "../board/Board.hpp"

// View Class.
// Handles all user input and output.
// No chess logic or board state — only rendering and input translation.
class ChessUI
{
public:
    // Prints the board based on the color orientation.
    static void printBoard(const Board &b, Color orientation);

    // Wrapper for a getline().
    static std::string getInput();

    // Matches the input text to a possible piece.
    // Returns a Piece with a PieceType::ERROR if no matches are possible.
    static Piece translatePiece(const std::string &p);

    // Matches the input text to a possible coordinate on the board.
    // Returns an Out-of-Bounds `Coord {8, 8}` if no matches are possible.
    static Coord translateCoordinates(const std::string &coord);

    // Wrapper for a simple cout statement with an endl.
    static void println(const std::string &s);

    // Prints all possible moves inside a Coord[27] array.
    static void printMoves(const std::array<Coord, 27> &posb);

    // Converts a coordinate into algebraic chess form.
    // Returns "ER" if the given coordinates are invalid.
    static std::string convertCoordToText(Coord c);

    // Converts a piece into text form.
    // Returns "ERR" if the given piece is invalid.
    static std::string convertPieceToText(Piece p);

    // Prints the given piece movement.
    static void printMovePiece(Piece p, Coord from, Coord to);
};
