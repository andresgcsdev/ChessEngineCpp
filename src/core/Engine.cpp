#pragma once
#include <array>
#include "Game.hpp"
#include "Board.hpp"
#include "Common.hpp"
#include "Engine.hpp"

Engine::Engine(Color c)
{
    selfColor = c;
}

std::array<Coord, 2> Engine::getBestMove(Game game)
{
    if (game.getTurn() != selfColor)
        return {Coord{8, 8}, Coord{8, 8}};


}
