#pragma once

#include <iostream>
#include <array>
#include "core/Common.hpp"
#include "core/Game.hpp"
#include "testHelper.hpp"

// SnapShot copying test routine.
void testSnapCopy(int &ERROR_COUNT, Game &g)
{
    std::cout << "    SnapShot setter:" << std::endl;
    SnapShot testSnap;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    g.revertState(testSnap);
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -")
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    testSnap.state.turn = Color::BLACK;
    testSnap.state.enPassant = Coord{5, 4};
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR");
    g.revertState(testSnap);
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3")
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
}

// Pawn move generator test routine.
void testPawnMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    std::cout << "    Pawn movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.blackKing = Coord{8, 8};
    testSnap.whiteKing = Coord{8, 8};
    // Pawn Routine
    // Test for single movement in row.
    // White
    testSnap.board = FEN_to_matrix("8/8/8/8/4P3/8/8/8");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);

    std::array<Coord, 1> singleMovePawn = {Coord{3, 4}};
    if (validateMoves(g.possibleMoves(Coord{4, 4}), singleMovePawn))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    // Black
    testSnap.board = FEN_to_matrix("8/8/8/3p4/8/8/8/8");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);

    singleMovePawn[0] = Coord{4, 3};
    if (validateMoves(g.possibleMoves(Coord{3, 3}), singleMovePawn))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    // Test for double movement in row.
    // White
    testSnap.board = FEN_to_matrix("8/8/8/8/8/8/4P3/8");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    std::array<Coord, 2> doubleMovePawn = {Coord{5, 4}, Coord{4, 4}};
    if (validateMoves(g.possibleMoves(Coord{6, 4}), doubleMovePawn))
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.board = FEN_to_matrix("8/4p3/8/8/8/8/8/8");
    testSnap.state.turn = Color::BLACK;
    doubleMovePawn = {Coord{2, 4}, Coord{3, 4}};
    g.revertState(testSnap);
    if (validateMoves(g.possibleMoves(Coord{1, 4}), doubleMovePawn))
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }

    // Test for capture in diagonal.
    // White
    testSnap.board = FEN_to_matrix("8/8/8/2p1p3/3P4/8/8/8");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    std::array<Coord, 3> tripleMovePawn = {Coord{3, 2}, Coord{3, 3}, Coord{3, 4}};
    if (validateMoves(g.possibleMoves(Coord{4, 3}), tripleMovePawn))
    {
        std::cout << "        SUCCESS on case 5." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 5." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.board = FEN_to_matrix("8/8/8/3p4/2P1P3/8/8/8");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    tripleMovePawn = {Coord{4, 2}, Coord{4, 3}, Coord{4, 4}};
    if (validateMoves(g.possibleMoves(Coord{3, 3}), tripleMovePawn))
    {
        std::cout << "        SUCCESS on case 6." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 6." << std::endl;
        ERROR_COUNT++;
    }

    // Test for En Passant.
    // White
    testSnap.board = FEN_to_matrix("8/8/8/3pP3/8/8/8/8");
    testSnap.state.turn = Color::WHITE;
    testSnap.state.enPassant = Coord{2, 3};
    g.revertState(testSnap);
    doubleMovePawn = {Coord{2, 4}, Coord{2, 3}};
    if (validateMoves(g.possibleMoves(Coord{3, 4}), doubleMovePawn))
    {
        std::cout << "        SUCCESS on case 7." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 7." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.board = FEN_to_matrix("8/8/8/8/3pP3/8/8/8");
    testSnap.state.turn = Color::BLACK;
    testSnap.state.enPassant = Coord{5, 4};
    g.revertState(testSnap);
    doubleMovePawn = {Coord{5, 3}, Coord{5, 4}};
    if (validateMoves(g.possibleMoves(Coord{4, 3}), doubleMovePawn))
    {
        std::cout << "        SUCCESS on case 8." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 8." << std::endl;
        ERROR_COUNT++;
    }
}

// Knight move generator test routine.
void testKnightMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Knight Routine
    // White
    std::cout << "    Knight movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("N6N/8/8/8/3N4/8/8/N6N");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr Coord upperLeftCorner = {0, 0};
    constexpr Coord upperRightCorner = {0, 7};
    constexpr Coord lowerLeftCorner = {7, 0};
    constexpr Coord lowerRightCorner = {7, 7};

    std::array<Coord, 2> expectedWhiteKnightCorners = {Coord{2, 1}, Coord{1, 2}};
    if (validateMoves(g.possibleMoves(upperLeftCorner), expectedWhiteKnightCorners))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    expectedWhiteKnightCorners = {Coord{2, 6}, Coord{1, 5}};
    if (validateMoves(g.possibleMoves(upperRightCorner), expectedWhiteKnightCorners))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    expectedWhiteKnightCorners = {Coord{5, 1}, Coord{6, 2}};
    if (validateMoves(g.possibleMoves(lowerLeftCorner), expectedWhiteKnightCorners))
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }

    expectedWhiteKnightCorners = {Coord{5, 6}, Coord{6, 5}};
    if (validateMoves(g.possibleMoves(lowerRightCorner), expectedWhiteKnightCorners))
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }

    constexpr std::array<Coord, 8> expectedWhiteKnightCenter = {
            Coord{2, 4}, Coord{6, 4},
            Coord{2, 2}, Coord{6, 2},
            Coord{5, 1}, Coord{5, 5},
            Coord{3, 1}, Coord{3, 5}
            };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedWhiteKnightCenter))
    {
        std::cout << "        SUCCESS on case 5." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 5." << std::endl;
        ERROR_COUNT++;
    }
}

