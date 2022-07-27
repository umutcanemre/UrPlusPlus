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
    vector<pair<float, pair<size_t, size_t>>> weightedMovelist = assignPriorities(movelist,gameState);

    // first loop: find the highest priority
    float maxWeight = weightedMovelist.at(0).first;
    maxWeight = max_element(weightedMovelist.begin(), weightedMovelist.end(),
        [](const pair<float, pair<size_t, size_t>> &a, const pair<float, pair<size_t, size_t>> &b) {
            return (a.first < b.first);
        }
    )->first;

    // pick randomly if multiple moves are weighted the same
    vector<pair<float, pair<size_t, size_t>>> candidates;
    for (auto x: weightedMovelist) {
        if (maxWeight == x.first) {
            candidates.emplace_back(x);
        }
    }

    size_t index = static_cast<size_t>(rand() % candidates.size());
    return candidates.at(index).second;
}

vector<pair<float, pair<size_t, size_t>>> Level4AI::assignPriorities(
    const vector<pair<size_t, size_t>> &movelist, const GameState &gameState) {
    // Level 4: an AI that favours certain beneficial moves over others and also is capable
    // of using abilities at the right times.

    // This is the order of preference: (highest to lowest priority)
    // 1. Most strongly favours moves that end the current token's run. +6
    // 2. Favour moves that land on a rosette on a shared path (+5) and rosettes (+3).
    // 3. Favour moves that steal someone else’s token. +S where S = x + y, x is
    //    token tastiness and y is capture ability modifier
    // 4. Favours moves that end up protecting a special token, slightly favours moves that
    //    protect a normal token (+3, +2)
    // 5. Slightly penalizes moves that end up losing protection by rosette or supporter (-1)
    // 6. Avoid moves that land on black holes. -6


<<<<<<< HEAD
    // 7. Penalizes moves that land on a tile 1, 2 or 3 ahead of an enemy token without protection 
    //      (-2 each, -3 for distance 2) ✓
    // 8. if pathProgress + distance - 1 = lucky, +luckyDistanceCalculation() ✓
    // 9. if pathProgress + distance - 1 = tornado, +tornadoDistanceCalculation() ✓
=======
    // 7. Penalizes moves that land on a tile 2 or 3 ahead of an enemy token without protection (-2 each)
    // 8. if pathProgress + distance - 1 = lucky, +luckyDistanceCalculation()
    // 9. if pathProgress + distance - 1 = tornado, +tornadoDistanceCalculation()
>>>>>>> 5a4f365d60f6338f8e1862c5a4c63e76e2c6e55b

    vector<pair<float, pair<size_t, size_t>>> weightMovePairs;
    allPathsOnBoard = gameState.getPlayersPaths();
    const vector<Tile*> playerPath = allPathsOnBoard.at(getPlayerId());
    vector<Token*> myTokens = gameState.getPlayersTokens().at(getPlayerId());

    for (const auto &x : movelist) {
        weightMovePairs.emplace_back(0, x);
    }

    // assign weight 1 to item 3, weight 2 to item 2, weight 3 to item 1
    for (auto &weightAndMove : weightMovePairs) {
        currentToken = myTokens.at(weightAndMove.second.first);
        size_t newIndex = currentToken->getPathProgress() + weightAndMove.second.second - 1;

        // item 1 - end of path is reached
        if (newIndex == playerPath.size()) {
            weightAndMove.first += 6;
        }

        else { // end of path was not reached
            Tile *tileAtMove = playerPath.at(newIndex);
            Tile *currentTile;
            if (currentToken->getPathProgress() > 0) {
                currentTile = playerPath.at(currentToken->getPathProgress() - 1);
            }
            else {currentTile = nullptr;}
            // check the tile at the next move and see how incentivized we are to move to that tile
            tileAtMove->acceptVisitor(*this);
            weightAndMove.first += getTileScore(); // accounts for all of Rosette, Lucky, BH, and Tornado
            
            // check for capture incentives
            if (tileAtMove->getOccupant()) {
                Token *occupant = tileAtMove->getOccupant();
                vector<Tile*> opponentPath = allPathsOnBoard.at(occupant->getPlayerId());
                occupant->acceptVisitor(*this);
                weightAndMove.first += getTokenScore();

                // Add to weight if has capture ability
                if (currentToken->activateCapture()) { 
                    weightAndMove.first += 2;
                }
            }




            // Would making this move most likely put us at risk of capture?
            // and if isAtRisk, how much do we care about this token?
            // however this is completely negated if one of the occupants behind you is the supporter or if you
            // are currently not on anything

            // After the tile lands on a tornado or black hole none of this is relevant as it will be sent to a
            // new location anyway.
            bool currentlySupported = false;
            if (currentTile) {
                currentTile->acceptVisitor(*this);
                currentlySupported = currentToken->getIsProtected() || tileId=='*';
            }

            bool wouldBeSupported = false;
            bool tileMovesToken = false;
            try {
                Tile* inFrontOfYou = playerPath.at(currentToken->getPathProgress() - 1 + weightAndMove.second.second);
                inFrontOfYou->acceptVisitor(*this);
                if (tileId == '*') wouldBeSupported = true;
                else if (tileId == 'T') {
                    wouldBeSupported = false;
                    tileMovesToken = true;
                } // tile gets moved to a different place so not relevant
                else if (tileId == 'B') continue; // black hole sends to beginning so this fails to be relevant
            } catch (out_of_range &) { } // do nothing

            // if we already know we will be supported by a Rosette, we don't need to check Supporter protection
            if (!wouldBeSupported && !tileMovesToken) {
                // the supporter check
                for (int i=1; i<4; i++) {
                    try {
                        Tile* behindYou = playerPath.at(currentToken->getPathProgress() + weightAndMove.second.second - 1 - i);
                        if (behindYou->getOccupant()) {
                            behindYou->getOccupant()->acceptVisitor(*this);
                        } else {
                            continue;
                        }
                        if (behindYou->getOccupant() && tokenId == 'G' &&
                            behindYou->getOccupant()->getPlayerId() == getPlayerId() &&
                            behindYou->getOccupant()->getTokenId() != currentToken->getTokenId()) // make sure guardian doesn't check itself
                            { 
                            // check if guardian guards you
                            Token* guardian = behindYou->getOccupant();
<<<<<<< HEAD
                            int j = 0;
=======
                            int j;
>>>>>>> 5a4f365d60f6338f8e1862c5a4c63e76e2c6e55b
                            for (j=1;j<i;j++) { // only need to check the i positions in front of it for occupation
                                try {
                                    playerPath.at(guardian->getPathProgress() - 1 + j);
                                    if (playerPath.at(guardian->getPathProgress() - 1 + j)->getOccupant() &&
                                        playerPath.at(guardian->getPathProgress() - 1 + j)->getOccupant()->getPlayerId()
                                        == getPlayerId()) {
                                        // if anyone that is at an index closer to the Guardian than you are
                                        // has your playerID, they would be supported instead
                                            wouldBeSupported = false;
                                            break;
                                        }
                                } catch (out_of_range &) {
                                    break;
                                }
                            }
                            if (j == i) { wouldBeSupported = true; }
                            break;
                        }
                    } catch (out_of_range &) {
                        // no tiles to be worried about
                        break;
                    }
                }
            }

            // check out what the current token is
            // if making this move will make you gain protection...
            currentToken->acceptVisitor(*this);
            if (wouldBeSupported && !currentlySupported && (tokenId == 'B' ||
                tokenId == 'A' || (tokenId == 'S' && !currentToken->getManualAvailable()))) {
                // the idea is that basic tokens are less valuable
                // and assassins are meant to be attackers so it will be beneficial for them to move
                // whenever possible
                // speedster becomes basic when the manual is gone
                weightAndMove.first += 2;
            }
            else if (wouldBeSupported && !currentlySupported) {
                // tokenId == 'G', 'S', 'F', any other special token to be added
                weightAndMove.first += 3;
            }

            // Would NOT making this move put us at risk of capture?
            // the opponent is most likely to roll a 1 2 or 3 - this accounts for 7/8 cases, with 2 
            // accounting for most of that.
            // if one (or more) of these squares are occupied and you are not supported, we slightly 
            // incentivize moving off the current square.
            if (!currentlySupported) {
                for (int i=1; i<4; i++) {
                    try {
                        Tile* behindYou = playerPath.at(currentToken->getPathProgress() - 1 - i);
                        if (behindYou->getOccupant() && 
                            behindYou->getOccupant()->getPlayerId() != getPlayerId()) {
                            if (i == 2 && // incentivize more if 2
                                isTileOnPlayersPath(tileAtMove->getPosition(), 
                                behindYou->getOccupant()->getPlayerId())) {  
                                    ++weightAndMove.first;
                            }
                            weightAndMove.first += 2;
                        }
                    } catch (out_of_range &) {
                        // no more tiles to be worried about
                        break;
                    }
                }
            }

            // next we check if MAKING this move will put us at risk of capture...
            if (!wouldBeSupported && !tileMovesToken) {
                for (int i=1; i<4; i++) {
                    try {
                        Tile* behindYou = playerPath.at(currentToken->getPathProgress() +
                            weightAndMove.second.second - 1 - i);
                        if (behindYou->getOccupant() && behindYou->getOccupant()->getPlayerId() != getPlayerId()) {
                            if (i == 2 && // disincentivize more if 2
                                isTileOnPlayersPath(tileAtMove->getPosition(), 
                                behindYou->getOccupant()->getPlayerId())) {
                                    --weightAndMove.first;
                            }
                            weightAndMove.first -= 2;
                        }
                    } catch (out_of_range &) {
                        // no more tiles to be worried about
                        break;
                    }
                }
            }

            // are we disincentivized to move off of this tile? Namely, this tile is a Rosette on a shared path.
            // slightly disincentivize this as opponent would gain invulnerability
            // but not by too much
            if (currentToken->getPathProgress() != 0) {
                Tile *curTile = playerPath.at(currentToken->getPathProgress() - 1);
                curTile->acceptVisitor(*this);
                if (tileId == '*') {
                    if (isTileOnSharedPath(curTile->getPosition())) {
                        --weightAndMove.first;
                    } else {
                        ++weightAndMove.first; // we want to incentivize leaving a rosette that is not on a shared path
                        // because this allows another token to step on the rosette
                    }
                }
            }
        }
        // if you want to view the weights and all possible moves:
        cout << "Weight " << weightAndMove.first << ": Token " << weightAndMove.second.first
            << ", distance " << weightAndMove.second.second << endl;
    }
    return weightMovePairs;

}


