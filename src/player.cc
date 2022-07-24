#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "player.h"
#include "gamestate.h"

using namespace std;

size_t Player::nextPlayerId = 0;

Player::Player() : playerId{nextPlayerId} { ++nextPlayerId; }
Player::~Player() {}

void Player::playTurn(GameState& gameState) {
    if (gameState.requiresDiceRoll()) {
        makeDiceRoll(gameState);
        return;
    }
    if (gameState.hasNoValidMoves()) {
        skipTurn(gameState);
        return;
    }

    determineAndMakeMove(gameState);
}


size_t Player::getPlayerId() const {
    return playerId;
}

