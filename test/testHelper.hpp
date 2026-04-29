#pragma once

#include <iostream>
#include <array>
#include <cctype>
#include "core/Common.hpp"
#include "core/Board.hpp"
#include "ui/ChessUI.hpp"

constexpr GameState defaultGameState = GameState{Color::WHITE, {{true, true}, {true, true}}, Coord{8, 8}};
constexpr Coord defaultBlackKing = Coord{0, 4};
constexpr Coord defaultWhiteKing = Coord{7, 4};

// Transforms a board into a FEN chess notation string.
// Easier to use for tests, rather than accessing a matrix.
std::string FEN_notation_generator(const SnapShot &snap)
{
    std::string FEN;
    // Board conversion.
    Board b;
    b.setMatrix(snap.board);
    for (int row = 0; row < 8; row++)
    {
        int consecutiveBlank = 0;
        for (int col = 0; col < 8; col++)
        {
            Piece piece = b.getPiece(Coord{row, col});
            if (piece.t == PieceType::BLANK)
            {
                consecutiveBlank++;
                continue;
            }
            if (consecutiveBlank > 0)
            {
                char numberOfBlanks = '0' + consecutiveBlank;
                FEN += numberOfBlanks;
                consecutiveBlank = 0;
            }
            char pieceType = ' ';
            switch (piece.t)
            {
                case PieceType::PAWN:
                    pieceType = 'P';
                    break;
                case PieceType::KNIGHT:
                    pieceType = 'N';
                    break;
                case PieceType::ROOK:
                    pieceType = 'R';
                    break;
                case PieceType::BISHOP:
                    pieceType = 'B';
                    break;
                case PieceType::QUEEN:
                    pieceType = 'Q';
                    break;
                case PieceType::KING:
                    pieceType = 'K';
                    break;
                default:
                    break;
            }
            if (piece.c == Color::BLACK)
                pieceType = tolower(pieceType);
            FEN += pieceType;
        }
        if (consecutiveBlank > 0)
        {
            char numberOfBlanks = '0' + consecutiveBlank;
            FEN += numberOfBlanks;
        }
        if (row < 7)
            FEN += '/';
    }
    // Turn conversion.
    FEN += snap.state.turn == Color::WHITE ? " w " : " b ";

    // King side and Queen side castling check for both colors.
    if (snap.state.castling[1][0])
        FEN += "K";
    if (snap.state.castling[0][0])
        FEN += "Q";
    if (snap.state.castling[1][1])
        FEN += "k";
    if (snap.state.castling[0][1])
        FEN += "q";
    if (!snap.state.castling[0][0] && !snap.state.castling[1][0] && !snap.state.castling[1][1] && !snap.state.castling[0][1])
        FEN += " -";

    // En Passant conversion
    if (isValidCoord(snap.state.enPassant))
    {
        ChessUI ui;
        FEN += ' ' + ui.convertCoordToText(snap.state.enPassant);
    }
    else
    {
        FEN += " -";
    }

    return FEN;
}

// Returns the piece that matches the FEN char. Will always have id = 0.
Piece matchCharToPiece(char charPiece)
{
    Piece p = {PieceType::BLANK, Color::BLACK, '0'};
    if (std::islower(charPiece))
    {
        switch (charPiece)
        {
            case 'p':
                p.t = PieceType::PAWN;
                break;
            case 'r':
                p.t = PieceType::ROOK;
                break;
            case 'n':
                p.t = PieceType::KNIGHT;
                break;
            case 'b':
                p.t = PieceType::BISHOP;
                break;
            case 'q':
                p.t = PieceType::QUEEN;
                break;
            case 'k':
                p.t = PieceType::KING;
                break;
            default:
                break;
        }
    }
    else
    {
        p.c = Color::WHITE;
        switch (charPiece)
        {
            case 'P':
                p.t = PieceType::PAWN;
                break;
            case 'R':
                p.t = PieceType::ROOK;
                break;
            case 'N':
                p.t = PieceType::KNIGHT;
                break;
            case 'B':
                p.t = PieceType::BISHOP;
                break;
            case 'Q':
                p.t = PieceType::QUEEN;
                break;
            case 'K':
                p.t = PieceType::KING;
                break;
            default:
                break;
        }
    }
    return p;
}

// Converts a FEN string into a Piece matrix.
// Parses only the piece placement part of FEN (before the first space).
// Assigns all pieces default ID '0'.
std::array<std::array<Piece, 8>, 8> FEN_to_matrix(std::string fen)
{
    std::array<std::array<Piece, 8>, 8> matrix;
    Piece blank = {PieceType::BLANK, Color::BLACK, 'b'};

    int row = 0, col = 0;

    for (char c: fen)
    {
        if (c == '/')
        {
            row++;
            col = 0;
            continue;
        }

        if (std::isdigit(c))
        {
            int emptyCount = c - '0';
            for (int i = 0; i < emptyCount; i++)
            {
                matrix[row][col] = blank;
                col++;
            }
        }
        else
        {
            matrix[row][col] = matchCharToPiece(c);
            col++;
        }
    }

    return matrix;
}

// Helper function to validate the array of possible moves the Game class returns.
// ASSUMES the number of valid moves in the `moves` array matches the size of the `expected` array.
// Returns true when the generated moves match the expected ones.
// Returns false otherwise.
template<size_t N>
bool validateMoves(const std::array<Coord, 27> &moves, const std::array<Coord, N> &expected)
{
    int countMoves = 0;
    for (int i = 0; i < 27; i++)
    {
        if (isValidCoord(moves[i]))
            countMoves++;
    }
    if (countMoves != N)
    {
        return false;
    }

    for (int i = 0; i < N; i++)
    {
        bool foundMatch = false;
        for (int j = 0; j < 27; j++)
        {
            if (moves[j] == expected[i])
            {
                foundMatch = true;
                break;
            }
        }
        if (!foundMatch)
            return false;
    }

    return true;
}

// Helper function to be able to see the moves generated with g.possibleMoves() better.
// Used only while building the test routine, for debugging. Isn't actually used for the test units.
void viewMoves(const std::array<Coord, 27> &posb)
{
    for (int i = 0; i < 27; i++)
    {
        if (posb[i] == Coord{8, 8})
            break;
        std::cout << "Coord{" << posb[i].row << ", " << posb[i].col << "}," << std::endl;
    }
}