#pragma once
#include <array>
#include <optional>
#include <vector>

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
    // Evaluates what is the next best move for the engine.
    // Returns an array of coords, index 0 is `from` and index 1 is `to`.
    // May return an Out-of-bounds coord array if the turn doesn't match the Engine color.
    static std::array<Coord, 2> getBestMove(Game game, const Color &selfColor, int depth);

private:

    static constexpr size_t TT_SIZE = 1 << 20; // 1.048.576 or 2^20 entries

    // Hash Table
    static std::array<TTEntry, TT_SIZE> TranspositionTable;

    // Store TTEntry into the hash table
    static void storeTT(const TTEntry &entry);

    // Get TTEntry based on hash.
    static std::optional<TTEntry> probeTT(uint64_t hash, int depth);

    // Minimax function for interation through moves.
    // Includes Alpha-Beta pruning.
    static int minimax(Game &game, int depth, int alpha, int beta, const Color &selfColor);

    // Returns a vector of all possible moves on the board.
    static std::vector<MoveInfo> getAllMoveInfo(Game &game, const Color &selfColor);

    // Sorts a MoveInfo vector by MVV-LVA.
    // Returns a sorted vector of the pair moves + their score.
    static std::vector<std::pair<MoveInfo, int> > sortMoveInfo(const std::vector<MoveInfo> &moves, const Board &board);

    static bool compareMoves(const std::pair<MoveInfo, int> &a, const std::pair<MoveInfo, int> &b)
    {
        return a.second > b.second;
    }
};
