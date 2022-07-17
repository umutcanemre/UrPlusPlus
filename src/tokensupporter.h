#ifndef URPLUSPLUS_TOKENSUPPORTER_H_
#define URPLUSPLUS_TOKENSUPPORTER_H_
#include "token.h"

class TokenSupporter : public Token {
  public:
    TokenSupporter(size_t playerId, size_t id, size_t row, size_t col);
    void acceptVisitor(GameViewer&) const override;
    ~TokenSupporter();
};

#endif
