#pragma once

#include "../src/types/Common.hpp"
#include "../src/engine/Engine.hpp"
#include "../src/game/Game.hpp"

// Main Engine test routine.
int runEngineTests()
{
    SnapShot testSnap;
    Game g;
    int ERROR_COUNT = 0;

    // Basic move function test.
    std::cout << "Engine test routine:" << std::endl;
    std::cout << "    Engine instances initialized." << std::endl;
    std::cout << "    Basic move routine:" << std::endl;
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    testSnap.state = defaultGameState;
    testSnap.blackKing = defaultBlackKing;
    testSnap.whiteKing = defaultWhiteKing;
    g.revertState(testSnap);
    std::array<Coord, 2> testEngine;
    testEngine = Engine::getBestMove(g, Color::WHITE, 3);
    if (g.makeMove(testEngine[0], testEngine[1]))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    testEngine = Engine::getBestMove(g, Color::BLACK, 3);
    if (g.makeMove(testEngine[0], testEngine[1]))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    // Checkmate finding test.
    std::cout << "    Checkmate routine:" << std::endl;
    testSnap.board = FEN_to_matrix("1k6/8/1K6/8/8/8/8/3Q4");
    testSnap.state = GameState{Color::WHITE, {{false, false}, {false, false}}, {8, 8}};
    testSnap.blackKing = {0, 1};
    testSnap.whiteKing = {2, 1};
    g.revertState(testSnap);
    testEngine = Engine::getBestMove(g, Color::WHITE, 3);
    constexpr Coord queen = {7, 3};
    constexpr Coord queenMate = {0, 3};
    if (testEngine[0] == queen && testEngine[1] == queenMate)
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.board = FEN_to_matrix("1K6/8/1k6/8/8/8/8/3q4");
    testSnap.state = GameState{Color::BLACK, {{false, false}, {false, false}}, {8, 8}};
    testSnap.blackKing = {2, 1};
    testSnap.whiteKing = {0, 1};
    g.revertState(testSnap);
    testEngine = Engine::getBestMove(g, Color::BLACK, 3);
    if (testEngine[0] == queen && testEngine[1] == queenMate)
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    // Mate with rook.
    testSnap.board = FEN_to_matrix("1k6/ppp5/8/8/8/8/8/3K3R");
    testSnap.blackKing = {0, 1};
    testSnap.whiteKing = {7, 3};
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    testEngine = Engine::getBestMove(g, Color::WHITE, 3);
    constexpr Coord rook = {7, 7};
    constexpr Coord rookMate = {0, 7};
    if (testEngine[0] == rook && testEngine[1] == rookMate)
    {
        std::cout << "        SUCCESS on case 3." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 3." << std::endl;
        ERROR_COUNT++;
    }

    testSnap.board = FEN_to_matrix("kr6/pp6/4N3/8/8/8/8/3K4");
    testSnap.blackKing = {0, 0};
    testSnap.whiteKing = {7, 3};
    testSnap.state.turn = Color::WHITE;
    g.revertState(testSnap);
    testEngine = Engine::getBestMove(g, Color::WHITE, 3);
    constexpr Coord knight = {2, 4};
    constexpr Coord knightMate = {1, 2};
    if (testEngine[0] == knight && testEngine[1] == knightMate)
    {
        std::cout << "        SUCCESS on case 4." << std::endl;
    } else
    {
        std::cout << "        ERROR on case 4." << std::endl;
        ERROR_COUNT++;
    }


    return ERROR_COUNT;
}
