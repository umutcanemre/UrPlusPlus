#include "gameobserver.h"

void GameObserver::notify(const GameState& g) {
    doNotify(g);
}

GameObserver::~GameObserver() {}
