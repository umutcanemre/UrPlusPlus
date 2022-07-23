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
#include "tile.h"
#include "tilebasic.h"
#include "tileblackhole.h"
#include "tilelucky.h"
#include "tilerosette.h"
#include "tiletornado.h"
#include "tilenull.h"

using namespace std;

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

    (*out) << "Please type 'roll' to roll the dice or 't' to show your tokens: " << endl;

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

void Human::determineAndMakeMove(GameState &gameState) const {
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

//////////////////////////////////////////// AI ///////////////////////////////////////////////////

AI::~AI() {}
AI::AI() : Player() {}

void AI::determineAndMakeMove(GameState &gameState) const {
    // pre: there exists at least one valid move this turn
    pair<size_t, size_t> move = findMove(gameState); // returns a tokenID and a distance 

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


vector<pair<size_t, size_t>> AI::findAllValidMoves(const GameState& gameState) const {
    // find my tokens in the gamestate

    vector<Token *> myTokens = gameState.getPlayersTokens()[getPlayerId()];
    vector<pair<size_t, size_t>> validMoves;

    size_t diceRoll = gameState.getDiceRoll();
    size_t flexRoll = gameState.getFlexDiceRoll();

    
    for (auto x : myTokens) {
        vector<size_t> movesOfX = x->getValidMoveDistances(diceRoll, flexRoll);
        for (auto y : movesOfX) {
            if (gameState.moveValid(x->getTokenId(), y) && 
                    moveIsNotRepeated(validMoves, x->getTokenId(), y)) {
                validMoves.emplace_back(make_pair(x->getTokenId(), y));
            }
        }
    }

    return validMoves;
}

bool AI::moveIsNotRepeated(vector<pair<size_t, size_t>> validMoves, size_t tokenId, size_t rollNum) {
    // tokenId is tokenId, rollNum is either diceRoll or flexDiceRoll
    // returns true if <tokenId, rollNum> is unique in validMoves (no occurrences yet)
    cout << (find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end()) << " ";
    cout << tokenId << rollNum << endl;
    return (find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end());
}

vector<pair<int, pair<size_t, size_t>>> AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) const {
    // Level 1 and Default AI implementation: an AI that randomly chooses a move that is valid with no preference
    // no priorities are needed to be assigned
    vector<pair<int, pair<size_t, size_t>>> movesAndWeights;
    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }
    return movesAndWeights;
}

// level 1 AI

Level1AI::Level1AI(): AI() {}
Level1AI::~Level1AI() {}
pair<size_t, size_t> Level1AI::findMove(const GameState& gameState) const {
    // find my valid moves in the gamestate
    // pre: since gamestate did not tell us noValidMoves, there exists at least 1 valid move
    // cout << "before findAllValidMoves " << endl;
    vector<pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    
    // generate a random number between 0 and movelist.length() and return the pair at that index
    // cout << "before generating randnums " << endl;
    size_t index = static_cast<size_t>(rand() % movelist.size());
    return movelist.at(index);
}


// level 2 AI

Level2AI::Level2AI(): AI() {}
Level2AI::~Level2AI() {}
pair<size_t, size_t> Level2AI::findMove(const GameState& gameState) const {
    // pre: there is at least 1 valid move, so movelist.length should be >= 1 or something is wrong
    // with my code
    vector<pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    vector<pair<int, pair<size_t, size_t>>> weightedMovelist = assignPriorities(movelist,gameState);

    // sortByPriorities(movelist); // post: first is highest priority 
    // first loop: find the highest priority
    int maxWeight = weightedMovelist.at(0).first;
    for (auto x: weightedMovelist) {
        if (maxWeight < x.first) {
            maxWeight = x.first;
        }
    }
    // if there are multiple moves with the same weight I should randomly generate which move to pick, 
    // because I do not want it to pick the first one in order every single time - it would just end 
    // up moving the same token every time when there are multiple of the same weight and random
    // would be strategically better in this case than predictable
    vector<pair<int, pair<size_t, size_t>>> candidates;
    for (auto x: weightedMovelist) {
        if (maxWeight == x.first) {
            candidates.emplace_back(x);
        }
    }

    size_t index = static_cast<size_t>(rand() % candidates.size());
    cout << candidates.at(index).first << candidates.at(index).second.first 
        << candidates.at(index).second.second << endl;
    return candidates.at(index).second;
}

