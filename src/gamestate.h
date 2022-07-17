#ifndef URPLUSPLUS_GAMESTATE_H_
#define URPLUSPLUS_GAMESTATE_H_

#include <iostream>
#include <memory>
#include <vector>

class Board;
class Token;
class Tile;

class GameState {
    size_t playerTurn = 0;
    int diceroll, flexdiceroll;
    std::unique_ptr<Board> board;
    bool moveValid();
  public:
    const std::vector<std::vector<Tile*>>& getBoard() const;
    void rollDice();
    void movePiece(size_t tokenId, size_t distance);
    GameState();

    friend std::istream& operator>>(std::istream&, GameState&);
};

#endif
