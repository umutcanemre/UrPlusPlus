#ifndef URPLUSPLUS_TOKENSPEEDSTER_H_
#define URPLUSPLUS_TOKENSPEEDSTER_H_
#include "token.h"

class TokenSpeedster : public Token {
    // speedster ability has been used?
    int uses = 1;
    // overwritten methods
    void manualAbility() override;
    bool manualIsAvailable() const override;
    bool checkValid(int diceRoll, int flexRoll, int move) const override;
    std::vector<size_t> validMoveDists(int diceRoll, int flexRoll) const override;
  public:
    TokenSpeedster(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress, size_t uses);
    TokenSpeedster(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);

    // overwritten methods
    void acceptVisitor(GameViewer&) const override;

    ~TokenSpeedster();

};

#endif
