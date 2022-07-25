#ifndef URPLUSPLUS_TILEBLACKHOLE_H_
#define URPLUSPLUS_TILEBLACKHOLE_H_
#include "tile.h"

class TileBlackHole : public Tile {
    bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path) override;
  public:
    void acceptVisitor(EntityVisitor&) const override;
    TileBlackHole(size_t row, size_t col);
};

#endif
