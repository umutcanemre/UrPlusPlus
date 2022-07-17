#include "tilelucky.h"
#include "gameviewer.h"

void TileLucky::acceptVisitor(GameViewer& g) const {
    g.visitTileLucky(*this);
}

TileLucky::TileLucky(): Tile{} {}
