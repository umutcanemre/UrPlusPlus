#include <functional>
#include "board.h"
#include "gamestate.h"

#include "guiview.h"
#include "sdl_wrap.h"

#include "tile.h"

#include "tokenbasic.h"
#include "tokenassassin.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

Colour GUIView::getTokenColour(const Token& t) {
    // red is player 0 blue is player 1
    return (t.getPlayerId() == 0) ?
        Colour{'\xff', '\x00', '\x00', '\xff'}
        : Colour{'\x00', '\x00', '\xff', '\xff'};
}

void GUIView::processPathSegment(PathDirection curDir,
        PathDirection nextDir, std::pair<size_t, size_t> pathCoord,
        const size_t playerId, const size_t tilesRemaining) {

    // unwise ASCII hacking
    char playerIdSuffix = playerId + '0';

    std::string imageKey;
    // start or end tile
    if (tilesRemaining == 0) {
        switch (nextDir) {
            case PathDirection::right:
                imageKey = "ra";
                break;
            case PathDirection::up:
                imageKey = "ua";
                break;
            case PathDirection::left:
                imageKey = "la";
                break;
            case PathDirection::down:
                imageKey = "da";
                break;
            default:
                break;
        }
    }

    else if (nextDir != curDir && curDir != PathDirection::nulldir) {
        if ((curDir == PathDirection::up && nextDir == PathDirection::right) ||
            (curDir == PathDirection::left && nextDir == PathDirection::down)) {
            imageKey = "ur";
        }
        else if ((curDir == PathDirection::up && nextDir == PathDirection::left) ||
            (curDir == PathDirection::right && nextDir == PathDirection::down)) {
            imageKey = "ul";
        }
        else if ((curDir == PathDirection::down && nextDir == PathDirection::right) ||
            (curDir == PathDirection::left && nextDir == PathDirection::up)) {
            imageKey = "dr";
        }
        else if ((curDir == PathDirection::down && nextDir == PathDirection::left) ||
            (curDir == PathDirection::right && nextDir == PathDirection::up)) {
            imageKey = "dl";
        }
    }

    else if (curDir == PathDirection::right || curDir == PathDirection::left
        || nextDir == PathDirection::right || nextDir == PathDirection::left) {
        imageKey = "horz";
    }
    else if (curDir == PathDirection::down || curDir == PathDirection::up
        || nextDir == PathDirection::down || nextDir == PathDirection::up) {
        imageKey = "vert";
    }

    imageKey = imageKey + playerIdSuffix;

    int pathSegmentX = boardStartX + (pathCoord.second * dimension);
    int pathSegmentY = boardStartY + (pathCoord.first * dimension);

    screen->draw_img(imageKey, pathSegmentX, pathSegmentY);
}

void GUIView::drawBoard(const std::vector<std::vector<Tile*>> &gameboard) {
    // pre: number of squares fit otherwise LOL
    xPos = boardStartX;
    yPos = boardStartY;

    for (const auto &row : gameboard) {
        xPos = boardStartX;
        for (const auto &tile : row) {
            tile->acceptVisitor(*this);
            xPos += dimension;
        }
        yPos += dimension;
    }
}

void GUIView::drawBoardTokens(const std::vector<std::vector<Tile*>> &gameboard) {
    // pre: number of squares fit otherwise LOL
    xPos = boardStartX;
    yPos = boardStartY;

    for (const auto &row : gameboard) {
        xPos = boardStartX;
        for (const auto &tile : row) {
            if (tile->getOccupant()) {
                tile->getOccupant()->acceptVisitor(*this);
                if (tile->getOccupant()->getIsProtected()) {
                    screen->draw_img("protected", xPos, yPos);
                }
            }
            xPos += dimension;
        }
        yPos += dimension;
    }
}

