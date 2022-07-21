
#include "tokenspeedster.h"
#include "gameviewer.h"

void TokenSpeedster::acceptVisitor(GameViewer& g) const {
    g.visitTokenSpeedster(*this);
};

TokenSpeedster::TokenSpeedster(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenSpeedster::~TokenSpeedster() {}
