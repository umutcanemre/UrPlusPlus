
#include "tileend.h"
#include "gameviewer.h"

void TileEnd::acceptVisitor(GameViewer& g) const {
    g.visitTileEnd(*this);
}

TileEnd::TileEnd(): Tile{} {}
