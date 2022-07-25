#include "tokenflexible.h"
#include "entityvisitor.h"

void TokenFlexible::acceptVisitor(EntityVisitor& g) const {
    g.visitTokenFlexible(*this);
};

bool TokenFlexible::checkValid(int diceroll, int flexroll, int move) const {
    return move == diceroll || move == flexroll;
}
std::vector<size_t> TokenFlexible::validMoveDists(int diceRoll, int flexRoll) const {
    std::vector<size_t> v;
    v.emplace_back(static_cast<size_t>(diceRoll));
    v.emplace_back(static_cast<size_t>(flexRoll));
    return v;
}

TokenFlexible::TokenFlexible(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenFlexible::~TokenFlexible() {}
