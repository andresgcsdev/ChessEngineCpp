#pragma once

#include <array>
#include <iostream>
#include "core/Common.hpp"

// Main Common operations test routine.
int runCommonTests()
{
    int ERROR_COUNT = 0;

    std::cout << "Common Operations test routine:" << std::endl;
    std::cout << "    Coordinate validation basic function:" << std::endl;
    constexpr std::array<Coord, 8> commonCoordTest = {
            Coord{1, 2}, Coord{0, 0}, // Success cases
            Coord{8, 8}, Coord{-1, -1}, Coord{8, 0}, Coord{0, 8}, Coord{-1, 0}, Coord{0, -1} // Error Cases
            };
    for (int i = 0; i < 2; i++)
    {
        if (isValidCoord(commonCoordTest[i]))
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }
    for (int i = 2; i < 8; i++)
    {
        if (!isValidCoord(commonCoordTest[i]))
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }
    std::cout << "    Coordinate equality and inequality operators:" << std::endl;
    for (int i = 0; i < 8; i++)
    {
        if (commonCoordTest[i] == commonCoordTest[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }
    for (int i = 0; i < 8; i++)
    {
        if (commonCoordTest[i] != Coord{9, 9})
        {
            std::cout << "        SUCCESS on case " << i + 9 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 9 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    std::cout << "    Piece equality operator:" << std::endl;
    constexpr std::array<Piece, 4> pieceComparisons = {
            Piece{PieceType::PAWN, Color::WHITE, '1'},
            Piece{PieceType::PAWN, Color::WHITE, '1'},
            Piece{PieceType::PAWN, Color::WHITE, '2'},
            Piece{PieceType::ROOK, Color::WHITE, '1'}
            };

    if (pieceComparisons[0] == pieceComparisons[1])
    {
        std::cout << "        SUCCESS on case 1" << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1" << std::endl;
    }

    if (pieceComparisons[0] == pieceComparisons[2])
    {
        std::cout << "        ERROR on case 2" << std::endl;
    }
    else
    {
        std::cout << "        SUCCESS on case 2" << std::endl;
    }

    if (pieceComparisons[0] == pieceComparisons[3])
    {
        std::cout << "        ERROR on case 3" << std::endl;
    }
    else
    {
        std::cout << "        SUCCESS on case 3" << std::endl;
    }

    return ERROR_COUNT;
}
