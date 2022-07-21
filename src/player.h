#ifndef URPLUSPLUS_PLAYER_H_
#define URPLUSPLUS_PLAYER_H_

// for now, all player-related classes are in the same header file

#include <vector>
#include "gamestate.h"

class Player {
    size_t playerId;
  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    Player();
    virtual void playTurn(GameState&) const = 0;
    virtual ~Player();
};


class Human : public Player {
  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    Human();
    void playTurn(GameState&) const override;
    virtual ~Human();
};

class AI : public Player {
  protected:
    virtual int findMove(GameState&) = 0;
  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    Human();
    void playTurn(GameState&) const override;
    virtual ~Human();
};

#endif
