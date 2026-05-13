#include <array>
#include <climits>
#include <algorithm>
#include "../game/Game.hpp"
#include "../board/Board.hpp"
#include "../types/Common.hpp"
#include "../engine/Engine.hpp"
#include "Eval.hpp"
#include "movegen/MoveGenerator.hpp"

std::array<Coord, 2> Engine::getBestMove(Game game, const Color &selfColor)
{
    if (game.getTurn() != selfColor)
        return {Coord{8, 8}, Coord{8, 8}};

    int max = INT_MIN;
    Coord fromMax = Coord{8, 8};
    Coord toMax = Coord{8, 8};

    std::vector<std::pair<MoveInfo, int> > scoredMoves = sortMoveInfo(getAllMoveInfo(game, game.getTurn()), game.getBoard());
    for (auto &[m, _]: scoredMoves)
    {
        game.applyMove(m.from, m.to);
        CheapSnap snap = game.getCheapSnap();
        int minimaxVal = minimax(game, SEARCH_DEPTH, INT_MIN, INT_MAX, selfColor);
        game.revertState(snap);

        if (minimaxVal > max)
        {
            max = minimaxVal;
            fromMax = m.from;
            toMax = m.to;
        }
    }

    return
            {fromMax, toMax};
}

int Engine::minimax(Game &game, int depth, int alpha, int beta, const Color &selfColor)
{
    if (depth == 0)
        return Eval::evaluate(game, selfColor);

    bool isMaximizing = game.getTurn() == selfColor;
    int bestScore = isMaximizing ? INT_MIN : INT_MAX;
    std::vector<std::pair<MoveInfo, int> > scoredMoves = sortMoveInfo(getAllMoveInfo(game, game.getTurn()), game.getBoard());
    for (auto &[m, _]: scoredMoves)
    {
        game.applyMove(m.from, m.to);
        CheapSnap snap = game.getCheapSnap();
        int score = minimax(game, depth - 1, alpha, beta, selfColor);
        game.revertState(snap);
        if (isMaximizing)
        {
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
            if (beta <= alpha)
                break;
        }
        else
        {
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
            if (beta <= alpha)
                break;
        }
    }
    return bestScore;
}

std::vector<MoveInfo> Engine::getAllMoveInfo(Game &game, const Color &selfColor)
{
    std::vector<MoveInfo> allMoves;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            const Piece currentPiece = game.getBoard().getPiece(Coord{row, col});
            if (currentPiece.t == PieceType::BLANK || currentPiece.c != selfColor)
                continue;

            std::array<Coord, 27> posb = game.possibleMoves(Coord{row, col});
            for (int k = 0; k < 27; k++)
            {
                if (isValidCoord(posb[k]))
                {
                    allMoves.push_back(MoveGenerator::classifyMove(game.getBoard(), game.getCurrentState(), Coord{row, col}, posb[k]));
                }
            }
        }
    }

    return allMoves;
}


std::vector<std::pair<MoveInfo, int> > Engine::sortMoveInfo(const std::vector<MoveInfo> &moves, const Board &board)
{
    std::vector<std::pair<MoveInfo, int> > scored;
    for (auto &m: moves)
        scored.push_back(std::pair<MoveInfo, int>{m, scored.size()});

    std::sort(scored.begin(), scored.end(), compareMoves);

    return scored;
}
