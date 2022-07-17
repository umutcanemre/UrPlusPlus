#include <iostream>
#include "gamestate.h"
#include "board.h"
#include "gameviewer.h"
#include "cliview.h"

using namespace std;

int main() {
    CLIView cliview{&cout};
    GameViewer& viewer = cliview;

    GameState g;
    cin >> g;

    // viewer.notify(g);
}
