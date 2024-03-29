#ifndef URPLUSPLUS_GAMESTATE_H_
#define URPLUSPLUS_GAMESTATE_H_

#include <iostream>
#include <memory>
#include <vector>

class InvalidGameStateOperation : public std::exception {
  const std::string message = "Illegal Operation";
  public:
    const char *what();
};

class InvalidWinnerException : public std::exception {
  const std::string message = "No winner exists yet";
  public:
    const char *what();
};


class Board;
class Token;
class Tile;

class GameState {
    size_t playerTurn = 0; // ID of player next to act
    int winnerPlayer = -1; // garbage value until hasWon
    size_t diceroll, flexdiceroll;
    bool diceAreFresh = false;
    bool noValidMoves = false;
    std::unique_ptr<Board> board;
    void moveToNextPlayerTurn();
    void repeatPlayerTurn();
    void recalculatePassive();
  public:
    GameState();
    void rollDice();
    void skipTurn(); // only valid if no valid moves, skips to next player
    bool movePiece(size_t tokenId, size_t distance);
    bool moveValid(size_t tokenId, size_t distance) const; // checks if move is valid

    bool gameIsOver() const;
    size_t getWinner() const;
    bool hasNoValidMoves() const;
    bool requiresDiceRoll() const;

    size_t getDiceRoll() const;
    size_t getFlexDiceRoll() const;
    size_t getPlayerTurn() const;

    // Expose game entities for printing/display purposes
    const std::vector<std::vector<Tile*>>& getBoard() const;
    const std::vector<std::vector<Token*>>& getPlayersTokens() const;
    const std::vector<std::vector<Tile*>>& getPlayersPaths() const;

    // Will be changed

    friend std::istream& operator>>(std::istream&, GameState&);
};

#endif
