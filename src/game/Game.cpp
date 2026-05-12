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

    lastCheapSnap.state = gameState;
    lastCheapSnap.blackKing = blackKing;
    lastCheapSnap.whiteKing = whiteKing;
    lastCheapSnap.lastMove.from = from;
    lastCheapSnap.lastMove.to = to;
    lastCheapSnap.lastMove.captured = board.getPiece(to);

    if (lastCheapSnap.lastMove.captured.t == PieceType::BLANK)
        lastCheapSnap.lastMove.type = MoveType::QUIET;
    else
        lastCheapSnap.lastMove.type = MoveType::CAPTURE;

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
                lastCheapSnap.lastMove.type = MoveType::ENPASSANT;
                lastCheapSnap.lastMove.captured = board.getPiece(Coord{from.row, gameState.enPassant.col});
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
            lastCheapSnap.lastMove.type = MoveType::PROMOTION;
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
                lastCheapSnap.lastMove.type = MoveType::CASTLING_KS;
                Piece rookKingSide = Piece{PieceType::ROOK, movingPiece.c, '2'};
                board.setPiece(Coord{from.row, from.col + 1}, rookKingSide);

                // Clearing the old rook
                board.setPiece(Coord{from.row, from.col + 3}, Piece{PieceType::BLANK, Color::BLACK, 'b'});
            }
            // Setting queen side rook to the left of the king
            else if (to == Coord{from.row, from.col - 2})
            {
                lastCheapSnap.lastMove.type = MoveType::CASTLING_QS;
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
    // Save moving piece's color before move (since it may be overwritten)
    Piece movingPiece = board.getPiece(from);
    Color moverColor = movingPiece.c;

    // Perform the move - this populates lastCheapSnap automatically
    move(from, to);

    // Grab the cheap snapshot (pre-move state)
    CheapSnap snap = getCheapSnap();

    bool check = isKingInCheck(moverColor);

    // Revert using the cheap snapshot
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

void Game::revertState(const SnapShot &snap)
{
    gameState = snap.state;
    board.setMatrix(snap.board);
    blackKing = snap.blackKing;
    whiteKing = snap.whiteKing;
}

SnapShot Game::getSnap() const
{
    return SnapShot{board.snapshot(), gameState, blackKing, whiteKing};
}

void Game::revertState(const CheapSnap &snap)
{
    gameState = snap.state;
    blackKing = snap.blackKing;
    whiteKing = snap.whiteKing;

    const MoveInfo& lastMove = snap.lastMove;
    const Piece moved = board.getPiece(lastMove.to);

    if (lastMove.type == MoveType::PROMOTION)
    {
        const Piece originalPawn = {PieceType::PAWN, moved.c, moved.id};
        board.setPiece(lastMove.from, originalPawn);
        board.setPiece(lastMove.to, lastMove.captured);
    }

    if (lastMove.type == MoveType::QUIET || lastMove.type == MoveType::CAPTURE)
    {
        board.setPiece(lastMove.from, moved);
        board.setPiece(lastMove.to, lastMove.captured);
    }

    if (lastMove.type == MoveType::ENPASSANT)
    {
        board.setPiece(lastMove.from, moved);
        const Coord capturedPawnCoord = {lastMove.from.row, lastMove.to.col};
        board.setPiece(capturedPawnCoord, lastMove.captured);
        board.setPiece(lastMove.to, Piece{PieceType::BLANK, Color::BLACK, 'b'});
    }

    if (lastMove.type == MoveType::CASTLING_KS)
    {
        board.setPiece(lastMove.from, moved);
        board.setPiece(lastMove.to, lastMove.captured);
        const Coord initialRookCoord = {lastMove.to.row, lastMove.to.col - 1};
        const Piece rook = board.getPiece(initialRookCoord);
        board.setPiece(initialRookCoord, Piece{PieceType::BLANK, Color::BLACK, 'b'});
        const Coord finalRookCoord = {lastMove.to.row, lastMove.to.col + 1};
        board.setPiece(finalRookCoord, rook);
    }
    if (lastMove.type == MoveType::CASTLING_QS)
    {
        board.setPiece(lastMove.from, moved);
        board.setPiece(lastMove.to, lastMove.captured);
        const Coord initialRookCoord = {lastMove.to.row, lastMove.to.col + 1};
        const Piece rook = board.getPiece(initialRookCoord);
        board.setPiece(initialRookCoord, Piece{PieceType::BLANK, Color::BLACK, 'b'});
        const Coord finalRookCoord = {lastMove.to.row, lastMove.to.col - 2};
        board.setPiece(finalRookCoord, rook);
    }
}

CheapSnap Game::getCheapSnap() const
{
    return lastCheapSnap;
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

