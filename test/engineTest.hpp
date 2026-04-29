#pragma once

#include "core/Common.hpp"
#include "core/Engine.hpp"
#include "core/Game.hpp"

// Main Engine test routine.
int runEngineTests()
{
    SnapShot testSnap;
    Game g;
    int ERROR_COUNT = 0;

    std::cout << "Engine test routine:" << std::endl;
    Engine wai(Color::WHITE);
    Engine bai(Color::BLACK);
    std::cout << "    Engine instances initialized." << std::endl;
    testSnap.board = FEN_to_matrix("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
    testSnap.state = defaultGameState;
    testSnap.blackKing = defaultBlackKing;
    testSnap.whiteKing =defaultWhiteKing;
    g.revertState(testSnap);
    std::array<Coord, 2> testEngine = wai.getBestMove(g);
    if (g.makeMove(testEngine[0], testEngine[1]))
    {
        std::cout << "        SUCCESS on case 1." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 1." << std::endl;
        ERROR_COUNT++;
    }
    testEngine = bai.getBestMove(g);
    if (g.makeMove(testEngine[0], testEngine[1]))
    {
        std::cout << "        SUCCESS on case 2." << std::endl;
    }
    else
    {
        std::cout << "        ERROR on case 2." << std::endl;
        ERROR_COUNT++;
    }

    return ERROR_COUNT;
}