// Rook move generator test routine.
void testRookMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Rook Routine
    // White
    std::cout << "    Rook movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("4n3/8/8/8/1q2RB2/8/8/4P3");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr std::array<Coord, 9> expectedWhiteRook = {
        Coord{4, 3}, Coord{4, 2}, Coord{4, 1}, // Left
        Coord{3, 4}, Coord{2, 4}, Coord{1, 4}, Coord{0, 4}, // Up
        Coord{5, 4}, Coord{6, 4} // Down
    };
    if (validateMoves(g.possibleMoves(Coord{4, 4}), expectedWhiteRook))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("4N3/8/8/8/1Q2rb2/8/8/4p3");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    constexpr std::array<Coord, 9> expectedBlackRook = {
        Coord{4, 3}, Coord{4, 2}, Coord{4, 1}, // Left
        Coord{3, 4}, Coord{2, 4}, Coord{1, 4}, Coord{0, 4}, // Up
        Coord{5, 4}, Coord{6, 4} // Down
    };
    if (validateMoves(g.possibleMoves(Coord{4, 4}), expectedBlackRook))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
}

// Bishop move generator test routine.
void testBishopMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Bishop Routine
    // White
    std::cout << "    Bishop movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/1p5/4P3/3B4/2p5/8/6P1");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr std::array<Coord, 5> expectedWhiteBishop = {
        Coord{3, 2}, Coord{2, 1}, // North-West
        Coord{5, 4}, Coord{6, 5}, // South-East
        Coord{5, 2} // South-WEST
    };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedWhiteBishop))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/1P5/4p3/3b4/2P5/8/6p1");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    constexpr std::array<Coord, 5> expectedBlackBishop = {
        Coord{3, 2}, Coord{2, 1}, // North-West
        Coord{5, 4}, Coord{6, 5}, // South-East
        Coord{5, 2} // South-WEST
    };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedBlackBishop))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
}

// Queen move generator test routine.
void testQueenMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Queen routine
    // White
    std::cout << "    Queen movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/1p5/4P3/3Q4/2p5/8/6P1");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr std::array<Coord, 19> expectedWhiteQueenDiagonal = {
            Coord{5, 3}, Coord{6, 3}, Coord{7, 3},
            Coord{3, 3}, Coord{2, 3}, Coord{1, 3},
            Coord{0, 3}, Coord{4, 4}, Coord{4, 5},
            Coord{4, 6}, Coord{4, 7}, Coord{4, 2},
            Coord{4, 1}, Coord{4, 0}, Coord{5, 4},
            Coord{6, 5}, Coord{5, 2}, Coord{3, 2},
            Coord{2, 1}
            };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedWhiteQueenDiagonal))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/1P5/4p3/3q4/2P5/8/6p1");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    constexpr std::array<Coord, 19> expectedBlackQueenDiagonal = {
            Coord{5, 3}, Coord{6, 3}, Coord{7, 3},
            Coord{3, 3}, Coord{2, 3}, Coord{1, 3},
            Coord{0, 3}, Coord{4, 4}, Coord{4, 5},
            Coord{4, 6}, Coord{4, 7}, Coord{4, 2},
            Coord{4, 1}, Coord{4, 0}, Coord{5, 4},
            Coord{6, 5}, Coord{5, 2}, Coord{3, 2},
            Coord{2, 1}
            };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedBlackQueenDiagonal))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("4n3/8/8/8/1q2QB2/8/8/4P3");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr std::array<Coord, 22> expectedWhiteQueenOrthogonal = {
            Coord{5, 4}, Coord{6, 4}, Coord{3, 4}, Coord{2, 4},
            Coord{1, 4}, Coord{0, 4}, Coord{4, 3}, Coord{4, 2},
            Coord{4, 1}, Coord{5, 5}, Coord{6, 6}, Coord{7, 7},
            Coord{3, 5}, Coord{2, 6}, Coord{1, 7}, Coord{5, 3},
            Coord{6, 2}, Coord{7, 1}, Coord{3, 3}, Coord{2, 2},
            Coord{1, 1}, Coord{0, 0}
            };
    if (validateMoves(g.possibleMoves(Coord{4, 4}), expectedWhiteQueenOrthogonal))
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("4N3/8/8/8/1Q2qb2/8/8/4p3");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    constexpr std::array<Coord, 22> expectedBlackQueenOrthogonal = {
            Coord{5, 4}, Coord{6, 4}, Coord{3, 4}, Coord{2, 4},
            Coord{1, 4}, Coord{0, 4}, Coord{4, 3}, Coord{4, 2},
            Coord{4, 1}, Coord{5, 5}, Coord{6, 6}, Coord{7, 7},
            Coord{3, 5}, Coord{2, 6}, Coord{1, 7}, Coord{5, 3},
            Coord{6, 2}, Coord{7, 1}, Coord{3, 3}, Coord{2, 2},
            Coord{1, 1}, Coord{0, 0}
            };
    if (validateMoves(g.possibleMoves(Coord{4, 4}), expectedBlackQueenOrthogonal))
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }
}

