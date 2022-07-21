#ifndef URPLUSPLUS_TOKENFLEXIBLE_H_
#define URPLUSPLUS_TOKENFLEXIBLE_H_
#include "token.h"

class TokenFlexible : public Token {
    // overwritten methods
    bool checkValid(int, int, int) const override;
  public:
    TokenFlexible(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);
    
    void acceptVisitor(GameViewer&) const override;

    ~TokenFlexible();
};

#endif
