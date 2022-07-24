#include "tiletornado.h"
#include "entityvisitor.h"


void TileTornado::acceptVisitor(EntityVisitor& g) const {
    g.visitTileTornado(*this);
}

TileTornado::TileTornado(size_t row, size_t col): Tile{row, col} {}
