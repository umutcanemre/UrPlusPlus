#ifndef URPLUSPLUS_TILEBLACKHOLE_H_
#define URPLUSPLUS_TILEBLACKHOLE_H_
#include "tile.h"

class TileBlackHole : public Tile {
    // NVI overrides
    // int avgPlayerMoveDistance(size_t location, const std::vector<Tile*> &path) const override;
  public:
    void acceptVisitor(GameViewer&) const override;
    TileBlackHole(size_t row, size_t col);
};

#endif
