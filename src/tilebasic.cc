#include "tilebasic.h"
#include "gameviewer.h"

void TileBasic::acceptVisitor(GameViewer& g) const {
    g.visitTileBasic(*this);
}

TileBasic::TileBasic(): Tile{} {}