vector<pair<int, pair<size_t, size_t>>> Level2AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) const {
    // Level 2: an AI that slightly favours certain beneficial moves over others.
    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. 
    // 2. Favour moves that land on a rosette or another tile that grants a new turn. 
    // 3. Favour moves that land on a rosette or another tile that grants invulnerability. 
    // 4. Favour moves that steal someone else’s token. 
    // 5. Avoid moves that land on black hole or tornado or lucky if has a negative effect, 
    //    but favours these tiles if they have a positive effect.


    // Level 2: an AI that slightly favours certain beneficial moves over others.
    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. +4 
    // 2. Favour moves that land on a rosette. +3
    // 3. Favour moves that steal someone else’s token. +2 
    // 4. Favour moves that land on a lucky tile. +1 
    // 5. Avoid moves that land on tornado. -1
    // 6. Avoid moves that land on black hole. -2
    // 7. Neutral to moves otherwise. +0
    
    vector<pair<int, pair<size_t, size_t>>> movesAndWeights;
    const std::vector<Tile*> path = gameState.getPlayersPaths().at(getPlayerId());
    std::vector<Token*> tokens = gameState.getPlayersTokens().at(getPlayerId());

    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }

    // TODO: implement Tile public interface to enable this
    // assign weight 1 to item 3, weight 2 to item 2, weight 3 to item 1
    for (auto &x : movesAndWeights) {
        Token *token = tokens.at(x.second.first);
        size_t newIndex = token->getPathProgress() + x.second.second - 1;
        // item 1 - end of path is reached
        if (newIndex == path.size()) {
            x.first += 4;
        }
        // item 2,3,4 (exclusive with item 1) but not each other
        else {
            int tileMoveWeight = path.at(newIndex)->acceptVisitor(*this);
            x.first += tileMoveWeight;
            // if (path.at(newIndex)->grantsExtraTurn()) {
            //     x.first += 2;
            // }
            // if (path.at(newIndex)->grantsInvulnerability()) {
            //     x.first += 2;
            // }
            // if (path.at(newIndex)->getOccupant()) {
            //     // at this point since we already ran moveValid, we are guaranteed that if 
            //     // the tile has an occupant, it is an enemy token
            //     ++x.first;
            // }
            // x.first += path.at(newIndex)->sendsPlayerForward(newIndex, path);
        }
    }

}

// level 3 AI
Level3AI::Level3AI(): AI() {}
Level3AI::~Level3AI() {}
pair<size_t, size_t> Level3AI::findMove(const GameState &gameState) const {
    // pre: there is at least 1 valid move, so movelist.length should be >= 1 or something is wrong
    // with my code
    vector<pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    vector<pair<int, pair<size_t, size_t>>> weightedMovelist = assignPriorities(movelist,gameState);

    // sortByPriorities(movelist); // post: first is highest priority 
    // first loop: find the highest priority
    int maxWeight = weightedMovelist.at(0).first;
    for (auto x: weightedMovelist) {
        if (maxWeight < x.first) {
            maxWeight = x.first;
        }
    }
    // if there are multiple moves with the same weight I should randomly generate which move to pick, 
    // because I do not want it to pick the first one in order every single time - it would just end 
    // up moving the same token every time when there are multiple of the same weight and random
    // would be strategically better in this case than predictable
    vector<pair<int, pair<size_t, size_t>>> candidates;
    for (auto x: weightedMovelist) {
        if (maxWeight == x.first) {
            candidates.emplace_back(x);
        }
    }

    size_t index = static_cast<size_t>(rand() % candidates.size());
    cout << candidates.at(index).first << candidates.at(index).second.first 
        << candidates.at(index).second.second << endl;
    return candidates.at(index).second;
}

vector<pair<int, pair<size_t, size_t>>> Level3AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) const {
    // Level 3: an AI that favours certain beneficial moves over others and also is capable
    // of using abilities at the right times. This includes:

    // - prefers using assassin to capture other tokens over using basic/others
    // - prefers using supporter to support special tokens over basic tokens
    // - prefers using limited-use abilities only when it comes with a significant benefit: like 
    //   claiming a shared-path rosette to gain invulnerability early-game, or completing a path
    // - prefers choosing the higher flexible tile move unless it moves it 2 or 3 squares
    //   in front of an enemy token - the opponent's most likely roll - on a shared path
    // - prefers tornado if it sends player forward in the path; avoids tornado if it sends player
    //   backward in the path

    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. 
    // 2. Favour moves that land on a rosette or a lucky tile. 
    // 3. Favour moves that steal someone else’s token. 
    // 4. Avoid moves that land on black holes.

}