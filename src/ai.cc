#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "ai.h"
#include "gamestate.h"
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

const char* AIMadeInvalidMoveException::what() {
    return message.c_str();
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


    for (const auto &x : myTokens) {
        vector<size_t> movesOfX = x->getValidMoveDistances(diceRoll, flexRoll);
        for (const auto &y : movesOfX) {
            if (gameState.moveValid(x->getTokenId(), y) &&
                    moveIsNotRepeated(validMoves, x->getTokenId(), y)) {
                validMoves.emplace_back(make_pair(x->getTokenId(), y));
            }
        }
    }

    return validMoves;
}

bool AI::moveIsNotRepeated(const vector<pair<size_t, size_t>> &validMoves, size_t tokenId, size_t rollNum) {
    // tokenId is tokenId, rollNum is either diceRoll or flexDiceRoll
    // returns true if <tokenId, rollNum> is unique in validMoves (no occurrences yet)
    return (find_if(validMoves.begin(), validMoves.end(), [tokenId,rollNum](pair<size_t, size_t> move) {
                return move.first == tokenId && move.second == rollNum;
            }) == validMoves.end());
}

vector<pair<float, pair<size_t, size_t>>> AI::assignPriorities(
    const vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
    // Level 1 and Default AI implementation: an AI that randomly chooses a move that is valid with no preference
    // no priorities are needed to be assigned
    vector<pair<float, pair<size_t, size_t>>> weightMovePairs;
    for (const auto &x : movelist) {
        weightMovePairs.emplace_back(0, x);
    }
    return weightMovePairs;
}


void AI::setTokenScore(float v) { visitedTokenScore = v; }
float AI::getTokenScore() const { return visitedTokenScore; }
void AI::setTileScore(float v) { visitedTileScore = v; }
float AI::getTileScore() const { return visitedTileScore; }

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

