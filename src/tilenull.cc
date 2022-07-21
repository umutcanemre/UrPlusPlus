
#include "tilenull.h"
#include "gameviewer.h"

void TileNull::acceptVisitor(GameViewer& g) const {
    g.visitTileNull(*this);
}

TileNull::TileNull(size_t row, size_t col): Tile{row, col} {}