// level 4 AI visitor implementations - knows how to check tiles and enemy tokens

void Level4AI::visitTileTornado(const TileTornado& t) {
    tileId = 'T';
    vector<Tile*> playerPath = allPathsOnBoard.at(getPlayerId());
    pair<size_t, size_t> tilePosition = t.getPosition();
    // since no tile must be repeated in the path...
    auto iterator = find_if(playerPath.begin(), playerPath.end(),
    [tilePosition](Tile* tileInPath) {
        return tilePosition.first == tileInPath->getPosition().first
        && tilePosition.second == tileInPath->getPosition().second;
    }); // pointer to a tile pointer
    int index = iterator - playerPath.begin();
    float fwdWeight = 0, backWeight = 0;
    int i;

    for (i=index+1; i<playerPath.size(); i++) {
        if (!playerPath[i]->getOccupant()) {
            // we do not check special effects after a tornado moves you onto a special tile, so we only
            // need to know the distance in the path
            fwdWeight = static_cast<float>(i-index) / static_cast<float>(playerPath.size());
            break;
        }
    }

    if (i == playerPath.size()) { // sends to end
        fwdWeight = 1;
    }
    for (i=index-1; i>=0; i--) {
        if (!playerPath[i]->getOccupant() || playerPath[i]->getOccupant() == currentToken) { // pointer equality
            // we do not check special effects after a tornado moves you onto a special tile, so we only
            // need to know the distance in the path
            backWeight = static_cast<float>(index-i) / static_cast<float>(playerPath.size());
            break;
        }
    }
    if (i == -1) { // sends to beginning
        backWeight = 1;
    }

    setTileScore(6 * fwdWeight - 6 * backWeight);
}
void Level4AI::visitTileBlackHole(const TileBlackHole& t) {
    tileId = 'B';
    setTileScore(-6);
}
void Level4AI::visitTileLucky(const TileLucky& t) {
    tileId = 'L';
    pair<size_t, size_t> tilePosition = t.getPosition();
    vector<Tile*> playerPath = allPathsOnBoard.at(getPlayerId());
    // since no tile must be repeated in the path...
    auto iterator = find_if(playerPath.begin(), playerPath.end(),
    [tilePosition](Tile* tileInPath) {
        return tilePosition.first == tileInPath->getPosition().first
        && tilePosition.second == tileInPath->getPosition().second;
    }); // pointer to a tile pointer

    int index = iterator - playerPath.begin();
    float fwdWeight = static_cast<float>((playerPath.size() - index)) / static_cast<float>(playerPath.size()) ;
    float backWeight = static_cast<float>((index+1)) / static_cast<float>(playerPath.size());
    setTileScore(6 * (fwdWeight / 16 - backWeight / 16));
    // probability of rolling a 4 and a 0 respectively
    // multiplied by 6 because that is the weight we decided on for starting and ending a path for level 4s AI
}

