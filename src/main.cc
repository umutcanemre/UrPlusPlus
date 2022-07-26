#include <iostream>
#include "gamestate.h"
#include "board.h"
#include "gameviewer.h"
#include "cliview.h"
#include "guiview.h"
#include "controller.h"
#include "player.h"
#include "human.h"
#include "level1ai.h"
#include "level2ai.h"
#include "sdl_wrap.h"
using namespace std;

// Just must declare a global variable (or at the start of main)
// Don't do anything with this object. It just handles the start and end of SDL.
SDL_Runner r;

int main() {
    Screen s{640,640};
    CLIView cliview{&cout};

    GUIView guiview{&s};


    // read in the game state
    GameState state;
    cin >> state;

    Human gamer1{&cin, &cout};
    Human gamer2{&cin, &cout};
    // Level1AI gamer2{};
    // Level2AI gamer2{};
    // Level3AI gamer2{};

    Controller game{&state, {&gamer1, &gamer2}};

    game.registerObserver(&cliview);
    game.registerObserver(&guiview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }

}
