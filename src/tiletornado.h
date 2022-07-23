#ifndef URPLUSPLUS_TILETORNADO_H_
#define URPLUSPLUS_TILETORNADO_H_
#include "tile.h"

class TileTornado : public Tile {
  // NVI overrides
    // bool rollsDice() const override; 
    // int avgPlayerMoveDistance(size_t location, const std::vector<Tile*> &path) const override;
  public:
    void acceptVisitor(GameViewer&) const override;
    TileTornado(size_t row, size_t col);
};

#endif
