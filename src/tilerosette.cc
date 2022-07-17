#include "tilerosette.h"
#include "gameviewer.h"

void TileRosette::acceptVisitor(GameViewer& g) const {
    g.visitTileRosette(*this);
}

TileRosette::TileRosette(): Tile{} {}
