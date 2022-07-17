#ifndef URPLUSPLUS_TILELUCKY_H_
#define URPLUSPLUS_TILELUCKY_H_
#include "tile.h"

class TileLucky : public Tile {
  public:
    void acceptVisitor(GameViewer&) const override;
    TileLucky();
};

#endif
