#include "tilelucky.h"
#include "entityvisitor.h"

void TileLucky::acceptVisitor(EntityVisitor& g) const {
    g.visitTileLucky(*this);
}

TileLucky::TileLucky(size_t row, size_t col): Tile{row, col} {}
