#include <vector>
#include "cliview.h"
#include "gamestate.h"
#include "token.h"
#include "tile.h"
// #include "tile.h"
// #include "tilebasic.h"
// #include "tileblackhole.h"
// #include "tileend.h"
// #include "tilelucky.h"
// #include "tilerosette.h"
// #include "tilestart.h"
// #include "tiletornado.h"

#include "tokenbasic.h"
#include "tokenassassin.h"
#include "tokenflexible.h"
#include "tokenspeedster.h"
#include "tokensupporter.h"

CLIView::CLIView(std::ostream* out): GameViewer{}, out{out} {}
CLIView::~CLIView() {}

char CLIView::getTokenChar(const Token& t) {
    // unwise ASCII hacking
    return ('a' - (('0' *  - 1) * t.getPlayerId())) + t.getTokenId();
}

void CLIView::doNotify(const GameState& g) {
    for (const auto &row : g.getBoard()) {
        for (const auto &t : row) {
            if (!t->getOccupant()) {
                t->acceptVisitor(*this);
            }
            else {
                t->getOccupant()->acceptVisitor(*this);
            }
        }
    }
}

void CLIView::visitTileStart(const TileStart& t) {
    (*out) << 'S';
}

void CLIView::visitTileEnd(const TileEnd& t) {
    (*out) << 'E';
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
