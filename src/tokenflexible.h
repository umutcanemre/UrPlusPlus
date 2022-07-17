#ifndef URPLUSPLUS_TOKENFLEXIBLE_H_
#define URPLUSPLUS_TOKENFLEXIBLE_H_
#include "token.h"

class TokenFlexible : public Token {
  public:
    TokenFlexible(size_t playerId, size_t id, size_t row, size_t col);
    void acceptVisitor(GameViewer&) const override;
    ~TokenFlexible();
};

#endif
