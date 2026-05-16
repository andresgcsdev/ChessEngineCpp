#include <iostream>
#include <sstream>
#include <string>

#include "engine/Engine.hpp"
#include "fingerprint/Zobrist.hpp"
#include "game/Game.hpp"
#include "types/Common.hpp"

std::string coordToAlgebraic(const Coord c) {
    char file = 'a' + c.col;
    char rank = '8' - c.row;
    return {file, rank};
}

Coord algebraicToCoord(const std::string& s) {
    int col = s[0] - 'a';
    int row = 8 - (s[1] - '0');
    return {row, col};
}

int main() {
    Zobrist::init();

    std::string line;
    Game game;          // always holds the current position
    Engine engine;
    Color engineColor = Color::BLACK; // default, but will be set by the GUI

    while (std::getline(std::cin, line)) {
        if (line == "uci") {
            std::cout << "id name ChessEngineCpp v1.3" << std::endl;
            std::cout << "id author André Santos"<< std::endl;
            std::cout << "uciok"<< std::endl;
        }
        else if (line == "isready") {
            std::cout << "readyok"<< std::endl;
        }
        else if (line.rfind("position startpos", 0) == 0) {
            // Reset to start position
            game = Game();
            // If there are moves after "moves", apply them
            size_t pos = line.find("moves");
            if (pos != std::string::npos) {
                std::istringstream iss(line.substr(pos + 6));
                std::string moveStr;
                while (iss >> moveStr) {
                    Coord from = algebraicToCoord(moveStr.substr(0,2));
                    Coord to   = algebraicToCoord(moveStr.substr(2,2));
                    game.makeMove(from, to);
                }
            }
        }
        else if (line.rfind("go depth", 0) == 0) {
            int depth = std::stoi(line.substr(9)); // after "go depth "
            auto [from, to] = engine.getBestMove(game, game.getTurn(), depth);
            std::cout << "bestmove " << coordToAlgebraic(from) << coordToAlgebraic(to) << std::endl;
        }
        else if (line == "quit") {
            break;
        }
    }
    return 0;
}