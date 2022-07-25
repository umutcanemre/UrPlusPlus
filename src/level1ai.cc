#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "level1ai.h"
#include "gamestate.h"

// level 1 AI

Level1AI::Level1AI(): AI() {}
Level1AI::~Level1AI() {}

std::pair<size_t, size_t> Level1AI::findMove(const GameState& gameState) {
    // find my valid moves in the gamestate
    // pre: since gamestate did not tell us noValidMoves, there exists at least 1 valid move
    std::vector<std::pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    
    // generate a random number between 0 and movelist.length() and return the pair at that index
    size_t index = static_cast<size_t>(rand() % movelist.size());
    return movelist.at(index);
}
