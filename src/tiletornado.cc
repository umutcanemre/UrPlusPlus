#include "tiletornado.h"
#include "entityvisitor.h"
#include <random>


void TileTornado::acceptVisitor(EntityVisitor& g) const {
    g.visitTileTornado(*this);
}

bool TileTornado::doOnMoveSuccess(Token* t, const std::vector<Tile*> &path) {
    // I'm converting to int here for convenience to avoid underflow.
    int index = t->getPathProgress() - 1;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::binomial_distribution<> d(4, 0.5);
    size_t randomResult = static_cast<size_t>(d(gen));

    if (index != 0) {
        // Remove token from old tile
        path.at(index)->setOccupant(nullptr);
    }

    if (randomResult % 2 == 0) {
        // Even roll onto tile- move forward
        index++;
        while (index < path.size()) {
            if (path.at(index)->getOccupant() == nullptr) {
                path.at(index)->setOccupant(t);
                t->updatePosition(path.at(index)->getPosition(), index + 1);
                break;
            }
            index++;
        }
        if (index == path.size()) {
            // We have reached the end- just update position
            t->updatePosition(std::make_pair(0, 0), index + 1);
        }
    } else {
        // Odd roll onto tile- move backwards
        index--;
        while (index >= 0) {
            if (path.at(index)->getOccupant() == nullptr) {
                path.at(index)->setOccupant(t);
                t->updatePosition(path.at(index)->getPosition(), index + 1);
                break;
            }
            index--;
        }
        if (index == -1) {
            // We have reached the beginning- just update position
            t->updatePosition(std::make_pair(0, 0), index + 1);
        }
    }

    // Should not repeat turn
    return false;
}


TileTornado::TileTornado(size_t row, size_t col): Tile{row, col} {}
