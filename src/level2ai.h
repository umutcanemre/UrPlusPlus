#ifndef URPLUSPLUS_LEVEL2AI_H_
#define URPLUSPLUS_LEVEL2AI_H_

#include <vector>
#include "ai.h"

class GameState;

class TileStart;
class TileEnd;
class TileTornado;
class TileBlackHole;
class TileLucky;
class TileRosette;
class TileBasic;
class TileNull;


class Level2AI : public AI {
  protected:
    std::vector<std::pair<float, std::pair<size_t, size_t>>> assignPriorities(
      const std::vector<std::pair<size_t, size_t>> &, const GameState&) override;
    std::pair<size_t, size_t> findMove(const GameState&) override;
  public:
    Level2AI();
    ~Level2AI();
};

#endif 