#include "tilelucky.h"
#include "entityvisitor.h"
#include <random>

void TileLucky::acceptVisitor(EntityVisitor& g) const {
    g.visitTileLucky(*this);
}
bool TileLucky::doOnMoveSuccess(Token* t, const std::vector<Tile*> &path) {
    int index = t->getPathProgress() - 1;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::binomial_distribution<> d(4, 0.5);
    size_t randomResult = static_cast<size_t>(d(gen));

    if (randomResult == 0) {
        // Send to front
        path.at(index)->setOccupant(nullptr);
        t->updatePosition(std::make_pair(0, 0), 0);
    } else if (randomResult == 4) {
        // Send to end
        path.at(index)->setOccupant(nullptr);
        t->updatePosition(std::make_pair(0, 0), path.size() + 1);
    }
    return false;
}

TileLucky::TileLucky(size_t row, size_t col): Tile{row, col} {}
