#pragma once
#include <cstdint>

#include "types/Common.hpp"
#include "board/Board.hpp"

namespace Zobrist
{
    // Fingerprint key variables.
    extern uint64_t pieceKeys[6][2][64]; // pieceType, color, square
    extern uint64_t sideKey; // turn
    extern uint64_t enPassantKeys[9]; // 0-7 for files a-h, 8 for "none" (we won't use none)
    extern uint64_t castlingKeys[2][2]; // [0][0]=WK, [0][1]=WQ, [1][0]=BK, [1][1]=BQ

    // Must be called once at program start
    void init();

    // Full board + state hash (used for initial position and optionally for sanity checks)
    uint64_t computeHash(const Board &board, const GameState &state);

    // Returns fingerprint key of a piece at a position on the board.
    inline uint64_t getPieceKey(const Piece& p, const Coord& at)
    {
        const int squareIndex = at.row * 8 + at.col;
        return pieceKeys[static_cast<int>(p.t)][static_cast<int>(p.c)][squareIndex];
    }

    inline uint64_t getEnPassantKey(const Coord& enPassant)
    {
        return enPassant == Coord{8, 8} ? 0 : enPassantKeys[enPassant.col];
    }

    inline uint64_t getCastlingKey(const GameState &state)
    {
        uint64_t castlingKey = 0;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (state.castling[i][j])
                    castlingKey ^= castlingKeys[i][j];
            }
        }
        return castlingKey;
    }
}
