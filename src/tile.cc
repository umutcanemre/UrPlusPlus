#include "tile.h"
#include <vector>




// bool Tile::rollsDice() const {
//     return false;
// }
// bool Tile::extraTurn() const {
//     return false;
// }
// bool Tile::makesInvulnerable() const {
//     return false;
// }
// int Tile::avgPlayerMoveDistance(size_t location, const std::vector<Tile*> &path) const {
//     return 0;
// }

bool Tile::onMoveSuccess(Token* t, const std::vector<Tile*> &path) {
    return doOnMoveSuccess(t, path);
}

bool Tile::doOnMoveSuccess(Token* t, const std::vector<Tile*> &path) {
    return false;
}

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

// bool Tile::grantsExtraTurn() const {
//     return extraTurn();
// }
// // returns true if this tile grants an extra dice roll (but not turn) 
// bool Tile::grantsDiceRoll() const {
//     return rollsDice(); 
// }
// // returns true if this tile grants invulnerability 
// bool Tile::grantsInvulnerability() const {
//     return makesInvulnerable();
// }

// int Tile::sendsPlayerForward(size_t location, const std::vector<Tile*> &path) const {
//     return avgPlayerMoveDistance(location, path);
// }


Tile::~Tile() {}
