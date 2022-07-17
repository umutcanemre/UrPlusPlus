#include "tileblackhole.h"
#include "gameviewer.h"

void TileBlackHole::acceptVisitor(GameViewer& g) const {
    g.visitTileBlackHole(*this);
}

TileBlackHole::TileBlackHole(): Tile{} {}