void GUIView::drawInfoTokens(const std::vector<std::vector<Token*>>& playersTokens,
    const std::pair<int, int> &startXY,
    const std::pair<int, int> &playerIncXY, const std::pair<int, int> &incXY,
    std::function<bool(const Token*)> pred) {

    xPos = startXY.first;
    yPos = startXY.second;

    for (size_t i = 0; i < Board::playerCount; ++i) {
        auto & playerTokens = playersTokens[i];
        for (const auto & t : playerTokens) {
            if (pred(t)) {
                t->acceptVisitor(*this);
                xPos += incXY.first;
                yPos += incXY.second;
            }
        }
        xPos = startXY.first;
        yPos = startXY.second;

        xPos += playerIncXY.first;
        yPos += playerIncXY.second;
    }
}

void GUIView::drawHeader(const std::string message, const std::pair<int, int> &startXY, const Colour &c) {
    screen->draw_string(message,
        startXY.first, startXY.second, c);
}

void GUIView::doNotify(const GameState& state) {
    // initialize the top left corner of the board
    boardStartY = (screen->getHeight() - (state.getBoard().size() * dimension))/2;

    // make additional room for some UI on the right of the board
    boardStartX = (screen->getWidth() - 4 * dimension - (state.getBoard()[0].size() * dimension))/2;

    // draw the background
    screen->draw_img("bg", 0, 0);

    // draw the board
    drawBoard(state.getBoard());

    // draw the paths
    for (size_t i = 0; i < state.getPlayersPaths().size(); ++i) {
        drawPath(state.getBoard(), state.getPlayersPaths()[i], i);
    }

    // draw tokens on the board
    drawBoardTokens(state.getBoard());

    // draw tokens at start
    drawInfoTokens(state.getPlayersTokens(), {boardStartX, boardStartY - 2 * dimension},
        {0, state.getBoard().size() * dimension + 3 * dimension},
        {dimension, 0}, [](const Token *t) { return t->getPathProgress() == 0; });

    size_t pathLength = state.getPlayersPaths()[0].size();

    // draw tokens at end
    drawInfoTokens(state.getPlayersTokens(),
        {state.getBoard()[0].size() * dimension + 2 * dimension, boardStartY - 4 * dimension},
        {dimension, 0},
        {0, dimension}, [pathLength](const Token *t) { return t->getPathProgress() - 1 == pathLength; });



    size_t turn = state.getPlayerTurn();

    // draw dice roll message
    std::pair<int, int> diceHeaderXY = (turn == 0) ?
        std::pair<int, int>{boardStartX, boardStartY - 4 * dimension}
        : std::pair<int, int>{boardStartX, boardStartY + state.getBoard().size() * dimension + 3 * dimension};

    Colour playerColour = getTokenColour(*state.getPlayersTokens()[turn][0]);
    if (state.requiresDiceRoll()) {
        drawHeader("Needs to roll dice!", diceHeaderXY, playerColour);
    } else {
        drawHeader("Dice Roll: " +
            std::to_string(state.getDiceRoll())
            + ", Flex Dice Roll: "
            + std::to_string(state.getFlexDiceRoll()),
            diceHeaderXY,
            playerColour);
    }

    screen->update();
}

void GUIView::visitTileTornado(const TileTornado& t) {
    screen->draw_img("tiletornado", xPos, yPos);
}

void GUIView::visitTileBlackHole(const TileBlackHole& t) {
    screen->draw_img("tileblackhole", xPos, yPos);
}

void GUIView::visitTileLucky(const TileLucky& t) {
    screen->draw_img("tilelucky", xPos, yPos);
}

void GUIView::visitTileRosette(const TileRosette& t) {
    screen->draw_img("tilerosette", xPos, yPos);
}

void GUIView::visitTileBasic(const TileBasic& t) {
    screen->draw_img("tilebasic", xPos, yPos);
}

void GUIView::visitTileNull(const TileNull& t) {
    screen->draw_img("tilenull", xPos, yPos);
}

