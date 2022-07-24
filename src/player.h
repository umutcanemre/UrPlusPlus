#ifndef URPLUSPLUS_PLAYER_H_
#define URPLUSPLUS_PLAYER_H_

// for now, all player-related classes are in the same header file

#include <vector>
#include "gamestate.h"


class Player {
    static size_t nextPlayerId;
    const size_t playerId;
    virtual void makeDiceRoll(GameState&) const = 0;
    virtual void determineAndMakeMove(GameState&) = 0;
    virtual void skipTurn(GameState&) const = 0;

  public:
    Player();
    size_t getPlayerId() const;
    // void setPlayerId(size_t);
    void playTurn(GameState&);
    virtual ~Player();
};


#endif
