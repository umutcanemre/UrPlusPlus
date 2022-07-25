#ifndef URPLUSPLUS_TILENULL_H_
#define URPLUSPLUS_TILENULL_H_
#include "tile.h"

class TileNull : public Tile {
  public:
    void acceptVisitor(EntityVisitor&) const override;
    TileNull(size_t row, size_t col);
};

#endif
