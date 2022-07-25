
#include "tokenbasic.h"
#include "entityvisitor.h"

void TokenBasic::acceptVisitor(EntityVisitor& g) const {
    g.visitTokenBasic(*this);
}

TokenBasic::TokenBasic(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {}

TokenBasic::~TokenBasic() {}
