#ifndef URPLUSPLUS_HUMAN_H_
#define URPLUSPLUS_HUMAN_H_

#include <vector>
#include "gamestate.h"
#include "player.h"


class Human : public Player {
    std::istream* in;
    std::ostream* out;

    static size_t charToTokenId(char c);

    void makeDiceRoll(GameState&) const override;
    void determineAndMakeMove(GameState&) override;
    void skipTurn(GameState&) const override;
  public:
    Human(std::istream* in, std::ostream* out);
    virtual ~Human();
};


#endif
