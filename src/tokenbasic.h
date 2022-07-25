#ifndef URPLUSPLUS_TOKENBASIC_H_
#define URPLUSPLUS_TOKENBASIC_H_
#include "token.h"

class TokenBasic : public Token {
  public:
    TokenBasic(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);
    
    // overwritten methods
    void acceptVisitor(EntityVisitor&) const override;
    ~TokenBasic();
};

#endif
