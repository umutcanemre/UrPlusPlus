#ifndef URPLUSPLUS_TILEEND_H_
#define URPLUSPLUS_TILEEND_H_
#include "tile.h"

class TileEnd : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileEnd();
};

#endif