void GUIView::drawToken(const std::string &key, const Token& t) {

    std::string playerIdStr{};
    std::string tokenCharStr{};

    playerIdStr.push_back(t.getPlayerId() + '0');
    tokenCharStr.push_back(getTokenChar(t));

    screen->draw_img(key + playerIdStr, xPos, yPos);

    // std::cout << "trying to print: " << key + playerIdStr << std::endl;

    screen->draw_string(tokenCharStr,
        xPos, yPos, getTokenColour(t));
}

void GUIView::visitTokenBasic(const TokenBasic& t) {
    drawToken("tokenbasic", t);
}

void GUIView::visitTokenAssassin(const TokenAssassin& t) {
    drawToken("tokenassassin", t);
}

void GUIView::visitTokenFlexible(const TokenFlexible& t) {
    drawToken("tokenflexible", t);
}

void GUIView::visitTokenSpeedster(const TokenSpeedster& t) {
    if (t.getManualAvailable()) {
        drawToken("tokenspeedsterfast", t);
    }
    else {
        drawToken("tokenspeedsterslow", t);
    }
}

void GUIView::visitTokenSupporter(const TokenSupporter& t) {
    drawToken("tokensupporter", t);
}

GUIView::GUIView(Screen* s): GameViewer{}, screen{s},
    boardStartX{} {
    // background
    s->add_img("bg", "assets/bg.png");

    // misc
    s->add_img("protected", "assets/protected.png");

    // tiles
    s->add_img("tilebasic", "assets/tilebasic.png");
    s->add_img("tilelucky", "assets/tilelucky.png");
    s->add_img("tileblackhole", "assets/tileblackhole.png");
    s->add_img("tilerosette", "assets/tilerosette.png");
    s->add_img("tiletornado", "assets/tiletornado.png");
    s->add_img("tilenull", "assets/tilenull.png");

    // tokens
    s->add_img("tokenassassin0", "assets/tokenassassin0.png");
    s->add_img("tokenbasic0", "assets/tokenbasichm0.png");
    s->add_img("tokenspeedsterslow0", "assets/tokenspeedsterslow0.png");
    s->add_img("tokenspeedsterfast0", "assets/tokenspeedsterfast0.png");
    s->add_img("tokensupporter0", "assets/tokensupporter0.png");
    s->add_img("tokenflexible0", "assets/tokenflexible0.png");

    s->add_img("tokenassassin1", "assets/tokenassassin1.png");
    s->add_img("tokenbasic1", "assets/tokenbasichm1.png");
    s->add_img("tokenspeedsterslow1", "assets/tokenspeedsterslow1.png");
    s->add_img("tokenspeedsterfast1", "assets/tokenspeedsterfast1.png");
    s->add_img("tokensupporter1", "assets/tokensupporter1.png");
    s->add_img("tokenflexible1", "assets/tokenflexible1.png");

    // paths
    s->add_img("da1", "assets/path/da1.png");
    s->add_img("dl1", "assets/path/dl1.png");
    s->add_img("dr1", "assets/path/dr1.png");
    s->add_img("horz1", "assets/path/horz1.png");
    s->add_img("la1", "assets/path/la1.png");
    s->add_img("ra1", "assets/path/ra1.png");
    s->add_img("ua1", "assets/path/ua1.png");
    s->add_img("ul1", "assets/path/ul1.png");
    s->add_img("ur1", "assets/path/ur1.png");
    s->add_img("vert1", "assets/path/vert1.png");

    s->add_img("da0", "assets/path/da0.png");
    s->add_img("dl0", "assets/path/dl0.png");
    s->add_img("dr0", "assets/path/dr0.png");
    s->add_img("horz0", "assets/path/horz0.png");
    s->add_img("la0", "assets/path/la0.png");
    s->add_img("ra0", "assets/path/ra0.png");
    s->add_img("ua0", "assets/path/ua0.png");
    s->add_img("ul0", "assets/path/ul0.png");
    s->add_img("ur0", "assets/path/ur0.png");
    s->add_img("vert0", "assets/path/vert0.png");
}

GUIView::~GUIView() {}
