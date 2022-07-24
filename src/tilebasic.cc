#include "tilebasic.h"
#include "entityvisitor.h"

void TileBasic::acceptVisitor(EntityVisitor& g) const {
    g.visitTileBasic(*this);
}

TileBasic::TileBasic(size_t row, size_t col): Tile{row, col} {}
