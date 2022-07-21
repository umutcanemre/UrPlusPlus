#ifndef URPLUSPLUS_PLAYER_H_
#define URPLUSPLUS_PLAYER_H_

// for now, all player-related classes are in the same header file

#include <vector>
#include "gamestate.h"

class Player {
    size_t playerId;
    virtual void makeDiceRoll(GameState&) const = 0;
    virtual void determineAndMakeMove(GameState&) const = 0;
    virtual void skipTurn(GameState&) const = 0;

  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    Player();
    void playTurn(GameState&) const;
    virtual ~Player();
};


class Human : public Player {
    std::istream* in;
    std::ostream* out;

    static size_t charToTokenId(char c);

    void makeDiceRoll(GameState&) const override;
    void determineAndMakeMove(GameState&) const override;
    void skipTurn(GameState&) const override;
  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    Human(std::istream* in, std::ostream* out);
    virtual ~Human();
};

class AIMadeInvalidMoveException : public std::exception {
  public:
    char *what();
};

class AI : public Player {
    void makeDiceRoll(GameState&) const override;
    void determineAndMakeMove(GameState&) const override;
    void skipTurn(GameState&) const override;
  protected:
    virtual std::pair<size_t, size_t> findMove(const GameState&) const = 0;
  public:
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return
    AI();
    virtual ~AI();
};

#endif
