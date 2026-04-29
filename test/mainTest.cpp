#include "commonTest.hpp"
#include "chessUITest.hpp"
#include "boardTest.hpp"
#include "gameTest.hpp"
#include "engineTest.hpp"
#include <iostream>

// Code by André Santos.
int main()
{
    std::cout << "Starting test routine..." << std::endl;
    int ERROR_COUNT = 0;

    // Testing Common.hpp basic operations.
    ERROR_COUNT += runCommonTests();

    // Testing ChessUI view class core functionalities.
    ERROR_COUNT += runChessUITests();

    // Testing Board data class core functionalities.
    ERROR_COUNT += runBoardTests();

    // Testing Game controller class core functionalities.
    ERROR_COUNT += runGameTests();

    // Testing Engine class move generator validity.
    ERROR_COUNT += runEngineTests();

    std::cout << "================================" << std::endl;
    std::cout << "Test Summary: " << ERROR_COUNT << " error(s) out of 174 tests" << std::endl;
    if (ERROR_COUNT == 0)
        std::cout << "All systems nominal. You don't need to thank me." << std::endl;
    else
    {
        std::cout << "If multiple errors occurred on the same section, they may be related." << std::endl;
        std::cout << "The code tests cases from most basic operations (base for all others) to most complex operations." << std::endl;
    }
    std::cout << "================================" << std::endl;

    return ERROR_COUNT > 0 ? 1 : 0;
}
