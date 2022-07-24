
#include "tilenull.h"
#include "entityvisitor.h"

void TileNull::acceptVisitor(EntityVisitor& g) const {
    g.visitTileNull(*this);
}

TileNull::TileNull(size_t row, size_t col): Tile{row, col} {}