// King move generator test routine.
void testKingMoveGen(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // King routine
    // White
    std::cout << "    King movement generator:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/4P3/8/8");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    constexpr std::array<Coord, 9> expectedWhiteKing = {
        Coord{5, 3}, Coord{5, 2}, Coord{3, 3}, Coord{3, 4},
        Coord{3, 2}, Coord{4, 4}, Coord{4, 2}, Coord{4, 1}, Coord{4, 5}
    };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedWhiteKing))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("8/8/8/8/3k4/4p3/8/8");
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    constexpr std::array<Coord, 9> expectedBlackKing = {
        Coord{5, 3}, Coord{5, 2}, Coord{3, 3}, Coord{3, 4},
        Coord{3, 2}, Coord{4, 4}, Coord{4, 2}, Coord{4, 1}, Coord{4, 5}
    };
    if (validateMoves(g.possibleMoves(Coord{4, 3}), expectedBlackKing))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
}

// Square control test routine.
void testKingInCheck(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Check routine
    std::cout << "    King in check:" << std::endl;
    //  White
    testSnap.state = defaultGameState;
    testSnap.blackKing = Coord{8, 8};
    testSnap.whiteKing = Coord{4, 3};
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            testSnap.state.castling[i][j] = false;
    // Test for false positive
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    // Rook
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/3r4");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K3r/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/r2K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("3r4/8/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 5." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 5." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for false positive
    testSnap.board = FEN_to_matrix("3r4/8/8/3R4/r1RKR2r/3R4/8/3r4");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 6." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 6." << std::endl;
        ERROR_COUNT++;
    }

    // Bishop
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/6b1");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 7." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 7." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/b7");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 8." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 8." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/b7/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 9." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 9." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/6b1/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 10." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 10." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for false positive
    testSnap.board = FEN_to_matrix("8/b5b1/8/2B1B3/3K4/2B1B3/8/b5b1");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 11." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 11." << std::endl;
        ERROR_COUNT++;
    }

    // Queen
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/3q4");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 12." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 12." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K3q/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 13." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 13." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/q2K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 14." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 14." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("3q4/8/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 15." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 15." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for false positive
    testSnap.board = FEN_to_matrix("3q4/8/8/3Q4/q1QKQ2q/3Q4/8/3q4");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 16." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 16." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/6q1");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 17." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 17." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/8/q7");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 18." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 18." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/q7/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 19." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 19." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/6q1/8/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 20." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 20." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for false positive
    testSnap.board = FEN_to_matrix("8/q5q1/8/2Q1Q3/3K4/2Q1Q3/8/q5q1");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 21." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 21." << std::endl;
        ERROR_COUNT++;
    }
    // Knight
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/4n3/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 22." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 22." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/5n2/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 23." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 23." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/8/2n5/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 24." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 24." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/1n6/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 25." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 25." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/1n6/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 26." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 26." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/2n5/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 27." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 27." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/4n3/8/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 28." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 28." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/5n2/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 29." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 29." << std::endl;
        ERROR_COUNT++;
    }
    // Pawn
    testSnap.board = FEN_to_matrix("8/8/8/4p3/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 30." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 30." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/2p5/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 31." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 31." << std::endl;
        ERROR_COUNT++;
    }
    // King
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/4k3/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 32." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 32." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/3k4/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 33." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 33." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3K4/2k5/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 34." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 34." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/2kK4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 35." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 35." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/2k5/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 36." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 36." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/3k4/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 37." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 37." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/4k3/3K4/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 38." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 38." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3Kk3/8/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 39." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 39." << std::endl;
        ERROR_COUNT++;
    }
    // Black
    testSnap.whiteKing = Coord{8, 8};
    testSnap.blackKing = Coord{4, 3};
    testSnap.board = FEN_to_matrix("8/8/8/8/3k4/4P3/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 40." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 40." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3k4/2P5/8/8");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 41." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 41." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3k4/8/8/3Q4");
    g.revertState(testSnap);
    if (g.isKingInCheck(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 42." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 42." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("8/8/8/8/3k4/3q4/8/3Q4");
    g.revertState(testSnap);
    if (!g.isKingInCheck(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 43." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 43." << std::endl;
        ERROR_COUNT++;
    }
}

// Move applier test routine.
void testMoveExec(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Executing moves
    std::cout << "    Move applier:" << std::endl;
    testSnap.state = defaultGameState;
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    g.revertState(testSnap);
    g.applyMove(Coord{6, 4}, Coord{4, 4});
    // General test + en passant
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3")
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    g.revertState(testSnap);
    g.makeMove(Coord{6, 4}, Coord{4, 4});
    // Testing if makeMove() has same result
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3")
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
    g.undo();
    // Testing if undo() works after makeMove()
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq -")
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for castling queen side for white
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR");
    g.revertState(testSnap);
    g.applyMove(Coord{7, 4}, Coord{7, 2});
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/2KR1BNR b kq -")
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }
    // King side
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQK2R");
    g.revertState(testSnap);
    g.applyMove(Coord{7, 4}, Coord{7, 6});
    if (FEN_notation_generator(g.getSnap()) == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQ1RK1 b kq -")
    {
        std::cout << "        SUCCESS on case 5." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 5." << std::endl;
        ERROR_COUNT++;
    }
    // Testing for castling queen side for black
    testSnap.state.turn = Color::BLACK;
    testSnap.board = FEN_to_matrix("r3kbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    g.revertState(testSnap);
    g.applyMove(Coord{0, 4}, Coord{0, 2});
    if (FEN_notation_generator(g.getSnap()) == "2kr1bnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ -")
    {
        std::cout << "        SUCCESS on case 6." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 6." << std::endl;
        ERROR_COUNT++;
    }
    // King side
    testSnap.board = FEN_to_matrix("rnbqk2r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    g.revertState(testSnap);
    g.applyMove(Coord{0, 4}, Coord{0, 6});
    if (FEN_notation_generator(g.getSnap()) == "rnbq1rk1/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ -")
    {
        std::cout << "        SUCCESS on case 7." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 7." << std::endl;
        ERROR_COUNT++;
    }
}

// Has moves check test routine.
void testHasMoves(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Color has moves
    std::cout << "    Color has moves:" << std::endl;
    testSnap.board = FEN_to_matrix("8/8/8/8/8/8/8/8");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    if (!g.hasMoves(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    if (!g.hasMoves(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.board = FEN_to_matrix("4k3/8/8/8/8/8/8/4K3");
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    if (g.hasMoves(Color::WHITE))
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.state.turn = Color::BLACK;
    g.revertState(testSnap);
    if (g.hasMoves(Color::BLACK))
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }
}

// Stalemate condition check test routine.
void testStalemate(int &ERROR_COUNT, Game &g)
{
    SnapShot testSnap;
    // Stalemate
    std::cout << "    Stalemate by material:" << std::endl;
    testSnap.board = FEN_to_matrix("4k3/8/8/8/8/8/8/4K3");
    g.revertState(testSnap);
    if (g.staleMateByMaterial())
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    g.revertState(testSnap);
    if (!g.staleMateByMaterial())
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }
}

// Main Game class test routine function.
int runGameTests()
{
    int ERROR_COUNT = 0;
    std::cout << "Game test routine:" << std::endl;
    Game g;
    std::cout << "    Game instance initialized." << std::endl;

    testSnapCopy(ERROR_COUNT, g);
    testPawnMoveGen(ERROR_COUNT, g);
    testKnightMoveGen(ERROR_COUNT, g);
    testRookMoveGen(ERROR_COUNT, g);
    testBishopMoveGen(ERROR_COUNT, g);
    testQueenMoveGen(ERROR_COUNT, g);
    testKingMoveGen(ERROR_COUNT, g);
    testKingInCheck(ERROR_COUNT, g);
    testMoveExec(ERROR_COUNT, g);
    testHasMoves(ERROR_COUNT, g);
    testStalemate(ERROR_COUNT, g);

    return ERROR_COUNT;
}