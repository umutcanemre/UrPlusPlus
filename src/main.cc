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
    // Human gamer1{&cin, &cout};
    // Human gamer2{&cin, &cout};
    // Level1AI gamer2{};
    // Level2AI gamer2{};
    // Level3AI gamer2{};
    // Level4AI gamer2{};

    unique_ptr<Player> gamer1;
    unique_ptr<Player> gamer2;
    gamer1 = make_unique<Player>(Human{&cin, &cout});
    gamer2 = make_unique<Player>(Level3AI{});
    // gamer2 = make_unique<Player>(Level2AI{});
    // gamer2 = make_unique<Player>(Level1AI{});



    // if (argc % 2 == 0) { // enable GUI

    // } 
    // if (argc == 2 || argc == 1) { // no AI players
    //     gamer1 = make_unique<Player>(Human{&cin, &cout});
    //     gamer2 = make_unique<Player>(Human{&cin, &cout});
    // } else if (argc == 4 || argc == 3) { // 1 AI player 
    //     int maxIndex = argc - 1;
    //     gamer1 = make_unique<Player>(Human{&cin, &cout});
    //     if (stoi(argv[maxIndex]) == 1) gamer2 = make_unique<Player>(Level1AI{});
    //     else if (stoi(argv[maxIndex]) == 2) gamer2 = make_unique<Player>(Level2AI{});
    //     else if (stoi(argv[maxIndex]) == 3) gamer2 = make_unique<Player>(Level3AI{});
    // } else if (argc == 6 || argc == 5) { // 2 AI players
    //     int maxIndex = argc - 1; // ok i have no idea how best to do argv manipulation any tips?
    //     gamer1 = make_unique<Player>(Human{&cin, &cout});
    //     if (stoi(argv[maxIndex]) == 1) { 
    //         gamer2 = make_unique<Player>(Level1AI{});
    //     }
    //     else if (stoi(argv[maxIndex]) == 2) { 
    //         gamer2 = make_unique<Player>(Level2AI{});
    //     }
    //     else if (stoi(argv[maxIndex]) == 3) { 
    //         gamer2 = make_unique<Player>(Level3AI{});
    //     }
    // }

    CLIView cliview{&cout};

    // read in the game state
    GameState state;
    cin >> state;


    Controller game{&state, {gamer1.get(), gamer2.get()}};

    game.registerObserver(&cliview);

    while (!state.gameIsOver()) {
        game.makePlayerTurn();
    }
    
}
