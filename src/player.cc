#include <iostream>
#include <string>
#include "player.h"
#include "gamestate.h"

char* AIMadeInvalidMoveException::what() {
    return "Uh oh!";
}

Player::Player() {}
Player::~Player() {}

void Player::playTurn(GameState& gameState) const {
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

size_t Human::charToTokenId(char c) {
    // unwise ASCII hacking
    if (c >= 'a') {
        return static_cast<size_t>(c - 'a');
    } else {
        return static_cast<size_t>(c - '0');
    }
}

Human::~Human() {}
Human::Human(std::istream *in, std::ostream *out): Player{}, in{in}, out{out} {}

void Human::skipTurn(GameState& gameState) const {
    std::string cmd;
    std::cout << "hit here 2" << std::endl;
    (*out) << "No valid moves! Please type 'skip' to skip your turn: "
        << std::endl;

    (*in) >> cmd;

    // try again until valid
    while (cmd != "skip") {
        (*out) << "Invalid, please type 'skip' to skip your turn: " << std::endl;
        (*in) >> cmd;
    }

    gameState.skipTurn();
}

void Human::makeDiceRoll(GameState &gameState) const {
    std::string cmd;

    (*out) << "Please type 'roll' to roll the dice: " << std::endl;

    (*in) >> cmd;

    // try again until valid
    while (cmd != "roll") {
        (*out) << "Invalid, please type 'roll' to roll the dice: " << std::endl;
        (*in) >> cmd;
    }
    gameState.rollDice();
}

void Human::determineAndMakeMove(GameState &gameState) const {
    char tokenChar;
    size_t distance;

    (*out) <<
        "Enter token id followed by the distance to move it '<id> <distance>'"
        << std::endl;

    (*in) >> tokenChar >> distance;


    // try again until valid
    while (!gameState.movePiece(charToTokenId(tokenChar), distance)) {
        (*out) <<
            "Invalid Move: Enter token id and distance '<id> <distance>'"
            << std::endl;
        (*in) >> tokenChar >> distance;
    }
}

AI::~AI() {}
AI::AI() {}

void AI::determineAndMakeMove(GameState &gameState) const {
    std::pair<size_t, size_t> move = findMove(gameState);

    if (!gameState.movePiece(move.first, move.second)) {
        throw AIMadeInvalidMoveException{};
    }
}

// this is a noop, it instantly acknowledges
void AI::makeDiceRoll(GameState& gameState) const {
    gameState.rollDice();
}

// this is a noop, it instantly acknowledges
void AI::skipTurn(GameState& gameState) const {
    gameState.skipTurn();
}
