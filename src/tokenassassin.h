#ifndef URPLUSPLUS_TOKENASSASSIN_H_
#define URPLUSPLUS_TOKENASSASSIN_H_
#include "token.h"

class TokenAssassin : public Token {
    bool captureAbility() override;
  public:
    TokenAssassin(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);

    void acceptVisitor(EntityVisitor&) const override;

    ~TokenAssassin();
};

#endif
