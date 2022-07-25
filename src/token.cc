#include "token.h"

void Token::passiveAbility(std::vector<Tile*>& path) { }
bool Token::checkValid(int diceroll, int flexroll, int move) const {
    return move == diceroll;
}
bool Token::captureAbility() {
    return false;
}
void Token::manualAbility() {
}
bool Token::manualIsAvailable() const {
    // by default it returns true (non-limited ability)
    // possibly overridden if an ability of some child class is limited
    return false;
}

bool Token::activateCapture() {
    return captureAbility();
}
bool Token::isValidMove(int diceroll, int flexroll, int move) const {
    return checkValid(diceroll, flexroll, move);
}
std::vector<size_t> Token::validMoveDists(int diceRoll, int flexRoll) const {
    std::vector<size_t> v;
    v.emplace_back(static_cast<size_t>(diceRoll));
    return v;
}

void Token::activatePassive(std::vector<Tile*>& path) {
    passiveAbility(path);
}
void Token::activateManual() {
    manualAbility();
}
void Token::updatePosition(std::pair<size_t, size_t> newPos, size_t newPathProgress) {
    // precondition: newPos and distanceTraveled are valid
    pathProgress = newPathProgress;
    position = newPos;
}


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
    return isProtectedByToken;
}
void Token::setIsProtected(bool isProtected) {
    isProtectedByToken = isProtected;
}
bool Token::getManualAvailable() const {
    return manualIsAvailable();
}

std::vector<size_t> Token::getValidMoveDistances(int diceRoll, int flexRoll) const {
    return validMoveDists(diceRoll, flexRoll);
}

Token::Token(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress):
    playerId{playerId}, id{id}, position{row, col}, pathProgress{pathProgress} {}


Token::~Token() {}

