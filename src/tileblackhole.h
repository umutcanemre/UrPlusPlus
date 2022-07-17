#ifndef URPLUSPLUS_TILEBLACKHOLE_H_
#define URPLUSPLUS_TILEBLACKHOLE_H_
#include "tile.h"

class TileBlackHole : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileBlackHole();
};

#endif
