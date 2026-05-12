#include <array>
#include <stack>
#include "../game/Game.hpp"
#include "../board/Board.hpp"
#include "../types/Common.hpp"
#include "../movegen/MoveGenerator.hpp"

Game::Game()
{
    gameState.turn = Color::WHITE;
    gameState.enPassant = Coord{8, 8};
    blackKing = Coord{0, 4};
    whiteKing = Coord{7, 4};

    std::array<std::array<Piece, 8>, 8> matrix;

    // Row 0 (White back rank)
    matrix[7][0] = Piece{PieceType::ROOK, Color::WHITE, '1'};
    matrix[7][1] = Piece{PieceType::KNIGHT, Color::WHITE, '1'};
    matrix[7][2] = Piece{PieceType::BISHOP, Color::WHITE, '1'};
    matrix[7][3] = Piece{PieceType::QUEEN, Color::WHITE, '0'};
    matrix[7][4] = Piece{PieceType::KING, Color::WHITE, '0'};
    matrix[7][5] = Piece{PieceType::BISHOP, Color::WHITE, '2'};
    matrix[7][6] = Piece{PieceType::KNIGHT, Color::WHITE, '2'};
    matrix[7][7] = Piece{PieceType::ROOK, Color::WHITE, '2'};

    // Row 1 (White pawns)
    for (int col = 0; col < 8; col++)
    {
        matrix[6][col] = Piece{PieceType::PAWN, Color::WHITE, static_cast<char>('1' + col)};
    }

    // Row 7 (Black back rank)
    matrix[0][0] = Piece{PieceType::ROOK, Color::BLACK, '1'};
    matrix[0][1] = Piece{PieceType::KNIGHT, Color::BLACK, '1'};
    matrix[0][2] = Piece{PieceType::BISHOP, Color::BLACK, '1'};
    matrix[0][3] = Piece{PieceType::QUEEN, Color::BLACK, '0'};
    matrix[0][4] = Piece{PieceType::KING, Color::BLACK, '0'};
    matrix[0][5] = Piece{PieceType::BISHOP, Color::BLACK, '2'};
    matrix[0][6] = Piece{PieceType::KNIGHT, Color::BLACK, '2'};
    matrix[0][7] = Piece{PieceType::ROOK, Color::BLACK, '2'};

    // Row 6 (Black pawns)
    for (int col = 0; col < 8; col++)
    {
        matrix[1][col] = Piece{PieceType::PAWN, Color::BLACK, static_cast<char>('1' + col)};
    }

    // Rows 5-2 (Empty - defaults are already BLANK)

    board.setMatrix(matrix);
}

Board Game::getBoard() const
{
    return board;
}

Color Game::getTurn() const
{
    return gameState.turn;
}

void Game::changeTurn()
{
    gameState.turn = gameState.turn == Color::BLACK ? Color::WHITE : Color::BLACK;
}

bool Game::move(Coord from, Coord to)
{
    Piece movingPiece = board.getPiece(from);

    // Making sure all arguments are valid
    if (movingPiece.t == PieceType::BLANK || movingPiece.t == PieceType::ERROR || !isValidCoord(to))
        return false;

    if (movingPiece.t == PieceType::PAWN)
    {
        // Updating last en passant coordinates
        if (to == Coord{from.row + 2, from.col} || to == Coord{from.row - 2, from.col})
        {
            if (movingPiece.c == Color::BLACK)
                gameState.enPassant = Coord{from.row + 1, from.col};
            else
                gameState.enPassant = Coord{from.row - 1, from.col};
        }
        else
        {
            if (to == gameState.enPassant)
            {
                Coord removePawn = Coord{from.row, gameState.enPassant.col};
                board.setPiece(removePawn, Piece{PieceType::BLANK, Color::BLACK, 'b'});
            }
            // Clearing last en passant coordinates, since it now has been 2 moves since the pawn moved
            gameState.enPassant = Coord{8, 8};
        }
        if ((movingPiece.c == Color::WHITE && to.row == 0) ||
            (movingPiece.c == Color::BLACK && to.row == 7))
        {
            Piece promoted = movingPiece;
            promoted.t = PieceType::QUEEN;
            board.setPiece(to, promoted);
        }
    }
    else
    {
        // Clearing last en passant coordinates, since it now has been 2 moves since the pawn moved
        gameState.enPassant = Coord{8, 8};

        // Updating castling logic and moving the rook
        if (movingPiece.t == PieceType::ROOK)
        {
            // Removing castling possibility after rook move
            int colorIndex = movingPiece.c == Color::BLACK ? 1 : 0;
            gameState.castling[movingPiece.id - '1'][colorIndex] = false;
        }
        if (movingPiece.t == PieceType::KING)
        {
            // Setting king side rook to the right of the king
            if (to == Coord{from.row, from.col + 2})
            {
                Piece rookKingSide = Piece{PieceType::ROOK, movingPiece.c, '2'};
                board.setPiece(Coord{from.row, from.col + 1}, rookKingSide);

                // Clearing the old rook
                board.setPiece(Coord{from.row, from.col + 3}, Piece{PieceType::BLANK, Color::BLACK, 'b'});
            }
            // Setting queen side rook to the left of the king
            else if (to == Coord{from.row, from.col - 2})
            {
                Piece rookKingSide = Piece{PieceType::ROOK, movingPiece.c, '1'};
                board.setPiece(Coord{from.row, from.col - 1}, rookKingSide);

                // Clearing the old rook
                board.setPiece(Coord{from.row, from.col - 4}, Piece{PieceType::BLANK, Color::BLACK, 'b'});
            }

            int colorIndex = movingPiece.c == Color::BLACK ? 1 : 0;
            if (gameState.castling[0][colorIndex] || gameState.castling[1][colorIndex])
            {
                gameState.castling[0][colorIndex] = false;
                gameState.castling[1][colorIndex] = false;
            }
        }
    }

    board.setPiece(to, movingPiece);
    board.setPiece(from, Piece{PieceType::BLANK, Color::BLACK, 'b'});

    // Fixing king tracking
    if (movingPiece.t == PieceType::KING)
    {
        if (movingPiece.c == Color::BLACK)
            blackKing = to;
        else
            whiteKing = to;
    }

    changeTurn();
    return true;
}

