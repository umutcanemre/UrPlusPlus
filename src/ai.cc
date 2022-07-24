#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "ai.h"
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

char* AIMadeInvalidMoveException::what() {
    return "Uh oh!";
}

AI::~AI() {}
AI::AI() : Player() {}

void AI::determineAndMakeMove(GameState &gameState) {
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

    std::cout << "All moves: " << std::endl;
    for (auto x : validMoves) {
        std::cout << x.first << " " << x.second << std::endl;
    }
    return validMoves;
}

bool AI::moveIsNotRepeated(vector<pair<size_t, size_t>> validMoves, size_t tokenId, size_t rollNum) {
    // tokenId is tokenId, rollNum is either diceRoll or flexDiceRoll
    // returns true if <tokenId, rollNum> is unique in validMoves (no occurrences yet)
    std::cout << (find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end()) << " ";
    std::cout << tokenId << " " << rollNum << endl;
    return (find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end());
}

vector<pair<int, pair<size_t, size_t>>> AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
    // Level 1 and Default AI implementation: an AI that randomly chooses a move that is valid with no preference
    // no priorities are needed to be assigned
    vector<pair<int, pair<size_t, size_t>>> movesAndWeights;
    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }
    return movesAndWeights;
}


void AI::setTokenScore(int v) { visitedTokenScore = v; }
int AI::getTokenScore() const { return visitedTokenScore; }
void AI::setTileScore(int v) { visitedTileScore = v; }
int AI::getTileScore() const { return visitedTileScore; }

// default visitor implementations - do nothing 
void AI::visitTileTornado(const TileTornado&) {}
void AI::visitTileBlackHole(const TileBlackHole&) {}
void AI::visitTileLucky(const TileLucky&) {}
void AI::visitTileRosette(const TileRosette&) {}
void AI::visitTileBasic(const TileBasic&) {}
void AI::visitTileNull(const TileNull&) {}

void AI::visitTokenBasic(const TokenBasic&) {}
void AI::visitTokenAssassin(const TokenAssassin&) {}
void AI::visitTokenFlexible(const TokenFlexible&) {}
void AI::visitTokenSpeedster(const TokenSpeedster&) {}
void AI::visitTokenSupporter(const TokenSupporter&) {}

