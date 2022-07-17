#include "tilestart.h"
#include "gameviewer.h"

void TileStart::acceptVisitor(GameViewer& g) const {
    g.visitTileStart(*this);
}

TileStart::TileStart(): Tile{} {}