void Game::setHistory()
{
    SnapShot snap = SnapShot{board.snapshot(), gameState, blackKing, whiteKing};
    history.push(snap);
}

void Game::revertState(const SnapShot &snap)
{
    gameState = snap.state;
    board.setMatrix(snap.board);
    blackKing = snap.blackKing;
    whiteKing = snap.whiteKing;
}

bool Game::isKingInCheck(Color c)
{
    if (c == Color::BLACK)
        return MoveGenerator::isSquareAttackedBy(board, blackKing, Color::WHITE);

    return MoveGenerator::isSquareAttackedBy(board, whiteKing, Color::BLACK);
}

std::array<Coord, 27> Game::possibleMoves(const Coord& piece)
{
    std::array<Coord, 27> arr = {};

    Piece p = board.getPiece(piece);

    if (p.c != gameState.turn)
        return arr;

    arr = MoveGenerator::pseudoLegalMoves(board, gameState, piece);

    std::array<Coord, 27> possible;
    // Setting sentinel values - Remember to check
    for (int i = 0; i < 27; i++)
        possible[i] = Coord{8, 8};

    int k = 0;
    // Making sure the obtained moves don't result in check on self king
    for (int i = 0; i < 27; i++)
    {
        if (arr[i] == Coord{8, 8})
            break;

        if (!testForCheck(piece, arr[i]))
        {
            possible[k] = arr[i];
            k++;
        }
    }

    return possible;
}

bool Game::hasMoves(Color c)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board.getPiece(Coord{i, j}).c == c && board.getPiece(Coord{i, j}).t != PieceType::BLANK)
            {
                if (isValidCoord(possibleMoves(Coord{i, j})[0]))
                    return true;
            }
        }
    }

    return false;
}

bool Game::testForCheck(Coord from, Coord to)
{
    // Saving current Board and Game State
    SnapShot snap = SnapShot{board.snapshot(), gameState, blackKing, whiteKing};

    Piece movingPiece = board.getPiece(from);

    // Making the move
    move(from, to);

    // Finding if in check
    bool check = isKingInCheck(movingPiece.c);

    revertState(snap);

    return check;
}

bool Game::makeMove(Coord from, Coord to)
{
    setHistory();
    return move(from, to);
}

void Game::applyMove(Coord from, Coord to)
{
    move(from, to);
}

void Game::undo()
{
    const SnapShot snap = history.top();
    history.pop();

    board.setMatrix(snap.board);
    gameState = snap.state;
}

SnapShot Game::getSnap() const
{
    return SnapShot{board.snapshot(), gameState, blackKing, whiteKing};
}

std::stack<SnapShot> Game::getHistory() const
{
    return history;
}

bool Game::staleMateByMaterial() const
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board.getPiece(Coord{i, j}).t != PieceType::BLANK)
                if (board.getPiece(Coord{i, j}).t != PieceType::KING)
                    return false;
        }
    }

    return true;
}

// TODO: Implement getCheapSnap() and revertState(const CheapSnap & snap);

