#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "level4ai.h"
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

// level 4 AI
Level4AI::Level4AI(): AI() {}
Level4AI::~Level4AI() {}
pair<size_t, size_t> Level4AI::findMove(const GameState &gameState) {
    // pre: there is at least 1 valid move, so movelist.length should be >= 1 or something is wrong
    // with my code
    vector<pair<size_t, size_t>> movelist = findAllValidMoves(gameState);
    vector<pair<int, pair<size_t, size_t>>> weightedMovelist = assignPriorities(movelist,gameState);

    // first loop: find the highest priority
    int maxWeight = weightedMovelist.at(0).first;
    maxWeight = max_element(weightedMovelist.begin(), weightedMovelist.end(), 
        [](const pair<int, pair<size_t, size_t>> &a, const pair<int, pair<size_t, size_t>> &b) {
            return (a.first < b.first);
        }
    )->first;
    
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
    cout << "Selected move: " << candidates.at(index).first << 
        candidates.at(index).second.first << 
        candidates.at(index).second.second << endl;
    return candidates.at(index).second;
}

vector<pair<int, pair<size_t, size_t>>> Level4AI::assignPriorities( 
    const vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
    // Level 4: an AI that favours certain beneficial moves over others and also is capable
    // of using abilities at the right times. This includes:

    // - strongly prefers using assassin to capture other tokens over using basic/others
    // - prefers moves that capture other tokens 
    // - prefers using supporter to support special tokens over basic tokens - so when moving 
    //   supporter prefers to travel 
    // - prefers using limited-use abilities only when it comes with a significant benefit: like 
    //   claiming a shared-path rosette to gain invulnerability early-game, or completing a path
    // - prefers choosing the higher flexible tile move unless it moves it 2 or 3 squares
    //   in front of an enemy token - the opponent's most likely roll - on a shared path
    // - prefers tornado if it sends player forward in the path; avoids tornado if it sends player
    //   backward in the path
    // - slightly prefers lucky if it sends player forward in the path; slightly avoids lucky if 
    //   it sends player backward in the path

    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. +6
    // 2. Favour moves that land on a rosette. +5 
    // 3. Favour moves that steal someone else’s token. +S where S = x + y, x is 
    //    token tastiness and y is assassin ability modifier 
    // 4. Avoid moves that land on black holes. -4

    // 5. Penalizes moves that land on a tile 2 or 3 ahead of an enemy token (check)
    //    pathProgress-1-2 occupant and pathProgress-1-3 occupant (-2)
    // 6. Slightly penalizes moves that use a limited-time ability (-1)
    // 6. if pathProgress + distance - 1 = lucky, +luckyDistanceCalculation()
    // 7. if pathProgress + distance - 1 = tornado, +tornadoDistanceCalculation()

    // findAssassins 

    vector<pair<int, pair<size_t, size_t>>> weightMovePairs;
    const vector<Tile*> playerPath = gameState.getPlayersPaths().at(getPlayerId());
    vector<Token*> myTokens = gameState.getPlayersTokens().at(getPlayerId());

    for (const auto &x : movelist) {
        weightMovePairs.emplace_back(0, x);
    }
    
    // assign weight 1 to item 3, weight 2 to item 2, weight 3 to item 1
    for (auto &weightAndMove : weightMovePairs) {
        Token *token = myTokens.at(weightAndMove.second.first);
        size_t newIndex = token->getPathProgress() + weightAndMove.second.second - 1;

        // item 1 - end of path is reached
        if (newIndex == playerPath.size()) {
            weightAndMove.first += 6;
        }

        else {
            Tile *tileAtMove = playerPath.at(newIndex);
            // item 2, 3, 4
            tileAtMove->acceptVisitor(*this); 
            weightAndMove.first += getTileScore();
            
            if (tileAtMove->getOccupant()) {
                Token *occupant = tileAtMove->getOccupant();
                opponentPath = gameState.getPlayersPaths().at(occupant->getPlayerId());
                occupant->acceptVisitor(*this);
                weightAndMove.first += getTokenScore();
            }
        }
    }
    return weightMovePairs;

}


// level 4 AI visitor implementations - knows how to check tiles and enemy tokens

void Level4AI::visitTileTornado(const TileTornado& t) { 
    setTileScore(-1);
}
void Level4AI::visitTileBlackHole(const TileBlackHole& t) { 
    setTileScore(-2); 
}
void Level4AI::visitTileLucky(const TileLucky& t) { 
    int score = 1;
    std::pair<size_t, size_t> tilePosition = t.getPosition();

    setTileScore(score); 
}
void Level4AI::visitTileRosette(const TileRosette& t) { 
    setTileScore(5); 
}
void Level4AI::visitTileBasic(const TileBasic& t) { 
    setTileScore(0); 
}
void Level4AI::visitTileNull(const TileNull& t) { 
    setTileScore(0); 
}

void Level4AI::visitTokenBasic(const TokenBasic& o) { 
    setTokenScore(2); 
}
void Level4AI::visitTokenAssassin(const TokenAssassin& o) { 
    setTokenScore(4); 
}
void Level4AI::visitTokenFlexible(const TokenFlexible& o) { 
    setTokenScore(4); 
}
void Level4AI::visitTokenSpeedster(const TokenSpeedster& o) { 
    if (o.getManualAvailable()) setTokenScore(3); 
    else setTokenScore(2);
}
void Level4AI::visitTokenSupporter(const TokenSupporter& o) { 
    setTokenScore(5); 
}
