#include <array>
#include <climits>
#include <algorithm>
#include "../game/Game.hpp"
#include "../board/Board.hpp"
#include "../types/Common.hpp"
#include "../engine/Engine.hpp"

#include <optional>

#include "Eval.hpp"
#include "movegen/MoveGenerator.hpp"

std::array<Coord, 2> Engine::getBestMove(Game game, const Color &selfColor, int depth)
{
    if (game.getTurn() != selfColor)
        return {Coord{8, 8}, Coord{8, 8}};

    if (depth <= 0)
        depth = 1;
    if (depth >= 6)
        depth = 5;

    int max = INT_MIN;
    Coord fromMax = Coord{8, 8};
    Coord toMax = Coord{8, 8};

    std::vector<std::pair<MoveInfo, int> > scoredMoves = sortMoveInfo(getAllMoveInfo(game, game.getTurn()), game.getBoard());
    for (auto &[m, _]: scoredMoves)
    {
        game.applyMove(m.from, m.to);
        CheapSnap snap = game.getCheapSnap();
        int minimaxVal = minimax(game, depth, INT_MIN, INT_MAX, selfColor);
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
    if (!game.hasMoves(game.getTurn()))
    {
        if (game.isKingInCheck(game.getTurn()))
        {
            // Checkmate
            // Always avoids getting checkmated, always goes for checkmate on the enemy.
            const int mateValue = Eval::MATE_VAL - depth;
            return game.getTurn() == selfColor ? -mateValue : mateValue;
        }
        // Stalemate
        // Always discourage Stalemate, but will go for it if it's the only option
        return -50;
    }

    if (depth == 0) return Eval::evaluate(game, selfColor);

    const uint64_t nodeHash = game.getCurrentState().hash;
    const int alphaOrig = alpha;

    // TT lookup for the current node
    if (const auto entry = probeTT(nodeHash, depth))
    {
        if (entry->flag == TTEntry::EXACT) return entry->score;
        if (entry->flag == TTEntry::ALPHA && entry->score <= alpha) return alpha;
        if (entry->flag == TTEntry::BETA && entry->score >= beta) return beta;
    }

    const bool isMaximizing = (game.getTurn() == selfColor);
    int bestScore = isMaximizing ? INT_MIN : INT_MAX;
    auto moves = sortMoveInfo(getAllMoveInfo(game, game.getTurn()), game.getBoard());

    for (auto &[m, _]: moves)
    {
        game.applyMove(m.from, m.to);
        CheapSnap snap = game.getCheapSnap();
        const uint64_t childHash = game.getCurrentState().hash;

        int score;
        bool usedTT = false;

        if (const auto entry = probeTT(childHash, depth - 1))
        {
            if (entry->flag == TTEntry::EXACT || entry->flag == TTEntry::ALPHA && entry->score <= alpha)
            {
                score = entry->score;
                usedTT = true;
            }
            else if (entry->flag == TTEntry::BETA && entry->score >= beta)
            {
                score = beta;
                usedTT = true;
                game.revertState(snap);
                bestScore = beta;
                break; // cutoff
            }
        }

        if (!usedTT)
            score = minimax(game, depth - 1, alpha, beta, selfColor);

        game.revertState(snap);

        if (isMaximizing)
        {
            bestScore = std::max(bestScore, score);
            alpha = std::max(alpha, bestScore);
        }
        else
        {
            bestScore = std::min(bestScore, score);
            beta = std::min(beta, bestScore);
        }
        if (beta <= alpha) break;
    }

    // Store the result for this node
    TTEntry::Flag flag;
    if (bestScore <= alphaOrig) flag = TTEntry::ALPHA;
    else if (bestScore >= beta) flag = TTEntry::BETA;
    else flag = TTEntry::EXACT;
    storeTT({nodeHash, bestScore, depth, flag});

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
        scored.emplace_back(m, Eval::moveScore(m, board));

    std::sort(scored.begin(), scored.end(), compareMoves);

    return scored;
}

std::array<TTEntry, Engine::TT_SIZE> Engine::TranspositionTable = {};

void Engine::storeTT(const TTEntry &entry)
{
    const size_t index = entry.hash & (TT_SIZE - 1);
    TTEntry &existing = TranspositionTable[index];
    if (existing.hash == entry.hash && existing.depth > entry.depth)
    {
        return; // don't overwrite a deeper entry for the same position
    }
    existing = entry; // replace otherwise
}

std::optional<TTEntry> Engine::probeTT(const uint64_t hash, const int depth)
{
    const size_t index = hash & (TT_SIZE - 1);
    const TTEntry &entry = TranspositionTable[index];
    if (entry.hash == hash && entry.depth >= depth)
    {
        return entry;
    }
    return std::nullopt; // no usable entry
}
