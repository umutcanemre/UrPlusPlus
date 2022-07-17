#include "tokenassassin.h"
#include "gameviewer.h"

void TokenAssassin::acceptVisitor(GameViewer& g) const {
    g.visitTokenAssassin(*this);
};

TokenAssassin::TokenAssassin(size_t playerId, size_t id, size_t row, size_t col):
    Token{playerId, id, row, col} {};

TokenAssassin::~TokenAssassin() {}
