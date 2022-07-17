#ifndef URPLUSPLUS_TILENULL_H_
#define URPLUSPLUS_TILENULL_H_
#include "tile.h"

class TileNull : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileNull();
};

#endif
