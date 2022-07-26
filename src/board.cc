#include <memory>
#include <string>
#include <algorithm>
#include "board.h"
#include "tile.h"
#include "tilebasic.h"
#include "tileblackhole.h"
#include "tilelucky.h"
#include "tilerosette.h"
#include "tiletornado.h"
#include "tilenull.h"

#include "tokenbasic.h"
#include "tokenassassin.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

using namespace std;

const char* BoardParseException::what() {
    return message.c_str();
}

const vector<vector<Tile*>>& Board::getGameMap() const {
    return nonOwningGameMap;
}

const vector<vector<Token*>>& Board::getPlayersTokens() const {
    return nonOwningTokens;
}

const vector<vector<Tile*>>& Board::getPlayersPaths() const {
    return paths;
}

void Board::initCachedNonOwningVectors() {
    for (const auto &row : gameMap) {
        vector<Tile*> temp;
        for (const auto &tilePtr : row) {
            temp.emplace_back(tilePtr.get());
        }
        nonOwningGameMap.emplace_back(temp);
    }
    for (const auto &player : playersTokens) {
        vector<Token*> temp;
        for (const auto &tokenPtr : player) {
            temp.emplace_back(tokenPtr.get());
        }
        nonOwningTokens.emplace_back(temp);
    }
}

void Board::reset() {
    playersTokens.clear();
    paths.clear();
    gameMap.clear();
    nonOwningGameMap.clear();
    nonOwningGameMap.clear();
}

