#include "gameviewer.h"

void GameViewer::notify(const GameState& g) {
    doNotify(g);
}

GameViewer::~GameViewer() {}
