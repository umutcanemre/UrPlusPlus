
#include "tilenull.h"
#include "gameviewer.h"

void TileNull::acceptVisitor(GameViewer& g) const {
    g.visitTileNull(*this);
}

TileNull::TileNull(): Tile{} {}
