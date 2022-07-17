#ifndef URPLUSPLUS_BOARD_H_
#define URPLUSPLUS_BOARD_H_

#include <vector>
#include <memory>
#include <map>
#include "tile.h"

class BoardParseException : public std::exception {
  public:
    char *what();
};

class Board {
    void reset();
    // paths for each player
    std::vector<std::vector<Tile*>> paths;
    // tokens of each player
    std::vector<std::vector<std::unique_ptr<Token>>> playersTokens;
    // 2d map of tiles for game board
    std::vector<std::vector<std::unique_ptr<Tile>>> gameMap;

    std::vector<std::vector<Tile*>> nonOwningGameMap;
    std::vector<std::vector<Token*>> nonOwningTokens;

  public:
    const static size_t playerCount = 2;
    const std::vector<std::vector<Tile*>>& getGameMap() const;
    // const std::vector<std::vector<Token*>>& getPlayersTokens();
    // const std::vector<std::vector<std::vector<Tile*>>>& getPlayersPaths();

    friend std::istream& operator>>(std::istream&, Board&);
};

#endif
