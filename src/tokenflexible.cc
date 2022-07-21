#include "tokenflexible.h"
#include "gameviewer.h"

void TokenFlexible::acceptVisitor(GameViewer& g) const {
    g.visitTokenFlexible(*this);
};

bool TokenFlexible::checkValid(int diceroll, int flexroll, int move) const {
    return move == diceroll || move == flexroll;
}

TokenFlexible::TokenFlexible(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenFlexible::~TokenFlexible() {}
