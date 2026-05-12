#include "Eval.hpp"
#include <climits>
#include <cmath>

namespace
{
    // All position value tables
    // Pawn
    static constexpr int pawnTable[8][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0}, // Rank 1
            {5, 5, 5, 5, 5, 5, 5, 5}, // Rank 2
            {10, 10, 10, 10, 10, 10, 10, 10}, // Rank 3
            {20, 20, 25, 30, 30, 25, 20, 20}, // Rank 4
            {30, 30, 35, 40, 40, 35, 30, 30}, // Rank 5
            {50, 50, 55, 60, 60, 55, 50, 50}, // Rank 6
            {100, 100, 100, 100, 100, 100, 100, 100}, // Rank 7 (close to promotion!)
            {0, 0, 0, 0, 0, 0, 0, 0} // Rank 8 (promote)
            };

    // Knight
    static constexpr int knightTable[8][8] = {
            {-50, -40, -30, -30, -30, -30, -40, -50},
            {-40, -20, 0, 0, 0, 0, -20, -40},
            {-30, 0, 10, 15, 15, 10, 0, -30},
            {-30, 5, 15, 20, 20, 15, 5, -30},
            {-30, 0, 15, 20, 20, 15, 0, -30},
            {-30, 5, 10, 15, 15, 10, 5, -30},
            {-40, -20, 0, 5, 5, 0, -20, -40},
            {-50, -40, -30, -30, -30, -30, -40, -50}
            };

    // Bishop
    static constexpr int bishopTable[8][8] = {
            {-20, -10, -10, -10, -10, -10, -10, -20},
            {-10, 0, 0, 0, 0, 0, 0, -10},
            {-10, 0, 5, 10, 10, 5, 0, -10},
            {-10, 5, 5, 10, 10, 5, 5, -10},
            {-10, 0, 10, 10, 10, 10, 0, -10},
            {-10, 10, 10, 10, 10, 10, 10, -10},
            {-10, 5, 0, 0, 0, 0, 5, -10},
            {-20, -10, -10, -10, -10, -10, -10, -20}
            };

    // Rook
    static constexpr int rookTable[8][8] = {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {5, 10, 10, 10, 10, 10, 10, 5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {-5, 0, 0, 0, 0, 0, 0, -5},
            {0, 0, 0, 5, 5, 0, 0, 0}
            };

    // Queen
    static constexpr int queenTable[8][8] = {
            {-20, -10, -10, -5, -5, -10, -10, -20},
            {-10, 0, 0, 0, 0, 0, 0, -10},
            {-10, 0, 5, 5, 5, 5, 0, -10},
            {-5, 0, 5, 5, 5, 5, 0, -5},
            {0, 0, 5, 5, 5, 5, 0, -5},
            {-10, 5, 5, 5, 5, 5, 0, -10},
            {-10, 0, 5, 0, 0, 0, 0, -10},
            {-20, -10, -10, -5, -5, -10, -10, -20}
            };

    // King (opening/midgame)
    static constexpr int kingTable[8][8] = {
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-20, -30, -30, -40, -40, -30, -30, -20},
            {-10, -20, -20, -20, -20, -20, -20, -10},
            {20, 20, 0, 0, 0, 0, 20, 20},
            {20, 30, 10, 0, 0, 10, 30, 20}
            };

    // King (endgame)
    static constexpr int kingEndgameTable[8][8] = {
            {-50, -40, -30, -20, -20, -30, -40, -50},
            {-40, -20, 0, 10, 10, 0, -20, -40},
            {-30, 0, 20, 30, 30, 20, 0, -30},
            {-20, 10, 30, 40, 40, 30, 10, -20},
            {-20, 10, 30, 40, 40, 30, 10, -20},
            {-30, 0, 20, 30, 30, 20, 0, -30},
            {-40, -20, 0, 10, 10, 0, -20, -40},
            {-50, -40, -30, -20, -20, -30, -40, -50}
            };

    //Piece Values
    static constexpr int PAWN_VALUE = 100;
    static constexpr int KNIGHT_VALUE = 300;
    static constexpr int BISHOP_VALUE = 300;
    static constexpr int ROOK_VALUE = 500;
    static constexpr int QUEEN_VALUE = 900;
}

bool Eval::isEndgame(const Game &game)
{
    // Count non-pawn, non-king material
    int material = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Piece p = game.getBoard().getPiece(Coord{i, j});
            switch (p.t)
            {
                case PieceType::QUEEN:
                    material += 900;
                    break;
                case PieceType::ROOK:
                    material += 500;
                    break;
                case PieceType::BISHOP:
                case PieceType::KNIGHT:
                    material += 300;
                    break;
                default:
                    break;
            }
        }
    }

    // Endgame if material is low (no queens, few pieces)
    return material < 1000;
}

int Eval::evaluate(Game &game, const Color &selfColor)
{
    if (!game.hasMoves(game.getTurn()))
    {
        if (game.isKingInCheck(game.getTurn()))
            // Checkmate
            // Always avoids getting checkmated, always goes for checkmate on the enemy.
            return game.getTurn() == selfColor ? INT_MIN : INT_MAX;
        // Stalemate
        // Always discourage Stalemate, but will go for it if it's the only option
        return -50;
    }
    int score = 0;
    int whiteMultiplr = selfColor == Color::WHITE ? 1 : -1;
    for (int row = 0; row < 8; row++)
    {
        for (int col = 0; col < 8; col++)
        {
            Coord from = Coord{row, col};
            Piece p = game.getBoard().getPiece(from);
            if (p.t == PieceType::BLANK)
                continue;
            int val = 0;
            int fixedRow = p.c == Color::WHITE ? 7 - row : row;
            switch (p.t)
            {
                case PieceType::PAWN:

                    val += PAWN_VALUE;
                    val += pawnTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;

                case PieceType::ROOK:

                    val += ROOK_VALUE;
                    val += rookTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;
                case PieceType::KNIGHT:

                    val += KNIGHT_VALUE;
                    val += knightTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;
                case PieceType::BISHOP:

                    val += BISHOP_VALUE;
                    val += bishopTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;
                case PieceType::QUEEN:

                    val += QUEEN_VALUE;
                    val += queenTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;
                case PieceType::KING:
                    if (isEndgame(game))
                        val += kingEndgameTable[fixedRow][col];
                    else
                        val += kingTable[fixedRow][col];

                    if (p.c == Color::WHITE)
                        val *= whiteMultiplr;
                    else
                        val *= -whiteMultiplr;
                    break;
                default:
                    break;
            }

            score += val;
        }
    }

    return score;
}
