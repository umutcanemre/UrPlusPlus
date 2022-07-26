#include <vector>
#include <iostream>
#include <algorithm>
#include "board.h"
#include "cliview.h"
#include "gamestate.h"
#include "token.h"
#include "tile.h"

#include "tokenbasic.h"
#include "tokenassassin.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

CLIView::CLIView(std::ostream* out): GameViewer{}, out{out} {}
CLIView::~CLIView() {}

void CLIView::drawBoard(const std::vector<std::vector<Tile*>> &gameboard) {
    for (const auto &row : gameboard) {
        for (const auto &t : row) {
            if (!t->getOccupant()) {
                t->acceptVisitor(*this);
            }
            else {
                t->getOccupant()->acceptVisitor(*this);
            }
        }
        (*out) << std::endl;
    }
}

void CLIView::processPathSegment(PathDirection curDir, PathDirection nextDir,
        std::pair<size_t, size_t> pathCoord,
        const size_t playerId, const size_t tilesRemaining) {
    if (tilesRemaining == 0) {
        printBuffer[pathCoord.first][pathCoord.second] = 'E';
    }

    else if (nextDir != curDir) {
        char output;
        switch (nextDir) {
            case PathDirection::right:
                output = '>';
                break;
            case PathDirection::up:
                output = '^';
                break;
            case PathDirection::left:
                output = '<';
                break;
            case PathDirection::down:
                output = 'v';
                break;
            default:
                break;
        }
        printBuffer[pathCoord.first][pathCoord.second] = output;
    }

    else if (curDir == PathDirection::right || curDir == PathDirection::left) {
        printBuffer[pathCoord.first][pathCoord.second] = '-';
    }
    else if (curDir == PathDirection::down || curDir == PathDirection::up) {
        printBuffer[pathCoord.first][pathCoord.second] = '|';
    }
}

// void CLIView::drawPath(const std::vector<std::vector<Tile*>> &gameboard,
//     const std::vector<Tile*> &path) const {

//     // is this cheese? probably not
//     std::pair<size_t, size_t> pathCoord = path[0]->getPosition();

//     std::vector<std::vector<char>> printBuffer(gameboard.size(),
//         std::vector<char>(gameboard[0].size(), '?'));

//     char dir = '!'; // garbage intial values
//     char newDir = '?';

//     int rowDiff = 0, colDiff = 0;

//     // build print buffer
//     for (size_t i = 0; i < path.size()-1; ++i) {
//         rowDiff = path[i+1]->getPosition().first - path[i]->getPosition().first;
//         colDiff = path[i+1]->getPosition().second - path[i]->getPosition().second;

//         if (rowDiff > 0) {
//             newDir = 'v';
//         }
//         else if (rowDiff < 0) {
//             newDir = '^';
//         }
//         else if (colDiff > 0) {
//             newDir = '>';
//         }
//         else if (colDiff < 0) {
//             newDir = '<';
//         }

//         if (newDir != dir) {
//             printBuffer[pathCoord.first][pathCoord.second] = newDir;
//         }

//         else if (dir == '>' || dir == '<') {
//             printBuffer[pathCoord.first][pathCoord.second] = '-';
//         }
//         else if (dir == 'v' || dir == '^') {
//             printBuffer[pathCoord.first][pathCoord.second] = '|';
//         }

//         dir = newDir;
//         // update pathCoord
//         pathCoord.first += rowDiff;
//         pathCoord.second += colDiff;
//     }

//     pathCoord = path.back()->getPosition();

//     printBuffer[pathCoord.first][pathCoord.second] = 'E';

//     // print the print buffer
//     for (const auto &row : printBuffer) {
//         for (const auto &c : row) {
//             (*out) << c;
//         }
//         (*out) << std::endl;
//     }
// }

void CLIView::drawInfoTokens(const std::vector<std::vector<Token*>>& playersTokens,
    const std::string &info,
    std::function<bool(const Token*)> pred) const {
    for (size_t i = 0; i < Board::playerCount; ++i) {
        auto & playerTokens = playersTokens[i];
        (*out) << "Player " << i << "'s " << info <<  " tokens: ";
        for (const auto & t : playerTokens) {
            if (pred(t)) {
                (*out) << getTokenChar(*t) << ' ';
            }
        }
        (*out) << std::endl;
    }
}

void CLIView::outputPrintBuffer() const {
    for (const auto &row : printBuffer) {
        for (const auto &c : row) {
            (*out) << c;
        }
        (*out) << std::endl;
    }
}

void CLIView::doNotify(const GameState& g) {
    for (size_t i = 0; i < 10; ++i) {
        (*out) << '~';
    }
    (*out) << std::endl;

    // print Player turn
    (*out) << "Player " << g.getPlayerTurn() << "'s turn" << std::endl;

    // print dice roll
    if (g.requiresDiceRoll()) {
        (*out) << "Needs to roll dice!" << std::endl;
    } else {
        (*out) << "Dice Roll: " << g.getDiceRoll() << ", Flex Dice Roll:"
            << g.getFlexDiceRoll() << std::endl;
    }

    // abilities available print
    // ...

    drawInfoTokens(g.getPlayersTokens(),
        "game",
        [](const Token *t) { return true; });
    (*out) << std::endl;

    // print offboard tokens
    drawInfoTokens(g.getPlayersTokens(),
        "offboard",
        [](const Token *t) { return t->getPathProgress() == 0; });

    (*out) << std::endl;

    // print the board
    (*out) << "Board State:" << std::endl;
    drawBoard(g.getBoard());

    (*out) << std::endl;

    // print winning tokens
    size_t pathLength = g.getPlayersPaths()[0].size();
    drawInfoTokens(g.getPlayersTokens(),
        "victorious",
        [pathLength](const Token *t) { return t->getPathProgress() - 1 == pathLength; });

    (*out) << std::endl;

    // print player paths
    for  (size_t i = 0; i < g.getPlayersPaths().size(); ++i) {
        printBuffer = std::vector<std::vector<char>>(g.getBoard().size(),
            std::vector<char>(g.getBoard()[0].size(), '?'));

        (*out) << "Player " << i << " game path:" << std::endl;
        drawPath(g.getBoard(), g.getPlayersPaths()[i], i);
        outputPrintBuffer();
    }
}


void CLIView::visitTileTornado(const TileTornado& t) {
    (*out) << 'T';
}

void CLIView::visitTileBlackHole(const TileBlackHole& t) {
    (*out) << 'B';
}

void CLIView::visitTileLucky(const TileLucky& t) {
    (*out) << 'L';
}

void CLIView::visitTileRosette(const TileRosette& t) {
    (*out) << '*';
}

void CLIView::visitTileBasic(const TileBasic& t) {
    (*out) << 'O';
}

void CLIView::visitTileNull(const TileNull& t) {
    (*out) << ' ';
}


void CLIView::visitTokenBasic(const TokenBasic& t) {
    (*out) << getTokenChar(t);
}

void CLIView::visitTokenAssassin(const TokenAssassin& t) {
    (*out) << getTokenChar(t);
}

void CLIView::visitTokenFlexible(const TokenFlexible& t) {
    (*out) << getTokenChar(t);
}

void CLIView::visitTokenSpeedster(const TokenSpeedster& t) {
    (*out) << getTokenChar(t);
}

void CLIView::visitTokenSupporter(const TokenSupporter& t) {
    (*out) << getTokenChar(t);
}
