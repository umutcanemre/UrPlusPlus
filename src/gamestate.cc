#include <memory>
#include <random>
#include <vector>
#include <algorithm>
#include "gamestate.h"
#include "board.h"

char* InvalidGameStateOperation::what() {
    return "Illegal Operation";
}

char* InvalidWinnerException::what() {
    return "No winner exists yet";
}

void GameState::recalculatePassive() {
    // Activate passive abilities for all players
    // This is necessasry in the case we start on an opposite player's turn
    for (size_t i = 0; i < board->playerCount; ++i) {
        for (size_t j = 0; j < board->playersTokens.at(playerTurn).size(); ++j) {
            board->playersTokens.at(i).at(j)->activatePassive(board->paths.at(playerTurn));
        }
    }
}

void GameState::rollDice() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::binomial_distribution<> d(4, 0.5);
    flexdiceroll = static_cast<size_t>(d(gen));
    diceroll = static_cast<size_t>(d(gen));

    // Recalculate passive before checking move validity
    recalculatePassive();
    
    // Check if a valid move exists
    for (size_t i = 0; i < board->playersTokens.at(playerTurn).size(); ++i) {
        if (moveValid(i, diceroll) || moveValid(i, diceroll + 1)
            || moveValid(i, flexdiceroll)) {
            diceAreFresh = true;
            noValidMoves = false;
            return;
        }
    }
    // if there's no valid moves
    diceAreFresh = true;
    noValidMoves = true;
}

void GameState::moveToNextPlayerTurn() {
    diceAreFresh = false;
    playerTurn = (playerTurn + 1) % Board::playerCount;
}

void GameState::repeatPlayerTurn() {
    diceAreFresh = false;
}

bool GameState::gameIsOver() const {
    if (winnerPlayer != -1) {
        return true;
    }
    return false;
}

size_t GameState::getWinner() const {
    if (winnerPlayer == -1) {
        throw InvalidWinnerException{};
    }
    return winnerPlayer;
}

size_t GameState::getPlayerTurn() const {
    return playerTurn;
}

size_t GameState::getDiceRoll() const {
    return diceroll;
}

size_t GameState::getFlexDiceRoll() const {
    return flexdiceroll;
}

bool GameState::requiresDiceRoll() const {
    return !diceAreFresh;
}

bool GameState::hasNoValidMoves() const {
    return noValidMoves;
}

const std::vector<std::vector<Tile*>>& GameState::getBoard() const {
    return board->getGameMap();
}

const std::vector<std::vector<Token*>>& GameState::getPlayersTokens() const {
    return board->getPlayersTokens();
}

const std::vector<std::vector<Tile*>>& GameState::getPlayersPaths() const {
    return board->getPlayersPaths();
}

std::istream& operator>>(std::istream& in, GameState& g) {
    in >> *(g.board) >> g.playerTurn;

    if (g.playerTurn < 0 || g.playerTurn >= Board::playerCount) {
        // maybe a different exception
        throw BoardParseException{};
    }

    return in;
}

GameState::GameState() : board{std::make_unique<Board>()} {}

void GameState::skipTurn() {
    if (!hasNoValidMoves()) {
        throw InvalidGameStateOperation{};
    }
    moveToNextPlayerTurn();
}

bool GameState::moveValid(size_t tokenId, size_t distance) const {
    if (distance == 0) { // if player tries to move distance 0, this is invalid
        return false;
    }
    Token* movingToken = board->playersTokens.at(playerTurn).at(tokenId).get();
    // if player tries to make a move that is not a valid distance for this token return false
    if (!movingToken->isValidMove(diceroll, flexdiceroll, distance)) {
        return false;
    }
    size_t newIndex = movingToken->getPathProgress() + distance - 1;
    // if player tries to move out of bounds return false
    if (board->paths.at(playerTurn).size() < newIndex) {
        // Out of bounds move
        return false;
    }
    // Special case where index is equal to size
    // Need to return before creating newtile
    if (board->paths.at(playerTurn).size() == newIndex) {
        if (movingToken->getPathProgress() - 1 == board->paths.at(playerTurn).size()) {
            return false;
        }
        return true;
    }
    Tile* newTile = board->paths.at(playerTurn).at(newIndex);
    // if the next tile is not available return false
    if (!newTile->tileAvailable(movingToken)) {
        return false;
    }
    return true;
}

bool GameState::movePiece(size_t tokenId, size_t distance) {
    auto &playerTokens = board->playersTokens.at(playerTurn);

    // before any checks, make sure the tokenId is a valid token
    if (std::find_if(playerTokens.begin(), playerTokens.end(), [tokenId](std::unique_ptr<Token> &t) {
        return t->getTokenId() == tokenId;
    }) == playerTokens.end()) {
        return false;
    }

    // Recalculate passive before checking move validity
    recalculatePassive();

    if (!moveValid(tokenId, distance)) {
        return false;
    }
    Token* movingToken = playerTokens.at(tokenId).get();

    // Tile/token ability updates/checks
    if (diceroll + 1 == distance) {
        // Activate speedster ability
        // Note that in the case that we are using a flexible
        // diceroll that happens to be equal to diceroll + 1
        // It will just call activate manual on flexible which
        // is harmless
        movingToken->activateManual();
    }

    // Calculating old path progress causes issues when pathprogress = 0
    size_t oldPathProgress = movingToken->getPathProgress();
    Tile* oldTile = (oldPathProgress == 0) ? nullptr : board->paths.at(playerTurn).at(oldPathProgress - 1);
    bool captureTurnRepeat  = false;
    bool tileTurnRepeat = false;

    // Clear old tile
    if (oldTile != nullptr) {
        oldTile->setOccupant(nullptr);
    }

    if ((oldPathProgress - 1 + distance) == (board->paths.at(playerTurn).size())) {
        // Reached end- no need to check for capture,
        // Don't update new tile
        movingToken->updatePosition(std::make_pair(0, 0), movingToken->getPathProgress() + distance);
    } else {
        Tile* newTile = board->paths.at(playerTurn).at(oldPathProgress - 1 + distance);
        if (newTile->getOccupant() != nullptr) {
            // At this point, we are guaranteed this is an enemy token
            // that is capturable.
            // We need to send the enemy token to the start.
            Token* killedToken = newTile->getOccupant();
            killedToken->updatePosition(std::make_pair(0, 0), 0);
            // Assassin changes to true; so we repeat turn
            captureTurnRepeat = movingToken->activateCapture();
        }

        // Update new tile
        newTile->setOccupant(movingToken);

        // Update position stored in token
        movingToken->updatePosition(newTile->getPosition(), movingToken->getPathProgress() + distance);

        // Activates tile ability; returns true if additional turn gained
        tileTurnRepeat = newTile->onMoveSuccess(movingToken, board->paths.at(playerTurn));
    }

    // Check for winner
    bool playerWon = true;
    for (auto token : board->getPlayersTokens().at(playerTurn)) {
        if (token->getPathProgress() - 1 != board->paths.at(playerTurn).size()) {
            playerWon = false;
            break;
        }
    }
    if (playerWon) {
        winnerPlayer = playerTurn;
        return true;
    }

    if (captureTurnRepeat  || tileTurnRepeat) {
        repeatPlayerTurn();
    } else {
        moveToNextPlayerTurn();
    }
    return true;
}
