#pragma once
#include <array>
#include "../game/Game.hpp"
#include "../board/Board.hpp"
#include "../types/Common.hpp"

// AI Engine (Search & Evaluation)
// Analyzes chess positions and determines optimal moves.
// Uses minimax search with position evaluation to find the best move at a given depth.
// Does not modify game state; returns recommended moves for Game to execute.
class Engine
{
public:
    // `Color c` is the color that the Engine will evaluate for (positive multipliers for evaluation).
    Engine(Color c);

    // Evaluates what is the next best move for the engine.
    // Returns an array of coords, index 0 is `from` and index 1 is `to`.
    // May return an Out-of-bounds coord array if the turn doesn't match the Engine color.
    std::array<Coord, 2> getBestMove(Game game);

private:
    static constexpr int SEARCH_DEPTH = 4;
    Color selfColor;

    // Minimax function for interation through moves.
    // Includes Alpha-Beta pruning.
    int minimax(Game &game, int depth, int alpha, int beta);
};