bool Level4AI::isTileOnSharedPath(const pair<size_t, size_t> &tilePosition) {
    vector<Tile*> opponentPath;
    bool tileIsInPath;
    for (size_t i=0; i < allPathsOnBoard.size(); i++) {
        if (i != getPlayerId()) {
            tileIsInPath = isTileOnPlayersPath(tilePosition, i);

            if (tileIsInPath) { // we have found an opponent we can block
                return true;
            }
        }
    }
    return false;
}

bool Level4AI::isTileOnPlayersPath(const pair<size_t, size_t> &tilePosition, size_t pId) {
    // returns whether or not the tile is on this specific player's path
    vector<Tile*> opponentPath = allPathsOnBoard.at(pId);
    return find_if(opponentPath.begin(), opponentPath.end(),
    [tilePosition](Tile* tileInPath) {
        return tilePosition.first == tileInPath->getPosition().first
        && tilePosition.second == tileInPath->getPosition().second;
    }) != opponentPath.end();
}

void Level4AI::visitTileRosette(const TileRosette& t) {
    tileId = '*';
    pair<size_t, size_t> tilePosition = t.getPosition();
    vector<Tile*> opponentPath;
    bool tileIsInPath = isTileOnSharedPath(tilePosition);
    // else - this tile is not in any opponent paths
    if (tileIsInPath) {
        setTileScore(5); return;
    }
    setTileScore(3);

}
void Level4AI::visitTileBasic(const TileBasic& t) {
    tileId = 'O';
    setTileScore(0);
}
void Level4AI::visitTileNull(const TileNull& t) {
    tileId = 'X';
    setTileScore(0);
}

void Level4AI::visitTokenBasic(const TokenBasic& o) {
    tokenId = 'B';
    setTokenScore(2);
}
void Level4AI::visitTokenAssassin(const TokenAssassin& o) {
    tokenId = 'A';
    setTokenScore(4);
}
void Level4AI::visitTokenFlexible(const TokenFlexible& o) {
    tokenId = 'F';
    setTokenScore(4);
}
void Level4AI::visitTokenSpeedster(const TokenSpeedster& o) {
    tokenId = 'S';
    if (o.getManualAvailable()) setTokenScore(3);
    else setTokenScore(2);
}
void Level4AI::visitTokenSupporter(const TokenSupporter& o) {
    tokenId = 'G';
    setTokenScore(5);
}
