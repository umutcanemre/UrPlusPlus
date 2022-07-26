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
#include "level2ai.h" 
#include "level3ai.h" 
#include "level4ai.h" 

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


    Controller game{&state, {gamer1.get(), gamer2.get()}};

    game.registerObserver(&cliview);
    game.registerObserver(&guiview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }

}
