
#include "tokenspeedster.h"
#include "gameviewer.h"

bool TokenSpeedster::checkValid(int diceroll, int flexroll, int move) const {
    if (uses > 0) {
        return move == diceroll || move == diceroll + 1;
    } // else
    return move == diceroll;
}

void TokenSpeedster::manualAbility() { 
    --uses; 
}
bool TokenSpeedster::isAvailable() const {
    if (uses > 0) {
        return true;
    }
    return false;
}

void TokenSpeedster::acceptVisitor(GameViewer& g) const {
    g.visitTokenSpeedster(*this);
}

TokenSpeedster::TokenSpeedster(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress}, uses{1} {}

TokenSpeedster::~TokenSpeedster() {}
