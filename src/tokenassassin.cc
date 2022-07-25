#include "tokenassassin.h"
#include "entityvisitor.h"


bool TokenAssassin::captureAbility() {
    return true;
}

void TokenAssassin::acceptVisitor(EntityVisitor& g) const {
    g.visitTokenAssassin(*this);
}

TokenAssassin::TokenAssassin(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenAssassin::~TokenAssassin() {}
