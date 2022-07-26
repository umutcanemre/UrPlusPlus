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
    vector<pair<float, pair<size_t, size_t>>> weightedMovelist 
        = assignPriorities(movelist,gameState);

    int maxWeight = weightedMovelist.at(0).first;
    maxWeight = max_element(weightedMovelist.begin(), weightedMovelist.end(), 
        [](const pair<float, pair<size_t, size_t>> &a, const pair<float, pair<size_t, size_t>> &b) {
            return (a.first < b.first);
        }
    )->first;

    // if there are multiple moves with the same weight I should randomly generate which move to pick, 
    // because I do not want it to pick the first one in order every single time - it would just end 
    // up moving the same token every time when there are multiple of the same weight and random
    // would be strategically better in this case than predictable
    vector<pair<int, pair<size_t, size_t>>> candidates;
    for (const auto &x: weightedMovelist) {
        if (maxWeight == x.first) {
            candidates.emplace_back(x);
        }
    }

    size_t index = static_cast<size_t>(rand() % candidates.size());

    cout << "Selected move: " << candidates.at(index).first << 
        candidates.at(index).second.first << 
        candidates.at(index).second.second << endl;
    return candidates.at(index).second;
}

vector<pair<float, pair<size_t, size_t>>> Level2AI::assignPriorities( 
    const vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
    // Level 2: an AI that slightly favours beneficial moves over others.
    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. +4 
    // 2. Favour moves that steal someone else’s token. +2 
    // 3. Neutral to moves otherwise. +0
    
    vector<pair<float, pair<size_t, size_t>>> weightMovePairs;
    const vector<Tile*> path = gameState.getPlayersPaths().at(getPlayerId());
    vector<Token*> tokens = gameState.getPlayersTokens().at(getPlayerId());

    for (const auto &x : movelist) {
        weightMovePairs.emplace_back(0, x);
    }
    
    // assign weights
    for (auto &weightAndMove : weightMovePairs) {
        Token *token = tokens.at(weightAndMove.second.first);
        size_t newIndex = token->getPathProgress() + weightAndMove.second.second - 1;
        // item 1 - end of path is reached
        if (newIndex == path.size()) {
            weightAndMove.first += 4;
        }
        // item 2 (exclusive with item 1) 
        else if (path.at(newIndex)->getOccupant()) {
            weightAndMove.first += 2;
        }
    }
    return weightMovePairs;
}


