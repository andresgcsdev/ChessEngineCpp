#include "Zobrist.hpp"
#include <random>

namespace Zobrist
{
    uint64_t pieceKeys[6][2][64];
    uint64_t sideKey;
    uint64_t enPassantKeys[9];
    uint64_t castlingKeys[2][2];
}

void Zobrist::init()
{
    // Setting up hashes for board fingerprint.
    std::mt19937_64 rng(10042007);

    // Populating with random for later hash.
    // 0 -> black, 1 -> white
    for (auto &[black, white]: pieceKeys)
    {
        for (int i = 0; i < 64; i++)
        {
            black[i] = rng();
            white[i] = rng();
        }
    }

    sideKey = rng();

    for (auto &x: enPassantKeys)
        x = rng();

    for (auto &[king, queen]: castlingKeys)
    {
        king = rng();
        queen = rng();
    }
}

uint64_t Zobrist::computeHash(const Board &board, const GameState &state)
{
    uint64_t hash = 0;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            const Piece p = board.getPiece(Coord{row, col});
            const int typeIndex = static_cast<int>(p.t);
            const int colorIndex = static_cast<int>(p.c);
            const int singleCoordIndex = row * 8 + col;
            hash ^= pieceKeys[typeIndex][colorIndex][singleCoordIndex];
        }
    }

    if (state.turn == Color::WHITE)
        hash ^= sideKey;

    if (state.enPassant.col < 8)
        hash ^= enPassantKeys[state.enPassant.col];

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (state.castling[i][j])
                hash ^= castlingKeys[i][j];

    return hash;
}
