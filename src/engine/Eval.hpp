#pragma once
#include "game/Game.hpp"
#include "types/Common.hpp"

namespace Eval
{
    // Current implementation defines endgame when global material is low (both self and the opponent's).
    bool isEndgame(const Game &game);

    // Evaluation function for current board state.
    // Evaluates for material difference and piece positions.
    // Game& is non-const because hasMoves() internally tests moves (apply/revert).
    // The position is unchanged on return.
    int evaluate(
        Game &game,
        const Color &selfColor
    );

    // Evaluates a move based on MVV-LVA
    int moveScore(
        const MoveInfo &move,
        const Board &board
        );
}
