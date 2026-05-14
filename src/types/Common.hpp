#pragma once
#include <array>
#include <cstdint>

enum class Color
{
    BLACK = 0,
    WHITE = 1
};

enum class PieceType
{
    PAWN = 0,
    KING = 1,
    QUEEN = 2,
    KNIGHT = 3,
    BISHOP = 4,
    ROOK = 5,
    BLANK = 6,
    ERROR = -1
};

struct Piece
{
    PieceType t = PieceType::BLANK;
    Color c = Color::BLACK;
    char id = 'b';
};

// All coordinates above 7 are out of bounds.
// The code uses {8, 8} as default for invalid or null coordinates.
struct Coord
{
    int row = 8, col = 8;
};

struct GameState
{
    Color turn = Color::WHITE;
    // [id: 0 = 1, 1 = 2][color: 0 = white, 1 = black]
    // id 1: Queen Side, id 2: King Side
    bool castling[2][2] = {{true, true}, {true, true}};
    Coord enPassant;
    uint64_t hash = 0;
};

// Game data struct.
// Used for do/undo moves.
struct SnapShot
{
    std::array<std::array<Piece, 8>, 8> board;
    GameState state;
    Coord blackKing;
    Coord whiteKing;
};

enum class MoveType
{
    QUIET = 0,
    CAPTURE = 1,
    ENPASSANT = 3,
    CASTLING_KS = 4,
    CASTLING_QS = 5,
    PROMOTION = 6
};

struct MoveInfo
{
    Coord from;
    Coord to;
    Piece captured;
    MoveType type = MoveType::QUIET;
};

// Cheaper version of SnapShot.
// Less overhead and memory consumption.
// More error-prone, less flexible.
struct CheapSnap
{
    MoveInfo lastMove;
    GameState state;
    Coord blackKing;
    Coord whiteKing;
};

// Transposition Table Entry.
struct TTEntry
{
    enum Flag : uint8_t {
        EMPTY = 0,
        EXACT = 1,
        ALPHA = 2,   // upper bound (fail-low)
        BETA  = 3    // lower bound (fail-high)
    };

    uint64_t hash = 0;
    int score = 0;
    int depth = 0;
    uint8_t flag = 0;
};

// Checks if Coordinate is not out of bounds.
inline bool isValidCoord(Coord c)
{
    return c.row <= 7 && c.col <= 7 && c.row >= 0 && c.col >= 0;
}

// Checks if move coordinates are valid.
// Notice it doesn't check for overall validity.
inline bool isValidMove(MoveInfo m)
{
    return isValidCoord(m.from) && isValidCoord(m.to);
}

inline bool operator==(const Coord &a, const Coord &b)
{
    return a.row == b.row && a.col == b.col;
}

inline bool operator!=(const Coord &a, const Coord &b)
{
    return !(a == b);
}

inline bool operator==(const Piece &a, const Piece &b)
{
    return a.t == b.t && a.c == b.c && a.id == b.id;
}
