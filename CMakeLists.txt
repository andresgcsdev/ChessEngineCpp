cmake_minimum_required(VERSION 3.10)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main game executable
add_executable(chess
    src/main.cpp
    src/core/Board.cpp
    src/core/Game.cpp
    src/ui/ChessUI.cpp
    src/core/Engine.cpp
)

target_include_directories(chess PRIVATE src)

# Test executable
add_executable(chess_test
        test/mainTest.cpp
        src/core/Board.cpp
        src/core/Game.cpp
        src/core/Engine.cpp
        src/ui/ChessUI.cpp
        test/commonTest.hpp
        test/chessUITest.hpp
        test/boardTest.hpp
        test/testHelper.hpp
        test/gameTest.hpp
        test/engineTest.hpp
)
target_include_directories(chess_test PRIVATE src)