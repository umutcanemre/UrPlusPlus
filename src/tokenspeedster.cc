
#include "tokenspeedster.h"
#include "entityvisitor.h"

bool TokenSpeedster::checkValid(int diceroll, int flexroll, int move) const {
    if (manualIsAvailable()) {
        return move == diceroll || move == diceroll + 1;
    } // else
    return move == diceroll;
}

std::vector<size_t> TokenSpeedster::validMoveDists(int diceRoll, int flexRoll) const {
    std::vector<size_t> v;
    v.emplace_back(static_cast<size_t>(diceRoll));
    if (manualIsAvailable()) {
        v.emplace_back(static_cast<size_t>(diceRoll + 1));
    }
    return v;
}

void TokenSpeedster::manualAbility() {
    --uses;
}
bool TokenSpeedster::manualIsAvailable() const {
    if (uses > 0) {
        return true;
    }
    return false;
}

void TokenSpeedster::acceptVisitor(EntityVisitor& g) const {
    g.visitTokenSpeedster(*this);
}

TokenSpeedster::TokenSpeedster(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress, size_t uses):
    Token{playerId, id, row, col, pathProgress}, uses{uses} {}

TokenSpeedster::TokenSpeedster(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {}

TokenSpeedster::~TokenSpeedster() {}
