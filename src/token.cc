#include "token.h"

bool Token::activateCapture() {
    return false;
}
bool Token::isValidMove(int diceroll, int flexroll, int move) const {
    return move == diceroll;
}

void Token::activatePassive(const std::vector<Tile*>& path) {}


size_t Token::getPlayerId() const {
    return playerId;
}
size_t Token::getTokenId() const {
    return id;
}
size_t Token::getPathProgress() const {
    return pathProgress;
}
std::pair<size_t, size_t> Token::getPosition() {
    return position;
}

bool Token::getIsProtected() const {
    return isProtectedByTile;
}
void Token::setIsProtected(bool isProtected) {
    isProtectedByTile = isProtected;
}


Token::Token(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress):
    playerId{playerId}, id{id}, position{row, col}, pathProgress{pathProgress} {}

void Token::updatePosition(std::pair<size_t, size_t> newPos) {
    position = newPos;
}

Token::~Token() {}

