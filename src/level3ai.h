#ifndef URPLUSPLUS_LEVEL3AI_H_
#define URPLUSPLUS_LEVEL3AI_H_

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


class Level3AI : public AI {
    int valueOfOccupant(const Tile&);
  protected:
    std::vector<std::pair<int, std::pair<size_t, size_t>>> assignPriorities(
      const std::vector<std::pair<size_t, size_t>> &, const GameState&) override;
    std::pair<size_t, size_t> findMove(const GameState&) override;
  public:
    void visitTileTornado(const TileTornado&) override;
    void visitTileBlackHole(const TileBlackHole&) override;
    void visitTileLucky(const TileLucky&) override;
    void visitTileRosette(const TileRosette&) override;
    void visitTileBasic(const TileBasic&) override;
    void visitTileNull(const TileNull&) override;

    Level3AI();
    ~Level3AI();
};

#endif 