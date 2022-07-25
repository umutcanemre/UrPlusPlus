#ifndef URPLUSPLUS_TILELUCKY_H_
#define URPLUSPLUS_TILELUCKY_H_
#include "tile.h"

class TileLucky : public Tile {
    bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path) override;
  public:
    void acceptVisitor(EntityVisitor&) const override;
    TileLucky(size_t row, size_t col);
};

#endif
