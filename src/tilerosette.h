#ifndef URPLUSPLUS_TILEROSETTE_H_
#define URPLUSPLUS_TILEROSETTE_H_
#include "tile.h"

class TileRosette : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileRosette();
};

#endif
