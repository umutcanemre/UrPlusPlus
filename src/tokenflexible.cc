#include "tokenflexible.h"
#include "gameviewer.h"

void TokenFlexible::acceptVisitor(GameViewer& g) const {
    g.visitTokenFlexible(*this);
};

TokenFlexible::TokenFlexible(size_t playerId, size_t id, size_t row, size_t col):
    Token{playerId, id, row, col} {};

TokenFlexible::~TokenFlexible() {}
