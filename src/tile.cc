#include "tile.h"
#include <vector>

// bool Tile::moveAvailable(Token* t) {
//     return Tile::checkMoveAvailable(t);
// }

// bool Tile::onMoveSuccess(Token* t, const std::vector<Token*> &path) {
//     doOnMoveSuccess(t, path);
// }

const Token* Tile::getOccupant() const {
    return occupant;
}

Tile::~Tile() {}
