#include "tile.h"
#include <vector>

// bool Tile::moveAvailable(Token* t) {
//     return Tile::checkMoveAvailable(t);
// }

// bool Tile::onMoveSuccess(Token* t, const std::vector<Token*> &path) {
//     doOnMoveSuccess(t, path);
// }
Tile::Tile(size_t row, size_t col) : position{std::make_pair(row, col)} {}

bool Tile::specialCheck() {
    return true;
}

bool Tile::tileAvailable(Token* t) {
    if (!specialCheck()) {
        // Tile ability states move is invalid
        return false;
    }
    if ((getOccupant() != nullptr) && (t->getPlayerId() == getOccupant()->getPlayerId())) {
        // Tile is occupied by token on same team
        return false;
    }
    return true;
}

Token* Tile::getOccupant() const {
    return occupant;
}

void Tile::setOccupant(Token* t) {
    occupant = t;
}

std::pair<size_t, size_t> Tile::getPosition() const {
    return position;
}



Tile::~Tile() {}
