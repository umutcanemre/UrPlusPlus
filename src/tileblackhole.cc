#include "tileblackhole.h"
#include "entityvisitor.h"

void TileBlackHole::acceptVisitor(EntityVisitor& g) const {
    g.visitTileBlackHole(*this);
}

bool TileBlackHole::doOnMoveSuccess(Token* t, const std::vector<Tile*> &path) {
    path.at(t->getPathProgress() - 1)->setOccupant(nullptr);
    t->updatePosition(std::make_pair(0, 0), 0);
    return false;
}

TileBlackHole::TileBlackHole(size_t row, size_t col): Tile{row, col} {}
