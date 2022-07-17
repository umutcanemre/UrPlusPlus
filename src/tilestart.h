#ifndef URPLUSPLUS_TILESTART_H_
#define URPLUSPLUS_TILESTART_H_
#include "tile.h"

class TileStart : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileStart();
};

#endif
