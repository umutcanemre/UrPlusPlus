#include "tiletornado.h"
#include "gameviewer.h"

void TileTornado::acceptVisitor(GameViewer& g) const {
    g.visitTileTornado(*this);
}

TileTornado::TileTornado(size_t row, size_t col): Tile{row, col} {}
