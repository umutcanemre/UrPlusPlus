#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "human.h"
#include "gamestate.h"

using namespace std;

size_t Human::charToTokenId(char c) {
    // unwise ASCII hacking
    if (c >= 'a') {
        return static_cast<size_t>(c - 'a');
    } else {
        return static_cast<size_t>(c - '0');
    }
}

Human::~Human() {}
Human::Human(istream *in, ostream *out): Player{}, in{in}, out{out} {}

void Human::skipTurn(GameState& gameState) const {
    string cmd;

    (*out) << "No valid moves! Please type 'skip' to skip your turn: "
        << endl;

    (*in) >> cmd;

    // try again until valid
    while (cmd != "skip") {
        (*out) << "Invalid, please type 'skip' to skip your turn: " << endl;
        (*in) >> cmd;
    }

    gameState.skipTurn();
}

void Human::makeDiceRoll(GameState &gameState) const {
    string cmd;

    (*out) << "Please type 'roll' to roll the dice: " << endl;

    gameState.getPlayersTokens();

    (*in) >> cmd;

    // try again until valid
    while (cmd != "roll") {
        if (cmd == "t") {
            (*out) << "Invalid, please type 'roll' to roll the dice: " << endl;
        } else {
            (*out) << "Invalid, please type 'roll' to roll the dice: " << endl;
        }
        (*in) >> cmd;
    }

    gameState.rollDice();
}

void Human::determineAndMakeMove(GameState &gameState) {
    char tokenChar;
    size_t distance;

    (*out) <<
        "Enter token id followed by the distance to move it '<id> <distance>'"
        << endl;

    (*in) >> tokenChar >> distance;


    // try again until valid
    while (!gameState.movePiece(charToTokenId(tokenChar), distance)) {
        (*out) <<
            "Invalid Move: Enter token id and distance '<id> <distance>'"
            << endl;
        (*in) >> tokenChar >> distance;
    }
}
