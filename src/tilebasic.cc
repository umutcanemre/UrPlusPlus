#include "tilebasic.h"
#include "gameviewer.h"

void TileBasic::acceptVisitor(GameViewer& g) const {
    g.visitTileBasic(*this);
}

TileBasic::TileBasic(size_t row, size_t col): Tile{row, col} {}
