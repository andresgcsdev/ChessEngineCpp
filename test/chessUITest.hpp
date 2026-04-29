#pragma once

#include <array>
#include <iostream>
#include "core/Common.hpp"
#include "ui/ChessUI.hpp"

// Main UI test routine.
int runChessUITests()
{
    int ERROR_COUNT = 0;
    std::cout << "UI test routine:" << std::endl;
    ChessUI ui;
    std::cout << "    UI instance initialized." << std::endl;
    std::cout << "    Text to Piece transformation:" << std::endl;
    // Ids higher than 0 are permitted for all pieces, even kings, so WK8 isn't supposed to return an error.
    std::array<std::string, 10> strPieces = {
            "WP1", "BP1", "WR3", "BQ0", "WK8", // Success cases
            "wp1", "WP9", "YP1", "BC0", "FE9" // Error cases
            };
    constexpr std::array<const Piece, 5> pieceTestCases = {
            Piece{PieceType::PAWN, Color::WHITE, '1'},
            Piece{PieceType::PAWN, Color::BLACK, '1'},
            Piece{PieceType::ROOK, Color::WHITE, '3'},
            Piece{PieceType::QUEEN, Color::BLACK, '0'},
            Piece{PieceType::KING, Color::WHITE, '8'},
            };
    constexpr Piece perror = Piece{PieceType::ERROR};

    for (int i = 0; i < 5; i++)
    {
        const Piece p = ui.translatePiece(strPieces[i]);
        if (p == pieceTestCases[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }
    for (int i = 5; i < 10; i++)
    {
        const Piece p = ui.translatePiece(strPieces[i]);
        if (p == perror)
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    std::cout << "    Text to Coordinate transformation:" << std::endl;

    const std::array<std::string, 10> strCoordinates = {
            "a1", "h8", "a8", "e5", "d4", // Success cases
            "a0", "h9", "i1", "i9", "i0" // Error cases
            };
    constexpr std::array<const Coord, 5> coordinatesTestCases = {
            Coord{7, 0},
            Coord{0, 7},
            Coord{0, 0},
            Coord{3, 4},
            Coord{4, 3}
            };
    constexpr Coord cerror = Coord{8, 8};

    for (int i = 0; i < 5; i++)
    {
        const Coord c = ui.translateCoordinates(strCoordinates[i]);
        if (c == coordinatesTestCases[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    for (int i = 5; i < 10; ++i)
    {
        const Coord c = ui.translateCoordinates(strCoordinates[i]);
        if (c == cerror)
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    std::cout << "    Coordinate to Text transformation:" << std::endl;
    constexpr std::array<Coord, 11> coordinates = {
            Coord{7, 0}, Coord{0, 7}, Coord{0, 0}, Coord{3, 4}, Coord{4, 3}, // Success Cases
            Coord{-1, 0}, Coord{0, -1}, Coord{8, 0}, Coord{0, 8}, Coord{-1, -1}, Coord{8, 8} // Error Cases
            };
    const std::array<std::string, 5> strCoordinatesTestCases = {"a1", "h8", "a8", "e5", "d4"};
    const std::string strcerror = "ER";
    for (int i = 0; i < 5; i++)
    {
        std::string s = ui.convertCoordToText(coordinates[i]);
        if (s == strCoordinatesTestCases[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    for (int i = 5; i < 11; ++i)
    {
        std::string s = ui.convertCoordToText(coordinates[i]);
        if (s == strcerror)
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    std::cout << "    Piece to Text transformation:" << std::endl;
    //
    constexpr std::array<Piece, 8> pieces = {
            Piece{PieceType::BLANK}, Piece{PieceType::PAWN, Color::WHITE, '1'}, // Success Cases
            Piece{PieceType::ROOK, Color::WHITE, '0'}, Piece{PieceType::KNIGHT, Color::BLACK, 'a'}, // Success Cases
            Piece{PieceType::QUEEN, Color::WHITE, 'q'}, Piece{PieceType::BISHOP}, Piece{PieceType::KING, Color::BLACK, '-'}, // Success Cases
            Piece{PieceType::ERROR} // Error Cases
            };
    const std::array<std::string, 7> strPiecesTestCases = {"   ", "WP1", "WR0", "BNa", "WQq", "BBb", "BK-"};
    const std::string strperror = "ERR";

    for (int i = 0; i < 7; i++)
    {
        std::string s = ui.convertPieceToText(pieces[i]);
        if (s == strPiecesTestCases[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    if (ui.convertPieceToText(pieces[7]) == strperror)
    {
        std::cout << "        SUCCESS on case " << 8 << '.' << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case " << 8 << '!' << std::endl;
        ERROR_COUNT++;
    }

    return ERROR_COUNT;
}
