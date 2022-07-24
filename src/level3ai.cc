#include <iostream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <vector>

#include "level3ai.h"
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

// level 3 AI
Level3AI::Level3AI(): AI() {}
Level3AI::~Level3AI() {}
pair<size_t, size_t> Level3AI::findMove(const GameState &gameState) {
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
    cout << candidates.at(index).first << candidates.at(index).second.first 
        << candidates.at(index).second.second << endl;
    return candidates.at(index).second;
}

vector<pair<int, pair<size_t, size_t>>> Level3AI::assignPriorities( 
    vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
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

    vector<pair<int, pair<size_t, size_t>>> movesAndWeights;
    const std::vector<Tile*> playerPath = gameState.getPlayersPaths().at(getPlayerId());
    std::vector<Token*> tokens = gameState.getPlayersTokens().at(getPlayerId());

    for (auto x : movelist) {
        movesAndWeights.emplace_back(0, x);
    }
    
    // assign weight 1 to item 3, weight 2 to item 2, weight 3 to item 1
    for (auto &x : movesAndWeights) {
        Token *token = tokens.at(x.second.first);
        size_t newIndex = token->getPathProgress() + x.second.second - 1;
        // item 1 - end of path is reached
        if (newIndex == playerPath.size()) {
            x.first += 4;
        }
        // item 2,3,4 (exclusive with item 1) but not each other
        else {
            Token *occupant = playerPath.at(newIndex)->getOccupant();
            if (occupant) {
                const std::vector<Tile*> opponentPath = gameState.getPlayersPaths().at(occupant->getPlayerId());
            }
            playerPath.at(newIndex)->acceptVisitor(*this); // sets 
            x.first += getTileScore();
        }
    }
    return movesAndWeights;

}


// level 3 AI visitor implementations - knows how to check tiles and enemy tokens

void Level3AI::visitTileTornado(const TileTornado& t) { setTileScore(-1);}
void Level3AI::visitTileBlackHole(const TileBlackHole& t) { setTileScore(-2); }
void Level3AI::visitTileLucky(const TileLucky& t) { setTileScore(1); }
void Level3AI::visitTileRosette(const TileRosette& t) { setTileScore(3); }
void Level3AI::visitTileBasic(const TileBasic& t) { setTileScore(0); }
void Level3AI::visitTileNull(const TileNull& t) { setTileScore(0); }

void Level3AI::visitTokenBasic(const TokenBasic& o) { setTokenScore(1); }
void Level3AI::visitTokenAssassin(const TokenAssassin& o) { setTokenScore(3); }
void Level3AI::visitTokenFlexible(const TokenFlexible& o) { setTokenScore(3); }
void Level3AI::visitTokenSpeedster(const TokenSpeedster& o) { 
    if (o.getManualAvailable()) setTokenScore(2); 
    else setTokenScore(1);
}
void Level3AI::visitTokenSupporter(const TokenSupporter& o) { setTokenScore(4); }
