#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "player.h"
#include "gamestate.h"
// The imports exclusive to AI
#include "token.h"
#include "tokenassassin.h"
#include "tokenbasic.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

size_t Player::nextPlayerId = 0;

char* AIMadeInvalidMoveException::what() {
    return "Uh oh!";
}

Player::Player() : playerId{nextPlayerId} { ++nextPlayerId; }
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


size_t Player::getPlayerId() const {
    return playerId;
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

    (*out) << "Please type 'roll' to roll the dice or 't' to show your tokens: " << std::endl;

    gameState.getPlayersTokens();

    (*in) >> cmd;

    // try again until valid
    while (cmd != "roll") {
        if (cmd == "t") {
            (*out) << "Invalid, please type 'roll' to roll the dice: " << std::endl;
        } else {
            (*out) << "Invalid, please type 'roll' to roll the dice: " << std::endl;
        }
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

//////////////////////////////////////////// AI ///////////////////////////////////////////////////

AI::~AI() {}
AI::AI() : Player() {}

void AI::determineAndMakeMove(GameState &gameState) const {
    // pre: there exists at least one valid move this turn
    std::pair<size_t, size_t> move = findMove(gameState); // returns a tokenID and a distance 

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


std::vector<std::pair<size_t, size_t>> AI::findAllValidMoves(const GameState& gameState) const {
    // find my tokens in the gamestate

    std::vector<Token *> myTokens = gameState.getPlayersTokens()[getPlayerId()];
    std::vector<std::pair<size_t, size_t>> validMoves;

    size_t diceRoll = gameState.getDiceRoll();
    size_t flexRoll = gameState.getFlexDiceRoll();

    
    for (auto x : myTokens) {
        std::vector<size_t> movesOfX = x->getValidMoveDistances(diceRoll, flexRoll);
        for (auto y : movesOfX) {
            if (gameState.moveValid(x->getTokenId(), y) && 
                    moveIsNotRepeated(validMoves, x->getTokenId(), y)) {
                validMoves.emplace_back(std::make_pair(x->getTokenId(), y));
            }
        }
        // if (gameState.moveValid(x->getTokenId(), flexRoll) && 
        //         moveIsNotRepeated(validMoves, x->getTokenId(), flexRoll)) {
        //     validMoves.emplace_back(std::make_pair(x->getTokenId(), flexRoll));
        // }
    }

    return validMoves;
}

bool AI::moveIsNotRepeated(std::vector<std::pair<size_t, size_t>> validMoves, size_t tokenId, size_t rollNum) {
    // tokenId is tokenId, rollNum is either diceRoll or flexDiceRoll
    // returns true if <tokenId, rollNum> is unique in validMoves (no occurrences yet)
    std::cout << (std::find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](std::pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end()) << " ";
    std::cout << tokenId << rollNum << std::endl;
    return (std::find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](std::pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end());
}

std::vector<std::pair<int, std::pair<size_t, size_t>>> AI::assignPriorities( 
    std::vector<std::pair<size_t, size_t>> &movelist) {
    // Level 1 and Default AI implementation: an AI that randomly chooses a move that is valid with no preference
    // no priorities are needed to be assigned
    std::vector<std::pair<int, std::pair<size_t, size_t>>> movesAndWeights;
    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }
    return movesAndWeights;
}

// level 1 AI

Level1AI::Level1AI(): AI() {}
Level1AI::~Level1AI() {}
std::pair<size_t, size_t> Level1AI::findMove(const GameState& gameState) const {
    // find my valid moves in the gamestate
    // pre: since gamestate did not tell us noValidMoves, there exists at least 1 valid move
    // std::cout << "before findAllValidMoves " << std::endl;
    std::vector<std::pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    
    // generate a random number between 0 and movelist.length() and return the pair at that index
    std::cout << "before generating randnums " << std::endl;
    size_t index = static_cast<size_t>(rand() % movelist.size());
    return movelist.at(index);
}


// level 2 AI

Level2AI::Level2AI(): AI() {}
Level2AI::~Level2AI() {}
std::pair<size_t, size_t> Level2AI::findMove(const GameState& gameState) const {
    std::vector<std::pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    // sortByPriorities(movelist); // post: first is highest priority 
    // return highest priority move 
    return movelist.at(0);
    
}

std::vector<std::pair<int, std::pair<size_t, size_t>>> Level2AI::assignPriorities( 
    std::vector<std::pair<size_t, size_t>> &movelist) {
    // Level 2: an AI that slightly favours certain beneficial moves over others.
    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. 
    // 2. Favour moves that land on a rosette or a lucky tile. 
    // 3. Favour moves that steal someone else’s token. 
    
    std::vector<std::pair<int, std::pair<size_t, size_t>>> movesAndWeights;
    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }

    // assign weight 1 to item 3
    for (auto &x : movesAndWeights) {
        size_t distance = x.second.second;
        if (1) {
            ++x.first;
        }
    }

    // assign weight 2 to item 2
    for (auto &x : movesAndWeights) {
        size_t distance = x.second.second;
        if (1) {
            x.first += 2;
        }
    }

    // assign weight 3 to item 1
    for (auto &x : movesAndWeights) {
        size_t distance = x.second.second;
        if (1) {
            x.first += 3;
        }
    }
}

// level 3 AI
Level3AI::Level3AI(): AI() {}
Level3AI::~Level3AI() {}
std::pair<size_t, size_t> Level3AI::findMove(const GameState& gameState) const {

}

std::vector<std::pair<int, std::pair<size_t, size_t>>> Level3AI::assignPriorities( 
    std::vector<std::pair<size_t, size_t>> &movelist) {

}