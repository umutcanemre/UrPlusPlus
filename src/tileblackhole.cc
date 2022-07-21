#include "tileblackhole.h"
#include "gameviewer.h"

void TileBlackHole::acceptVisitor(GameViewer& g) const {
    g.visitTileBlackHole(*this);
}

TileBlackHole::TileBlackHole(size_t row, size_t col): Tile{row, col} {}
