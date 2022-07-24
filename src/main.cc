#include <iostream>
#include "gamestate.h"
#include "board.h"
#include "gameviewer.h"
#include "cliview.h"
#include "controller.h"
#include "player.h"
#include "human.h"
#include "level1ai.h"
#include "level2ai.h" 

using namespace std;

int main() {
    CLIView cliview{&cout};

    // read in the game state
    GameState state;
    cin >> state;

    Human gamer1{&cin, &cout};
    // Human gamer2{&cin, &cout};
    Level1AI gamer2{};
    // Level2AI gamer2{};

    Controller game{&state, {&gamer1, &gamer2}};

    game.registerObserver(&cliview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }

}
