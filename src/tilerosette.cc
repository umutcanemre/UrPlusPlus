#include "tilerosette.h"
#include "gameviewer.h"

void TileRosette::acceptVisitor(GameViewer& g) const {
    g.visitTileRosette(*this);
}

bool TileRosette::specialCheck() {
    if (getOccupant() != nullptr) {
        return false;
    }
    return true;
}

bool TileRosette::doOnMoveSuccess(Token*, const std::vector<Tile*> &path) {
    // indicate we should repeat turn
    return true;
}

TileRosette::TileRosette(size_t row, size_t col): Tile{row, col} {}
