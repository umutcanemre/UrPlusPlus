
#include "tokenbasic.h"
#include "gameviewer.h"

void TokenBasic::acceptVisitor(GameViewer& g) const {
    g.visitTokenBasic(*this);
};

TokenBasic::TokenBasic(size_t playerId, size_t id, size_t row, size_t col):
    Token{playerId, id, row, col} {};

TokenBasic::~TokenBasic() {}
