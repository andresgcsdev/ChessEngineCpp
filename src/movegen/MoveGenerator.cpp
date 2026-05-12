#include "../movegen/MoveGenerator.hpp"
#include "../board/Board.hpp"

namespace
{
    std::array<Coord, 27> pawnMoves(const Board &board, const GameState &gameState, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece pawn = board.getPiece(p);
        if (pawn.t != PieceType::PAWN)
            return arr;

        int moves[4][2] = {{1, 0}, {2, 0}, {1, 1}, {1, -1}};
        if (pawn.c == Color::WHITE)
        {
            for (auto &move: moves)
            {
                move[0] *= -1;
                move[1] *= -1;
            }
        }

        int k = 0;
        for (int i = 0; i < 4; i++)
        {
            Coord currentMove = Coord{p.row + moves[i][0], p.col + moves[i][1]};
            Piece to = board.getPiece(currentMove);
            if (isValidCoord(currentMove))
            {
                // Diagonals
                if (moves[i][1] == 1 || moves[i][1] == -1)
                {
                    if ((to.c != pawn.c &&
                         to.t != PieceType::BLANK))
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                    // En passant check, making sure it is only allowed when the space is blank and when it is the piece's turn (to avoid misscalculations in finding the control areas)
                    if ((gameState.enPassant == currentMove &&
                         to.t == PieceType::BLANK &&
                         gameState.turn == pawn.c))
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                }
                // Move 1 to the front
                else if (moves[i][0] == 1 || moves[i][0] == -1)
                {
                    if (to.t == PieceType::BLANK)
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                }
                // Move 2 to the front
                else if (moves[i][0] == 2 || moves[i][0] == -2)
                {
                    if (pawn.c == Color::BLACK &&
                        to.t == PieceType::BLANK &&
                        board.getPiece(Coord{p.row + 1, p.col}).t == PieceType::BLANK &&
                        p.row == 1)
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                    if (pawn.c == Color::WHITE &&
                        to.t == PieceType::BLANK &&
                        board.getPiece(Coord{p.row - 1, p.col}).t == PieceType::BLANK &&
                        p.row == 6)
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                }
            }
        }

        return arr;
    }

