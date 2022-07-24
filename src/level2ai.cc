#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "level2ai.h"
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



// level 2 AI

Level2AI::Level2AI(): AI() {}
Level2AI::~Level2AI() {}
pair<size_t, size_t> Level2AI::findMove(const GameState& gameState) {
    // pre: there is at least 1 valid move, so movelist.length should be >= 1 or something is wrong
    // with my code
    vector<pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    vector<pair<int, pair<size_t, size_t>>> weightedMovelist = assignPriorities(movelist,gameState);

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
    std::cout << candidates.at(index).first << candidates.at(index).second.first 
        << candidates.at(index).second.second << std::endl;
    return candidates.at(index).second;
}

vector<pair<int, pair<size_t, size_t>>> Level2AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
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
            path.at(newIndex)->acceptVisitor(*this); // sets 
            x.first += getTileScore();
        }
    }
    return movesAndWeights;
}


// level 2 AI visitor implementations - knows how to check tiles
void Level2AI::visitTileTornado(const TileTornado& t) { setTileScore(-1 + valueOfPiece(t)); }
void Level2AI::visitTileBlackHole(const TileBlackHole& t) { setTileScore(-2 + valueOfPiece(t)); }
void Level2AI::visitTileLucky(const TileLucky& t) { setTileScore(1 + valueOfPiece(t)); }
void Level2AI::visitTileRosette(const TileRosette& t) { setTileScore(3 + valueOfPiece(t)); }
void Level2AI::visitTileBasic(const TileBasic& t) { setTileScore(0 + valueOfPiece(t)); }
void Level2AI::visitTileNull(const TileNull& t) { setTileScore(0); }

int Level2AI::valueOfPiece(const Tile& t) {
    if (t.getOccupant()) return 2;
    return 0;
}
