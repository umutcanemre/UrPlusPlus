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
    const string usageMsg = "Usage: [-AI <AI level>] [-AI <AI level>] [--guienable]";
    Screen s{640,640};
    CLIView cliview{&cout};

    GUIView guiview{&s};

    // make the players
    Human gamer1{&cin, &cout};
    // Human gamer2{&cin, &cout};
    // Level1AI gamer2{};
    // Level2AI gamer2{};
    // Level3AI gamer2{};
    Level4AI gamer2{};


    // read in the game state
    GameState state;
    cin >> state;


    Controller game{&state, {&gamer1, &gamer2}};

    game.registerObserver(&cliview);
    game.registerObserver(&guiview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }

}
