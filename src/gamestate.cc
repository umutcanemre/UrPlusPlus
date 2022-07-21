#include <memory>
#include "gamestate.h"
#include "board.h"
#include <random>

bool GameState::rollDice() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::binomial_distribution<> d(4, 0.5);    
    flexdiceroll = static_cast<size_t>(d(gen));
    diceroll = static_cast<size_t>(d(gen));

    // Check if a valid move exists
    for (size_t i = 0; i < board->playersTokens.at(playerTurn).size(); ++i) {
        if (moveValid(i, diceroll) || moveValid(i, diceroll + 1) || moveValid(i, flexdiceroll)) {
            return true;
        }
    }
    return false;
}

const std::vector<std::vector<Tile*>>& GameState::getBoard() const {
    return board->getGameMap();
}


std::istream& operator>>(std::istream& in, GameState& g) {
    in >> *(g.board) >> g.playerTurn;

    return in;
}

GameState::GameState() : board{std::make_unique<Board>()} {}

bool GameState::moveValid(size_t tokenId, size_t distance) {
    Token* movingToken = board->playersTokens.at(playerTurn).at(tokenId).get();
    if (!movingToken->isValidMove(diceroll, flexdiceroll, distance)) {
        return false;
    }
    size_t newIndex = movingToken->getPathProgress() + distance - 1;
    if (board->paths.at(playerTurn).size() < newIndex) {
        // Out of bounds move
        return false;
    }
    Tile* newTile = board->paths.at(playerTurn).at(newIndex);
    if (!newTile->tileAvailable(movingToken)) {
        return false;
    }
    return true;
}

bool GameState::movePiece(size_t tokenId, size_t distance) {
    if (!moveValid(tokenId, distance)) {
        return false;
    }
    Token* movingToken = board->playersTokens.at(playerTurn).at(tokenId).get();
    size_t oldIndex = movingToken->getPathProgress() - 1; 
    Tile* oldTile = board->paths.at(playerTurn).at(oldIndex);
    if ((oldIndex + distance) == (board->paths.at(playerTurn).size())) {
        // We are moving into the end
        oldTile->setOccupant(nullptr);
        movingToken->updatePosition(std::make_pair(0, 0), movingToken->getPathProgress() + distance);
        return true;
    } 
    Tile* newTile = board->paths.at(playerTurn).at(oldIndex + distance);
    if (newTile->getOccupant() != nullptr) {
        // At this point, we are guaranteed this is an enemy token
        // that is capturable.
        // We need to send the enemy token to the start.
        Token* killedToken = newTile->getOccupant();
        killedToken->updatePosition(std::make_pair(0, 0), 0);
    }

    // update tile occupants
    oldTile->setOccupant(nullptr);
    newTile->setOccupant(movingToken);

    // update position stored in token
    movingToken->updatePosition(newTile->getPosition(), movingToken->getPathProgress() + distance);
    return true;
}
