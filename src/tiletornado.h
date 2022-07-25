#ifndef URPLUSPLUS_TILETORNADO_H_
#define URPLUSPLUS_TILETORNADO_H_
#include "tile.h"

class TileTornado : public Tile {
    bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path) override;
    size_t generateRandomResult();
  public:
    void acceptVisitor(EntityVisitor&) const override;
    TileTornado(size_t row, size_t col);
};

#endif
