#ifndef URPLUSPLUS_TOKENSUPPORTER_H_
#define URPLUSPLUS_TOKENSUPPORTER_H_
#include "token.h"

class TokenSupporter : public Token {
    // pointer to protected token
    Token *isProtecting = nullptr;

    // private functions
    void passiveAbility(std::vector<Tile*>& ) override;
  public:
    TokenSupporter(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);

    // overwritten methods
    void acceptVisitor(GameViewer&) const override;

    ~TokenSupporter();
};

#endif
