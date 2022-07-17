#ifndef URPLUSPLUS_TOKENSPEEDSTER_H_
#define URPLUSPLUS_TOKENSPEEDSTER_H_
#include "token.h"

class TokenSpeedster : public Token {
  public:
    TokenSpeedster(size_t playerId, size_t id, size_t row, size_t col);
    void acceptVisitor(GameViewer&) const override;
    ~TokenSpeedster();

};

#endif
