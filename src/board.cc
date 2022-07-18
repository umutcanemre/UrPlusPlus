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

#include "tokenbasic.h"
#include "tokenassassin.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

using namespace std;

char* BoardParseException::what() {
    return "Board parsing error";
}

const std::vector<std::vector<Tile*>>& Board::getGameMap() const {
    return nonOwningGameMap;
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

    in >> width >> height;

    char tileChar;

    // (row, col) pairs for starts and ends
    vector<pair<size_t,size_t>> starts;
    vector<pair<size_t,size_t>> ends;


    for (size_t i = 0; i < height; ++i) {
        b.gameMap.emplace_back();
        b.nonOwningGameMap.emplace_back();
        for (size_t j = 0; j < width; ++j) {
            in >> tileChar;
            switch (tileChar) {
                case 'O':
                    b.gameMap[i].emplace_back(make_unique<TileBasic>());
                    break;
                case 'B':
                    b.gameMap[i].emplace_back(make_unique<TileBlackHole>());
                    break;
                case 'L':
                    b.gameMap[i].emplace_back(make_unique<TileLucky>());
                    break;
                case 'T':
                    b.gameMap[i].emplace_back(make_unique<TileTornado>());
                    break;
                case '*':
                    b.gameMap[i].emplace_back(make_unique<TileRosette>());
                    break;
                default:
                    throw BoardParseException{};
                    break;
                b.nonOwningGameMap[i].emplace_back(b.gameMap[i][j].get());
            }
        }
    }


    // // read in paths
    size_t xPos, yPos;

    // // store the actual coordinates of the paths, will be useful for token validation later
    vector<vector<pair<size_t, size_t>>> pathsCoords
        (Board::playerCount, vector<pair<size_t, size_t>>());

    string pathStr;

    for (size_t i = 0; i < Board::playerCount; ++i) {
        in >> xPos >> yPos;
        if (xPos >= width || xPos < 0 ||
            yPos >= height || yPos < 0) {
            throw BoardParseException{};
        }

        // 0-initialize a 2d array
        vector<vector<int>> pathTracker(height, vector<int>(width));
        // Mark start as visited
        ++pathTracker[yPos][xPos];
        
        b.paths.emplace_back();

        in >> pathStr;

        for (const auto &c : pathStr) {
            switch (c) {
                case 'N':
                    yPos--;
                    break;
                case 'E':
                    xPos++;
                    break;
                case 'S':
                    yPos++;
                    break;
                case 'W':
                    xPos--;
                    break;
                default:
                    throw BoardParseException{};
            }
            // path takes out of bounds
            if (xPos >= width || xPos < 0 ||
                yPos >= height || yPos < 0) {
                throw BoardParseException{};
            }
            // revisiting a tile
            if (pathTracker[yPos][xPos] >= 1) {
                throw BoardParseException{};
            }
            ++pathTracker[yPos][xPos];

            b.paths[i].emplace_back(b.gameMap[yPos][xPos].get());
            pathsCoords[i].emplace_back(make_pair(yPos, xPos));
        }
    }

    // Check for end tiles along other player path
    for (size_t j = 0; j < Board::playerCount; ++j) {
        for (size_t k = 0; k < Board::playerCount; ++k) {
            if (j == k) {
                continue;
            }
            if (find(b.paths[k].begin(), b.paths[k].end(), b.paths[j].back()) != b.paths[k].end()) {
                // End tile found along other path; invalid board
                // This implementation also enforces no shared end tile (intentional)
                throw BoardParseException{};
            }
        }
    }

    // // read in token positions

    // // b.playersTokens = vector<vector<unique_ptr<Token>>>(Board::playerCount, vector<unique_ptr<Token>>());

    size_t tokenCount;

    in >> tokenCount;
    char tokenTypeChar;
    pair<size_t, size_t> tokenCoord;

    // // again, whichever player corresponds to the first start tile we read in
    // // will be the first player we read tokens for

    for (size_t i = 0; i < Board::playerCount; ++i) {
        b.playersTokens.emplace_back();
        b.nonOwningTokens.emplace_back();
        // tokens are id'd based on the order they appear
        // [token type char] [token row] [token col]
        for (size_t j = 0; j < tokenCount; ++j) {
            in >> tokenTypeChar >> tokenCoord.first >> tokenCoord.second;

            // if the token's position is not on the player's path we have error
            if (find(pathsCoords[i].begin(), pathsCoords[i].end(), tokenCoord)
                == pathsCoords[i].end()) {
                throw BoardParseException();
            }

            switch (tokenTypeChar) {
                case 'A':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenAssassin>
                            (i, j, tokenCoord.first, tokenCoord.second));
                    break;
                case 'B':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenBasic>
                            (i, j, tokenCoord.first, tokenCoord.second));
                    break;
                case 'F':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenFlexible>
                            (i, j, tokenCoord.first, tokenCoord.second));
                    break;
                case 'S':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenSpeedster>
                            (i, j, tokenCoord.first, tokenCoord.second));
                    break;
                case 'G':
                    b.playersTokens[i].emplace_back
                        (make_unique<TokenSupporter>
                            (i, j, tokenCoord.first, tokenCoord.second));
                    break;
                // invalid token type
                default:
                    throw BoardParseException{};
                    break;
                b.nonOwningTokens[i].emplace_back(b.playersTokens[i][j].get());
            }
        }
    }
    return in;
}


