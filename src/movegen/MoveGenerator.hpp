#pragma once

#include "types/Common.hpp"
#include "board/Board.hpp"
#include <array>

namespace MoveGenerator
{
    // Calculates possible moves based on given position + piece.
    // Doesn't account for possible self-king checks.
    std::array<Coord, 27> pseudoLegalMoves(
        const Board &board,
        const GameState &state,
        const Coord &at
    );

    // Calculates if the given square is a possible move for enemy pieces.
    bool isSquareAttackedBy(
        const Board &board,
        const Coord &at,
        Color attacker
    );

    // Classifies the possible move based on the given piece displacement.
    MoveInfo classifyMove(
        const Board& board,
        const GameState& gameState,
        Coord from,
        Coord to
    );
}
