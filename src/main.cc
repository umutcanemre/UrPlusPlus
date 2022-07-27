#include <iostream>
#include <sstream>
#include <algorithm>
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
#include "level2ai.h"
#include "level3ai.h"
#include "level4ai.h"
#include "sdl_wrap.h"

using namespace std;

// given a command line option to find of the form [option <value>], return
// pointer to value or nullptr
char** findCmdOption(char **begin, char **end, const std::string &option) {
    char **it = std::find(begin, end, option);

    // if we didn't find the option, or if we found
    // option but nothing follows it
    if (it == end || it + 1 == end) {
        return nullptr;
    }

    // to make it point to value
    ++it;

    return it;
}

// given a command line option to find of the form [option]
// return true if it is found false otherwise
bool cmdFlagExists(char **begin, char **end, const std::string &option) {
    char **it = std::find(begin, end, option);

    return it != end;
}

// Just must declare a global variable (or at the start of main)
// Don't do anything with this object. It just handles the start and end of SDL.

int main(int argc, char* argv[]) {
    const string usageMsg = "Usage: [-AI <AI level>] [-AI <AI level>] [--nogui]";

    size_t playerCount = 2;

    vector<unique_ptr<Player>> players{};
    vector<Player*> nonOwningPlayerPtrs{};

    // initialize any AI characters

    // super sus array pointer arithmetic
    char **AILevelStr = findCmdOption(argv, argv + argc, "-AI");
    size_t AILevel;

    while (playerCount > 0 && AILevelStr) {
        stringstream ss{};
        ss << *AILevelStr;
        // pre: ai level entered is a number
        ss >> AILevel;

        switch (AILevel) {
            case 1:
                players.emplace_back(make_unique<Level1AI>());
                break;
            case 2:
                players.emplace_back(make_unique<Level2AI>());
                break;
            case 3:
                players.emplace_back(make_unique<Level3AI>());
                break;
            case 4:
                players.emplace_back(make_unique<Level4AI>());
                break;
            default:
                throw invalid_argument{"AI level must be between 1 and 4 inclusive"};
        }

        AILevelStr = findCmdOption(AILevelStr, argv + argc, "-AI");
        --playerCount;
    }

    while (playerCount > 0) {
       players.emplace_back(make_unique<Human>(&cin, &cout));
       --playerCount;
    }

    // reverse the players vector so humans come first for
    // convenience
    reverse(players.begin(), players.end());

    for (const auto &p : players) {
        nonOwningPlayerPtrs.emplace_back(p.get());
    }

    CLIView cliview{&cout};

    // read in the game state
    GameState state;
    cin >> state;

    Controller game{&state, nonOwningPlayerPtrs};

    game.registerObserver(&cliview);

    try {
        if (!cmdFlagExists(argv, argv + argc, "--nogui")) {
            // the main game loop MUST live inside this
            // if statement since all of these things will destruct
            // after going out of scope. The benefit of this is that all
            // the SDL nonsense is contained here and it will never be constructed

            // the alternative is to make unique_ptrs to everything that manage ownership
            // outside of the scope of this if statement but im not sure if SDL_Runner
            // will like being put in a pointer and I don't have the time to find out
            SDL_Runner r;
            Screen s{640,640};
            GUIView guiview{&s};
            game.registerObserver(&guiview);
            while (!state.gameIsOver()) {
                game.makePlayerTurn();
            }
            cout << "Player " << state.getWinner() << " won, terminating program." << endl;
        }

        else {
            while (!state.gameIsOver()) {
                game.makePlayerTurn();
            }
            cout << "Player " << state.getWinner() << " won, terminating program." << endl;
        }
        return 0;
    }

    catch (const std::ios_base::failure& fail) {
        cout << "Input failure: terminating program, press enter." << endl;
        return 0;
    }

}
