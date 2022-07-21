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

TileRosette::TileRosette(): Tile{} {}
