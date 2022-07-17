#include <memory>
#include "gamestate.h"
#include "board.h"
#include <random>

void GameState::rollDice() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::binomial_distribution<> d(4, 0.5);    
    flexdiceroll = d(gen);
    diceroll = d(gen);
}

const std::vector<std::vector<Tile*>>& GameState::getBoard() const {
    return board->getGameMap();
}


std::istream& operator>>(std::istream& in, GameState& g) {
    in >> *(g.board);

    return in;
}

GameState::GameState(): board{std::make_unique<Board>()} {}
