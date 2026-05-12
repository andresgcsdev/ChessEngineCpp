#include <array>
#include <climits>
#include <cmath>
#include "../game/Game.hpp"
#include "../board/Board.hpp"
#include "../types/Common.hpp"
#include "../engine/Engine.hpp"
#include "Eval.hpp"

Engine::Engine(Color c)
{
    selfColor = c;
}

std::array<Coord, 2> Engine::getBestMove(Game game)
{
    if (game.getTurn() != selfColor)
        return {Coord{8, 8}, Coord{8, 8}};

    int max = INT_MIN;
    Coord fromMax = Coord{8, 8};
    Coord toMax = Coord{8, 8};

    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Coord currentPiece = Coord{row, col};
            Piece p = game.getBoard().getPiece(currentPiece);

            if (p.t != PieceType::BLANK && p.c == selfColor)
            {
                std::array<Coord, 27> posb = game.possibleMoves(currentPiece);
                for (int k = 0; k < 27; k++)
                {
                    if (posb[k] == Coord{8, 8})
                        break;

                    game.applyMove(currentPiece, posb[k]);
                    CheapSnap snap = game.getCheapSnap();
                    int minimaxVal = minimax(game, SEARCH_DEPTH, INT_MIN, INT_MAX);
                    game.revertState(snap);

                    if (minimaxVal > max)
                    {
                        max = minimaxVal;
                        fromMax = currentPiece;
                        toMax = posb[k];
                    }
                }
            }
        }
    }

    return {fromMax, toMax};
}

int Engine::minimax(Game &game, int depth, int alpha, int beta)
{
    if (depth == 0)
        return Eval::evaluate(game, selfColor);

    bool isMaximizing = game.getTurn() == selfColor;
    int bestScore = isMaximizing ? INT_MIN : INT_MAX;
    bool alphaBetaCutoff = false;
    for (int row = 0; row < 8 && !alphaBetaCutoff; row++)
    {
        for (int col = 0; col < 8 && !alphaBetaCutoff; col++)
        {
            Coord from = Coord{row, col};
            Piece p = game.getBoard().getPiece(from);
            if (p.t == PieceType::BLANK || p.c != game.getTurn())
                continue;

            std::array<Coord, 27> posb = game.possibleMoves(from);
            for (int k = 0; k < 27; k++)
            {
                if (posb[k] == Coord{8, 8})
                    break;

                game.applyMove(from, posb[k]);
                CheapSnap snap = game.getCheapSnap();
                int score = minimax(game, depth - 1, alpha, beta);
                game.revertState(snap);

                if (isMaximizing)
                {
                    bestScore = std::max(bestScore, score);
                    alpha = std::max(alpha, bestScore);
                    if (beta <= alpha)
                    {
                        alphaBetaCutoff = true;
                        break;
                    }
                }
                else
                {
                    bestScore = std::min(bestScore, score);
                    beta = std::min(beta, bestScore);
                    if (beta <= alpha)
                    {
                        alphaBetaCutoff = true;
                        break;
                    }
                }
            }
        }
    }
    return bestScore;
}
