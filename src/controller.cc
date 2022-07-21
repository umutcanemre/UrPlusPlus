#include <vector>
#include <iostream>
#include "gamestate.h"
#include "controller.h"
#include "gameviewer.h"
#include "player.h"

Controller::Controller(GameState* gameState, std::vector<Player*> players):
    gameState{gameState}, players{players} {}

Controller::~Controller() {}

// unneeded methinks
// const GameState& Controller::getState() const {
//     return *gameState;
// }

void Controller::registerObserver(GameViewer* o) {
    observers.emplace_back(o);
}

void Controller::makePlayerTurn() {
    notifyObservers();
    players[gameState->getPlayerTurn()]->playTurn(*gameState);
}

void Controller::notifyObservers() {
    for (const auto &o : observers) {
        o->notify(*gameState);
    }
}
