#include "tokensupporter.h"
#include "gameviewer.h"

void TokenSupporter::acceptVisitor(GameViewer& g) const {
    g.visitTokenSupporter(*this);
};

TokenSupporter::TokenSupporter(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenSupporter::~TokenSupporter() {}
