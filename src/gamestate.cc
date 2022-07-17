#include <memory>
#include "gamestate.h"
#include "board.h"

void GameState::rollDice() {
    flexdiceroll = 4;
    diceroll = 4;
}

const std::vector<std::vector<Tile*>>& GameState::getBoard() const {
    return board->getGameMap();
}


std::istream& operator>>(std::istream& in, GameState& g) {
    in >> *(g.board);

    return in;
}

GameState::GameState(): board{std::make_unique<Board>()} {}
