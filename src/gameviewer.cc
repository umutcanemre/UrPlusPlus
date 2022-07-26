#include "gameviewer.h"
#include "token.h"
#include "tile.h"

void GameViewer::drawPath(const std::vector<std::vector<Tile*>> &gameboard,
        const std::vector<Tile*> &path, const size_t playerId) {
    // is this cheese? probably not
    std::pair<size_t, size_t> pathCoord = path[0]->getPosition();

    std::vector<std::vector<char>> printBuffer(gameboard.size(),
        std::vector<char>(gameboard[0].size(), '?'));

    PathDirection dir = PathDirection::nulldir; // garbage intial values
    PathDirection newDir = PathDirection::nulldir;

    int rowDiff = 0, colDiff = 0;

    // build print buffer
    for (size_t i = 0; i < path.size()-1; ++i) {
        rowDiff = path[i+1]->getPosition().first - path[i]->getPosition().first;
        colDiff = path[i+1]->getPosition().second - path[i]->getPosition().second;

        if (rowDiff > 0) {
            newDir = PathDirection::down;
        }
        else if (rowDiff < 0) {
            newDir = PathDirection::up;
        }
        else if (colDiff > 0) {
            newDir = PathDirection::right;
        }
        else if (colDiff < 0) {
            newDir = PathDirection::left;
        }

        processPathSegment(dir, newDir, pathCoord, playerId, path.size() - (i+1));

        // if (newDir != dir) {
        //     printBuffer[pathCoord.first][pathCoord.second] = newDir;
        // }

        // else if (dir == '>' || dir == '<') {
        //     printBuffer[pathCoord.first][pathCoord.second] = '-';
        // }
        // else if (dir == 'v' || dir == '^') {
        //     printBuffer[pathCoord.first][pathCoord.second] = '|';
        // }

        dir = newDir;
        // update pathCoord
        pathCoord.first += rowDiff;
        pathCoord.second += colDiff;
    }

    pathCoord = path.back()->getPosition();

    processPathSegment(dir, newDir, pathCoord, playerId, 0);

    // printBuffer[pathCoord.first][pathCoord.second] = 'E';

    // print the print buffer
    // for (const auto &row : printBuffer) {
    //     for (const auto &c : row) {
    //         (*out) << c;
    //     }
    //     (*out) << std::endl;
    // }
}

char GameViewer::getTokenChar(const Token& t) {
    // unwise ASCII hacking
    return ('a' - (('0' + 1) * t.getPlayerId())) + t.getTokenId();
}

GameViewer::~GameViewer() {}