    std::array<Coord, 27> knightMoves(const Board &board, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece knight = board.getPiece(p);
        if (knight.t != PieceType::KNIGHT)
            return arr;

        // "L" moves for all directions
        int moves[8][2] = {{-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {1, -2}, {1, 2}, {-1, -2}, {-1, 2}};

        int k = 0;
        for (int i = 0; i < 8; i++)
        {
            Coord currentMove = Coord{p.row + moves[i][0], p.col + moves[i][1]};
            Piece to = board.getPiece(currentMove);
            if (isValidCoord(currentMove))
            {
                if (to.t == PieceType::BLANK || to.c != knight.c)
                {
                    arr[k] = currentMove;
                    k++;
                }
            }
        }

        return arr;
    }

    std::array<Coord, 27> rookMoves(const Board &board, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece rook = board.getPiece(p);
        if (rook.t != PieceType::ROOK && rook.t != PieceType::QUEEN)
            return arr;

        int moves[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int k = 0;
        for (int i = 0; i < 4; i++)
        {
            // Slide orthogonally in each direction until hitting a piece or board edge
            for (int j = 1; j < 8; j++)
            {
                Coord currentMove = Coord{p.row + (j * moves[i][0]), p.col + (j * moves[i][1])};
                Piece to = board.getPiece(currentMove);
                if (isValidCoord(currentMove))
                {
                    if (to.t == PieceType::BLANK)
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                    else if (to.c != rook.c)
                    {
                        arr[k] = currentMove;
                        k++;
                        break;
                    }
                    else
                        break;
                }
                else
                    break;
            }
        }

        return arr;
    }

    std::array<Coord, 27> bishopMoves(const Board &board, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece bishop = board.getPiece(p);
        if (bishop.t != PieceType::BISHOP && bishop.t != PieceType::QUEEN)
            return arr;

        int moves[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
        int k = 0;
        for (int i = 0; i < 4; i++)
        {
            // Slide diagonally in each direction until hitting a piece or board edge
            for (int j = 1; j < 8; j++)
            {
                Coord currentMove = Coord{p.row + (j * moves[i][0]), p.col + (j * moves[i][1])};
                Piece to = board.getPiece(currentMove);
                if (isValidCoord(currentMove))
                {
                    if (to.t == PieceType::BLANK)
                    {
                        arr[k] = currentMove;
                        k++;
                    }
                    else if (to.c != bishop.c)
                    {
                        arr[k] = currentMove;
                        k++;
                        break;
                    }
                    else
                        break;
                }
                else
                    break;
            }
        }

        return arr;
    }

    std::array<Coord, 27> queenMoves(const Board &board, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece queen = board.getPiece(p);
        if (queen.t != PieceType::QUEEN)
            return arr;

        // Perpendicular moves
        arr = rookMoves(board, p);
        int k = 0;
        for (int i = 0; i < 27; i++)
        {
            if (arr[i] == Coord{8, 8})
            {
                k = i;
                break;
            }
        }

        // Diagonal moves
        std::array<Coord, 27> bishop = bishopMoves(board, p);
        for (int i = k; i < 27; i++)
        {
            arr[i] = bishop[i - k];
        }

        return arr;
    }

    std::array<Coord, 27> kingMoves(const Board &board, const GameState &gameState, Coord p)
    {
        std::array<Coord, 27> arr = {};
        // Setting sentinel values - Remember to check
        for (int i = 0; i < 27; i++)
            arr[i] = Coord{8, 8};

        Piece king = board.getPiece(p);
        if (king.t != PieceType::KING)
            return arr;

        // Areas around the king's square
        int moves[8][2] = {{1, 0}, {1, 1}, {1, -1}, {-1, 0}, {-1, 1}, {-1, -1}, {0, 1}, {0, -1}};

        int k = 0;
        for (int i = 0; i < 8; i++)
        {
            Coord currentMove = Coord{p.row + moves[i][0], p.col + moves[i][1]};
            Piece to = board.getPiece(currentMove);
            if (isValidCoord(currentMove))
            {
                if ((to.t == PieceType::BLANK || to.c != king.c))
                {
                    arr[k] = currentMove;
                    k++;
                }
            }
        }

        int colorIndex = king.c == Color::BLACK ? 1 : 0;
        // Castling queen side
        if (gameState.castling[0][colorIndex])
        {
            Coord currentMove = Coord{p.row, p.col - 2};
            Coord id1RookToKingSpaces[2] = {Coord{p.row, p.col - 1}, Coord{p.row, p.col - 2}};
            if (isValidCoord(id1RookToKingSpaces[0]) && isValidCoord(id1RookToKingSpaces[1]))
            {
                bool allBlankAndNotInControl = true;
                for (int i = 0; i < 2; i++)
                {
                    if (board.getPiece(id1RookToKingSpaces[i]).t != PieceType::BLANK ||
                        MoveGenerator::isSquareAttackedBy(board, id1RookToKingSpaces[i], king.c == Color::BLACK ? Color::WHITE : Color::BLACK))
                    {
                        allBlankAndNotInControl = false;
                        break;
                    }
                }
                if (allBlankAndNotInControl)
                {
                    arr[k] = currentMove;
                    k++;
                }
            }
        }
        // Castling king side
        if (gameState.castling[1][colorIndex])
        {
            Coord currentMove = Coord{p.row, p.col + 2};
            Coord id1RookToKingSpaces[2] = {Coord{p.row, p.col + 1}, Coord{p.row, p.col + 2}};
            if (isValidCoord(id1RookToKingSpaces[0]) && isValidCoord(id1RookToKingSpaces[1]))
            {
                bool allBlankAndNotInControl = true;
                for (int i = 0; i < 2; i++)
                {
                    if (board.getPiece(id1RookToKingSpaces[i]).t != PieceType::BLANK ||
                        MoveGenerator::isSquareAttackedBy(board, id1RookToKingSpaces[i], king.c == Color::BLACK ? Color::WHITE : Color::BLACK))
                    {
                        allBlankAndNotInControl = false;
                        break;
                    }
                }
                if (allBlankAndNotInControl)
                {
                    arr[k] = currentMove;
                }
            }
        }

        return arr;
    }
}

std::array<Coord, 27> MoveGenerator::pseudoLegalMoves(const Board &board, const GameState &state, const Coord &at)
{
    Piece p = board.getPiece(at);

    switch (p.t)
    {
        case PieceType::PAWN: return pawnMoves(board, state, at);
        case PieceType::ROOK: return rookMoves(board, at);
        case PieceType::KNIGHT: return knightMoves(board, at);
        case PieceType::BISHOP: return bishopMoves(board, at);
        case PieceType::QUEEN: return queenMoves(board, at);
        case PieceType::KING: return kingMoves(board, state, at);
        default: return {};
    }
}

bool MoveGenerator::isSquareAttackedBy(const Board &board, const Coord &at, Color attacker)
{
    // Check for Knights
    int knightMoves[8][2] = {{-2, 1}, {-2, -1}, {2, 1}, {2, -1}, {1, -2}, {1, 2}, {-1, -2}, {-1, 2}};
    for (auto &m: knightMoves)
    {
        Coord target = {at.row + m[0], at.col + m[1]};
        if (isValidCoord(target))
        {
            Piece p = board.getPiece(target);
            if (p.t == PieceType::KNIGHT && p.c == attacker)
                return true;
        }
    }

    // Check for Sliders (Rooks, Bishops, Queens)
    int directions[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int i = 0; i < 8; i++)
    {
        for (int d = 1; d < 8; d++)
        {
            Coord target = {at.row + (directions[i][0] * d), at.col + (directions[i][1] * d)};
            if (!isValidCoord(target))
                break;

            Piece p = board.getPiece(target);
            if (p.t != PieceType::BLANK)
            {
                if (p.c == attacker)
                {
                    // Check if piece type matches the direction (Orthogonal vs Diagonal)
                    if (i < 4)
                    {
                        // Orthogonal (Rook/Queen)
                        if (p.t == PieceType::ROOK || p.t == PieceType::QUEEN)
                            return true;
                    }
                    else
                    {
                        // Diagonal (Bishop/Queen)
                        if (p.t == PieceType::BISHOP || p.t == PieceType::QUEEN)
                            return true;
                    }
                }
                break;
            }
        }
    }

    // Check for Pawns
    // If we are looking for a WHITE attacker, the pawn must be BELOW the square (row - 1)
    // If we are looking for a BLACK attacker, the pawn must be ABOVE the square (row + 1)
    int pawnRowDiff = attacker == Color::WHITE ? 1 : -1;
    int pawnCols[2] = {at.col - 1, at.col + 1};

    for (int c: pawnCols)
    {
        Coord target = {at.row + pawnRowDiff, c};
        if (isValidCoord(target))
        {
            Piece p = board.getPiece(target);
            if (p.t == PieceType::PAWN && p.c == attacker)
                return true;
        }
    }

    // 4. Check for King (to prevent Kings from standing next to each other)
    int kingMoves[8][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto &m: kingMoves)
    {
        Coord target = {at.row + m[0], at.col + m[1]};
        if (isValidCoord(target))
        {
            Piece p = board.getPiece(target);
            if (p.t == PieceType::KING && p.c == attacker)
                return true;
        }
    }

    return false;
}
