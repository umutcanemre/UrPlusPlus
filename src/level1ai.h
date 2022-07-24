#ifndef URPLUSPLUS_LEVEL1AI_H_
#define URPLUSPLUS_LEVEL1AI_H_

#include <vector>
#include "ai.h"

#include "entityvisitor.h"
#include "tile.h"
#include "tilebasic.h"
#include "tileblackhole.h"
#include "tilelucky.h"
#include "tilerosette.h"
#include "tiletornado.h"
#include "tilenull.h"


class Level1AI : public AI {
  protected:
    std::pair<size_t, size_t> findMove(const GameState&) override;
  public:
    Level1AI();
    ~Level1AI();
};


#endif 