istream& operator>>(istream& in, Board& b) {
    b.reset();
    // read game map
    size_t width, height;

    in >> height >> width;

    char tileChar;

    vector<vector<bool>> unlandableTiles(height, vector<bool>(width, false));

    for (size_t i = 0; i < height; ++i) {
        b.gameMap.emplace_back();
        for (size_t j = 0; j < width; ++j) {
            in >> tileChar;
            switch (tileChar) {
                case 'O':
                    b.gameMap[i].emplace_back(make_unique<TileBasic>(i, j));
                    break;
                case 'B':
                    b.gameMap[i].emplace_back(make_unique<TileBlackHole>(i, j));
                    break;
                case 'L':
                    b.gameMap[i].emplace_back(make_unique<TileLucky>(i, j));
                    break;
                case 'T':
                    b.gameMap[i].emplace_back(make_unique<TileTornado>(i, j));
                    break;
                case 'X':
                    b.gameMap[i].emplace_back(make_unique<TileNull>(i, j));
                    unlandableTiles[i][j] = true;
                    break;
                case '*':
                    b.gameMap[i].emplace_back(make_unique<TileRosette>(i, j));
                    break;
                default:
                    throw BoardParseException{};
                    break;
            }
        }
    }

    // read in paths

    // store the actual coordinates of the paths,
    // will be useful for token validation later
    vector<vector<pair<size_t, size_t>>> pathsCoords
        (Board::playerCount, vector<pair<size_t, size_t>>());

    // read in the paths for each player
    string pathStr;
    pair<size_t, size_t> pathCoord;
    for (size_t i = 0; i < Board::playerCount; ++i) {
        b.paths.emplace_back();
        // 0-initialize a 2d array to track the path
        vector<vector<bool>> pathTracker(height, vector<bool>(width));

        // read in first tile of path (row, col)
        in >> pathCoord.first >> pathCoord.second;

        // out of bounds start
        if (pathCoord.second >= width || pathCoord.second < 0 ||
            pathCoord.first >= height || pathCoord.first < 0) {
            throw BoardParseException{};
        }
        // starting on unlandable tile (nulltile or start/end of previous path)
        if (unlandableTiles[pathCoord.first][pathCoord.second]) {
            throw BoardParseException{};
        }

        // other paths (and this one)
        // should not land on the start tile of this path later
        unlandableTiles[pathCoord.first][pathCoord.second] = true;

        // Mark start as visited
        pathTracker[pathCoord.first][pathCoord.second] = true;

        // add start token to the path
        b.paths[i].emplace_back(
            b.gameMap[pathCoord.first][pathCoord.second].get());
        pathsCoords[i].emplace_back(pathCoord);

        in >> pathStr;

        for (const auto &c : pathStr) {
            // move the current position of the path in the specified direction
            switch (c) {
                case 'N':
                    pathCoord.first--;
                    break;
                case 'E':
                    pathCoord.second++;
                    break;
                case 'S':
                    pathCoord.first++;
                    break;
                case 'W':
                    pathCoord.second--;
                    break;
                default:
                    throw BoardParseException{};
            }
            // path takes out of bounds
            if (pathCoord.second >= width || pathCoord.second < 0 ||
                pathCoord.first >= height || pathCoord.first < 0) {
                throw BoardParseException{};
            }
            // revisiting a tile in this path
            if (pathTracker[pathCoord.first][pathCoord.second]) {
                throw BoardParseException{};
            }
            // landing on unlandable tile (nulltile or start/end of previous path)
            if (unlandableTiles[pathCoord.first][pathCoord.second]) {
                throw BoardParseException{};
            }
            pathTracker[pathCoord.first][pathCoord.second] = true;

            // add visited token to the path
            b.paths[i].emplace_back(
                b.gameMap[pathCoord.first][pathCoord.second].get());
            pathsCoords[i].emplace_back(pathCoord);
        }

    }

    // read in token positions

    size_t tokenCount;

    // read number of tokens to expect for each player
    in >> tokenCount;
    char tokenTypeChar;
    size_t abilityUses;
    pair<size_t, size_t> tCoord;
    int row, col;

    // again, whichever player corresponds to the first start tile we read in
    // will be the first player we read tokens for

    for (size_t i = 0; i < Board::playerCount; ++i) {
        b.playersTokens.emplace_back();
        // tokens are id'd based on the order they appear
        // [token type char] [token row] [token col]
        for (size_t j = 0; j < tokenCount; ++j) {
            in >> tokenTypeChar >> row >> col;

            size_t pathDist;
            tCoord.first = static_cast<size_t>(row);
            tCoord.second = static_cast<size_t>(col);

            // special input for off board
            if (row == -1 && col == -1) {
                pathDist = 0;
            }
            // special input for off the board and complete
            else if (row == -2 && col == -2) {
                pathDist = pathsCoords[0].size() + 1;
            }
            else if (row < 0 || col < 0) {
                throw BoardParseException();
            }
            else {
                auto tileIter = find(pathsCoords[i].begin(), pathsCoords[i].end(), tCoord);
                // if the token position is not on the player's path we have error
                if (tileIter == pathsCoords[i].end()) {
                    throw BoardParseException();
                }
                pathDist = distance(pathsCoords[i].begin(), tileIter) + 1;
            }
            switch (tokenTypeChar) {
                case 'A':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenAssassin>
                            (i, j, tCoord.first, tCoord.second, pathDist));
                    break;
                case 'B':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenBasic>
                            (i, j, tCoord.first, tCoord.second, pathDist));
                    break;
                case 'F':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenFlexible>
                            (i, j, tCoord.first, tCoord.second, pathDist));
                    break;
                case 'S':
                    // read additional parameter for number of uses on ability
                    in >> abilityUses;

                    b.playersTokens[i].emplace_back
                        (make_unique<TokenSpeedster>
                            (i, j, tCoord.first, tCoord.second, pathDist,
                            abilityUses));
                    break;
                case 'G':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenSupporter>
                            (i, j, tCoord.first, tCoord.second, pathDist));
                    break;
                // invalid token type
                default:
                    throw BoardParseException{};
                    break;
            }

            // set occupant of tile
            if (pathDist > 0 && pathDist < pathsCoords[0].size() + 1) {
                Tile& tile = *b.gameMap[tCoord.first][tCoord.second];
                // token already on this tile
                if (tile.getOccupant()) {
                    throw BoardParseException{};
                }
                tile.setOccupant(b.playersTokens[i][j].get());
            }
        }
    }

    b.initCachedNonOwningVectors();

    return in;
}
