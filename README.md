# Chess Engine
[![Tests](https://github.com/andresgcsdev/ChessEngineCpp/actions/workflows/test.yml/badge.svg)](https://github.com/ExcalDex/ChessEngineCpp/actions)


A C++ chess engine with a full game implementation and an AI opponent powered by minimax search, alpha‑beta pruning, move ordering, Zobrist hashing, and a transposition table.

---

## Features

- **Full Chess Rules:** Pawn promotion, castling, en passant, check/checkmate detection
- **AI Engine:** Minimax search with alpha-beta pruning and piece-square table evaluation
- **Position Hashing:** Zobrist hashing with incremental updates (64‑bit keys)
- **Game State Management:** Full move history with undo/revert functionality
- **Clean Architecture:** Separation of concerns into `board`, `engine`, `fingerprint`, `game`, `movegen`, `types` and `ui`
- **Testing:** 180 unit tests, all passing (FEN‑based validation)
---

## Building

### Requirements

- C++17 compiler
- CMake 3.10+

### Build Instructions

```bash
git clone https://github.com/andresgcsdev/ChessEngineCpp.git
cd ChessEngineCpp
mkdir build
cmake -B build
cmake --build build
```

The executable will be at build/chess on Linux/Mac or build/Debug/chess.exe on Windows.

---

## How to Play

Run the executable and follow the on-screen prompts:

```bash
./chess  # or ./chess.exe on Windows
```
- Select which color you want to play as
- Select the piece you want to move
- Enter coordinates in algebraic notation (e.g., `e2` or `h8`)
- White plays first
- The AI will respond with its best move

---

## Testing

Comprehensive unit test suite covering:
- **Common**: Coordinate/piece validation and equality operators
- **UI**: Text-to-piece, text-to-coordinate, and piece-to-text transformations
- **Board**: Initialization, matrix operations, piece lookup, move execution
- **Game**: Move validation, check/checkmate detection, castling, en passant
- **Engine**: Move validation for both colors

### Run Tests

```bash
cmake --build build --target chess_test
./build/chess_test
```

### Test Infrastructure

- FEN notation generator for board state validation
- FEN parser for easy position setup in test cases
- Test cases organized by component and complexity
- All systems nominal when tests pass ✓

---

## Architecture

### Core Components

**Board** (`board/Board.hpp`): 
- 8×8 piece matrix
- Raw piece placement

**Game** (`game/Game.hpp`): 
- Chess rules enforcement
- Move history
- Undo with full SnapShot
- Fast revert with CheapSnap

**Engine** (`engine/Engine.hpp`): 
- Depth‑6 minimax search with alpha‑beta pruning
- Move ordering
- Evaluation call to Eval::evaluate

**MoveGenerator** (`movegen/MoveGenerator.hpp`): 
- Stateless pseudo‑legal move generation
- Square control check logic

**Eval** (`engine/Eval.hpp`): 
- Material + piece‑square table evaluation
- Endgame detection

**Zobrist** (`fingerprint/Zobrist.hpp`): 
- 64‑bit position hashing with incremental XOR updates
- Random tables fixed for reproducibility.

**Transposition Table** (`currently inside Engine`):
- 12MB static array storing hash, score, depth, and bound flag (EXACT/ALPHA/BETA)
- Used to cache searched positions and avoid re‑searching transpositions

**ChessUI** (`ui/ChessUI.hpp`): 
- Board display with coordinates
- Input parsing.

---

## Design Decisions

**Depth 6 Minimax with Alpha-Beta Pruning + Table Transposition Cache:** Table Transposition allows Engine to cache already-calculated moves, and  Alpha-beta pruning eliminates branches that won't affect the final decision, allowing reasonable search depth without excessive computation. Depth 6 balances move quality with search time (10 to 15~ seconds per move at first, after TT population goes to 5~ seconds).

**Move Ordering (MVV‑LVA):** All legal moves are collected and sorted before the search. Captures and promotions are tried first, scored by Most Valuable Victim / Least Valuable Attacker. This triggers more cutoffs and drastically reduces the effective branching factor.

**Delta‑State Revert (CheapSnap):** Instead of copying the full 8×8 board at every search node, only the minimal move delta + pre‑move game state is stored. Reverting a move touches at most 4 squares – constant‑time undo. CheapSnap is 12.5× smaller than a full SnapShot, removing memory traffic as a bottleneck.

**Transposition Table (TT):** Positions are hashed with Zobrist (64‑bit). The table stores depth, score, and bound type. At the start of a search node we probe the TT – if the stored depth is sufficient and the bound allows an early cutoff, we return immediately. This prevents re‑searching identical positions reached through different move orders. The TT is a static 12 MB array (data segment, not stack) to avoid overflow.

**Zobrist Hashing – Incremental Updates:** The game state hash is updated during move execution by XOR‑ing out the old piece key at `from`, XOR‑ing in the new piece key at `to`, and toggling side‑to‑move, en passant file, and castling rights as needed. No recomputation from scratch. The hash in CheapSnap is restored automatically on revert.

**Piece-Square Tables:** Standard chess evaluation tables reward piece placement (e.g., pawns advancing, knights in center, kings centralizing in endgame).

**No Inheritance / Stack‑first Allocation:** The design uses composition over inheritance and avoids virtual classes. Core game state (board, piece data, state structs) lives on the stack where possible. Large structures (TT, move lists) are placed on the heap or in static data. This keeps memory predictable and removes allocation overhead from the search hot path.

---

## The Journey

This is my fifth chess implementation, and the first one I'm fully satisfied with architecturally.

It started in 2022 with a ~3000 line implementation in VisuAlg (a Portuguese pseudocode language) during the first year of technical school — built purely because the coursework got too easy. That version is lost.

From there came a JavaScript web implementation during the second year, featuring a partially functional AI called PeacockBass — the name is a joke one related to stockfish, I found the portuguese name funny at the time. It was abandoned due to JavaScript's performance limits and a bug in the capture evaluation that took too long to track down. The code is still on GitHub if you're curious.

Then a first C++ version, functional but without proper architecture. Then implementations in Java, Rust, and others — each one using chess as a benchmark for learning a new language. The same problem, different constraints, different lessons each time.

This version is the result of all of that. Same problem, finally complete.

---

## Known Limitations

- No opening book or endgame tables
- Evaluation considers material and position; no tactical pattern detection (pins, forks, etc.)
- Worst‑case search time still spikes in complex mid‑games (a few seconds on average, but up to ~20s)

---

## Future Improvements

- Iterative deepening
- Killer moves / history heuristic
- GUI (Python + tkinter, FEN‑based communication)
- Evaluation improvements (pawn structure, king safety, mobility)

---

## Technical Notes

The engine uses a minimax algorithm with alternating maximization/minimization layers:

- Engine maximizes its own position score
- Opponent minimizes the engine's advantage (plays optimally against you)
- Positions are evaluated at max depth; deeper moves are explored recursively
- Alpha-beta pruning optimizes the search by skipping evaluation of branches where the opponent has already proven they have better alternatives.
- Move ordering sorts captures and promotions first, making pruning far more effective.
- Transposition table caches evaluated positions, often reusing a deeper search result from another branch.

Position evaluation = material value + positional bonuses from piece-square tables. Checkmate returns ±∞; stalemate returns -50 (discouraged but acceptable).

---

## About the Author

I'm André, a Computer Science student at UFLA (Universidade Federal de Lavras), with a technical degree in Systems Development from CEFET-MG Timóteo.

I tend to learn by building things that genuinely interest me rather than following curricula. Chess has been a recurring benchmark throughout that process — something complex enough to teach you a lot about a language, familiar enough that the logic never gets in the way of the learning.

Most of what I know I taught myself out of curiosity: multiple programming languages, chess engines, procedural dungeon generators, PDF parsers, web scrapers. I'm most comfortable in C++, Python and Java, and most interested in algorithm design and software architecture.

If you want to see more: [github.com/andresgcsdev](https://github.com/andresgcsdev)

---

*Built as a portfolio project to demonstrate C++ fundamentals, algorithm design, and software architecture.*

---

***Pragmatism over purism.***