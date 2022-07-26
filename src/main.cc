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
#include "level3ai.h" 
#include "level4ai.h" 

using namespace std;

int main(int argc, char* argv[]) { 
    const string usageMsg = "Usage: [-AI <AI level>] [-AI <AI level>] [--guienable]";
    Human gamer1{&cin, &cout};
    // Human gamer2{&cin, &cout};
    // Level1AI gamer2{};
    // Level2AI gamer2{};
    // Level3AI gamer2{};
    Level4AI gamer2{};

    CLIView cliview{&cout};

    // read in the game state
    GameState state;
    cin >> state;


    Controller game{&state, {&gamer1, &gamer2}};

    game.registerObserver(&cliview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }
    
}
