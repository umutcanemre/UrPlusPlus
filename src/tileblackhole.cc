#include "tileblackhole.h"
#include "entityvisitor.h"

void TileBlackHole::acceptVisitor(EntityVisitor& g) const {
    g.visitTileBlackHole(*this);
}

TileBlackHole::TileBlackHole(size_t row, size_t col): Tile{row, col} {}
