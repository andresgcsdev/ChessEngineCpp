#pragma once

#include <iostream>
#include <array>
#include "core/Common.hpp"
#include "core/Board.hpp"
#include "testHelper.hpp"

// Main Board class test routine function.
int runBoardTests()
{
    int ERROR_COUNT = 0;

    std::cout << "Board test routine:" << std::endl;
    Board b;
    std::cout << "    Board instance initialized." << std::endl;
    std::cout << "    All blank initialization:" << std::endl;
    // ASSUMES b.getPiece() works. It's a get method that simply accesses the given coordinates in the array and returns the value found.
    bool foundNotBlank = false;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (b.getPiece(Coord{i, j}).t != PieceType::BLANK)
            {
                std::cout << "        ERROR on coordinate {row:" << i << " column:" << j << "}! Not a blank piece!" << std::endl;
                ERROR_COUNT++;
                foundNotBlank = true;
            }
        }
    }
    if (!foundNotBlank)
        std::cout << "        Blank squares correctly initialized." << std::endl;

    std::cout << "    Set Matrix:" << std::endl;

    std::array<std::array<Piece, 8>, 8> matrixSetText;

    // Setting matrix by hand because FEN creator does not support different IDs.
    // Different IDs are crucial to some of the following tests.

    // Row 0 (White back rank)
    matrixSetText[7][0] = Piece{PieceType::ROOK, Color::WHITE, '1'};
    matrixSetText[7][1] = Piece{PieceType::KNIGHT, Color::WHITE, '1'};
    matrixSetText[7][2] = Piece{PieceType::BISHOP, Color::WHITE, '1'};
    matrixSetText[7][3] = Piece{PieceType::QUEEN, Color::WHITE, '0'};
    matrixSetText[7][4] = Piece{PieceType::KING, Color::WHITE, '0'};
    matrixSetText[7][5] = Piece{PieceType::BISHOP, Color::WHITE, '2'};
    matrixSetText[7][6] = Piece{PieceType::KNIGHT, Color::WHITE, '2'};
    matrixSetText[7][7] = Piece{PieceType::ROOK, Color::WHITE, '2'};

    // Row 1 (White pawns)
    for (int col = 0; col < 8; col++)
    {
        matrixSetText[6][col] = Piece{PieceType::PAWN, Color::WHITE, static_cast<char>('1' + col)};
    }

    // Rows 5-2 (Empty - defaults are already BLANK)

    // Row 6 (Black pawns)
    for (int col = 0; col < 8; col++)
    {
        matrixSetText[1][col] = Piece{PieceType::PAWN, Color::BLACK, static_cast<char>('1' + col)};
    }

    // Row 7 (Black back rank)
    matrixSetText[0][0] = Piece{PieceType::ROOK, Color::BLACK, '1'};
    matrixSetText[0][1] = Piece{PieceType::KNIGHT, Color::BLACK, '1'};
    matrixSetText[0][2] = Piece{PieceType::BISHOP, Color::BLACK, '1'};
    matrixSetText[0][3] = Piece{PieceType::QUEEN, Color::BLACK, '0'};
    matrixSetText[0][4] = Piece{PieceType::KING, Color::BLACK, '0'};
    matrixSetText[0][5] = Piece{PieceType::BISHOP, Color::BLACK, '2'};
    matrixSetText[0][6] = Piece{PieceType::KNIGHT, Color::BLACK, '2'};
    matrixSetText[0][7] = Piece{PieceType::ROOK, Color::BLACK, '2'};

    b.setMatrix(matrixSetText);

    // ASSUMES b.snapshot() works. It's a base get method with a different name, returns a copy of the board matrix.
    SnapShot snapMatrixSet = SnapShot{b.snapshot(), defaultGameState, defaultBlackKing, defaultWhiteKing};

    std::string fenSetMatrix = FEN_notation_generator(snapMatrixSet);
    if (fenSetMatrix == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -")
        std::cout << "        Successfully copied the generated matrix." << std::endl;
    else
    {
        std::cout << "        Error! Matrix not copied correctly." << std::endl;
        ERROR_COUNT++;
    }


    std::cout << "    Find coordinate of piece:" << std::endl;

    std::array<Piece, 19> findPiece =
            {
            Piece{PieceType::ROOK, Color::BLACK, '1'}, // Success cases
            Piece{PieceType::KNIGHT, Color::BLACK, '1'}, // Success cases
            Piece{PieceType::BISHOP, Color::BLACK, '1'}, // Success cases
            Piece{PieceType::QUEEN, Color::BLACK, '0'}, // Success cases
            Piece{PieceType::KING, Color::BLACK, '0'}, // Success cases
            Piece{PieceType::BISHOP, Color::BLACK, '2'}, // Success cases
            Piece{PieceType::KNIGHT, Color::BLACK, '2'}, // Success cases
            Piece{PieceType::PAWN, Color::BLACK, '1'}, // Success cases
            Piece{PieceType::ROOK, Color::WHITE, '1'}, // Success cases
            Piece{PieceType::KNIGHT, Color::WHITE, '1'}, // Success cases
            Piece{PieceType::BISHOP, Color::WHITE, '1'}, // Success cases
            Piece{PieceType::QUEEN, Color::WHITE, '0'}, // Success cases
            Piece{PieceType::KING, Color::WHITE, '0'}, // Success cases
            Piece{PieceType::BISHOP, Color::WHITE, '2'}, // Success cases
            Piece{PieceType::KNIGHT, Color::WHITE, '2'}, // Success cases
            Piece{PieceType::PAWN, Color::WHITE, '1'}, // Success cases
            Piece{PieceType::ROOK, Color::WHITE, '4'}, // Error cases
            Piece{PieceType::ERROR, Color::WHITE, '0'}, // Error cases
            Piece{PieceType::BLANK, Color::BLACK, 'b'} // Error cases
            };
    std::array<Coord, 16> findPieceTestCases =
            {
            Coord{0, 0},
            Coord{0, 1},
            Coord{0, 2},
            Coord{0, 3},
            Coord{0, 4},
            Coord{0, 5},
            Coord{0, 6},
            Coord{1, 0},
            Coord{7, 0},
            Coord{7, 1},
            Coord{7, 2},
            Coord{7, 3},
            Coord{7, 4},
            Coord{7, 5},
            Coord{7, 6},
            Coord{6, 0},
            };

    for (int i = 0; i < 16; i++)
    {
        Coord c = b.getCoordinates(findPiece[i]);
        if (c == findPieceTestCases[i])
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    for (int i = 16; i < 19; i++)
    {
        Coord c = b.getCoordinates(findPiece[i]);
        if (!isValidCoord(c))
        {
            std::cout << "        SUCCESS on case " << i + 1 << '.' << std::endl;
        }
        else
        {
            std::cout << "        ERROR on case " << i + 1 << '!' << std::endl;
            ERROR_COUNT++;
        }
    }

    std::cout << "    Move piece:" << std::endl;
    // Making sure basic matrix is set.
    b.setMatrix(FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
    // Moving e2 pawn to e4.
    b.move(Coord{6, 4}, Coord{4, 4});

    SnapShot movingOnBoard;

    GameState gameSMoveOnBoard = defaultGameState;
    gameSMoveOnBoard.enPassant = Coord{5, 4};
    gameSMoveOnBoard.turn = Color::BLACK;

    movingOnBoard.board = b.snapshot();
    movingOnBoard.state = gameSMoveOnBoard;

    movingOnBoard.blackKing = defaultBlackKing;
    movingOnBoard.whiteKing = defaultWhiteKing;

    std::string fenMove = FEN_notation_generator(movingOnBoard);
    if (fenMove == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3")
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR unexpected movement behavior on case 1." << std::endl;
        ERROR_COUNT++;
    }

    // Undoing move.
    b.move(Coord{4, 4}, Coord{6, 4});
    movingOnBoard.board = b.snapshot();
    movingOnBoard.state = defaultGameState;
    fenMove = FEN_notation_generator(movingOnBoard);
    if (fenMove == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -")
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR unexpected movement behavior on case 2." << std::endl;
        ERROR_COUNT++;
    }

    // Making sure basic matrix is set.
    b.setMatrix(FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"));
    // Moving g1 knight to f3.
    b.move(Coord{7, 6}, Coord{1, 7});
    movingOnBoard.board = b.snapshot();
    movingOnBoard.state = defaultGameState;
    movingOnBoard.state.turn = Color::BLACK;
    fenMove = FEN_notation_generator(movingOnBoard);
    if (fenMove == "rnbqkbnr/pppppppN/8/8/8/8/PPPPPPPP/RNBQKB1R b KQkq -")
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR unexpected movement behavior on case 3." << std::endl;
        ERROR_COUNT++;
    }

    // Undoing move.
    b.move(Coord{1, 7}, Coord{7, 6});
    movingOnBoard.board = b.snapshot();
    movingOnBoard.state = defaultGameState;
    fenMove = FEN_notation_generator(movingOnBoard);
    if (fenMove == "rnbqkbnr/ppppppp1/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -")
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR unexpected movement behavior on case 4." << std::endl;
        ERROR_COUNT++;
    }

    return ERROR_COUNT;
}
