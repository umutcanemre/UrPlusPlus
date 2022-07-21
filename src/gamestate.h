#ifndef URPLUSPLUS_GAMESTATE_H_
#define URPLUSPLUS_GAMESTATE_H_

#include <iostream>
#include <memory>
#include <vector>

class Board;
class Token;
class Tile;

class GameState {
    size_t playerTurn = 0; // ID of player next to act
    size_t diceroll, flexdiceroll;
    std::unique_ptr<Board> board;
    bool moveValid(size_t tokenId, size_t distance);
  
  public:
    GameState();
    bool rollDice();
    bool movePiece(size_t tokenId, size_t distance);
    
    // Will be changed
    const std::vector<std::vector<Tile*>>& getBoard() const;

    friend std::istream& operator>>(std::istream&, GameState&);
};

#endif
