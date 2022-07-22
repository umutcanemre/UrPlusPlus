#ifndef URPLUSPLUS_PLAYER_H_
#define URPLUSPLUS_PLAYER_H_

// for now, all player-related classes are in the same header file

#include <vector>
#include "gamestate.h"

class Player {
    static size_t nextPlayerId;
    const size_t playerId;
    virtual void makeDiceRoll(GameState&) const = 0;
    virtual void determineAndMakeMove(GameState&) const = 0;
    virtual void skipTurn(GameState&) const = 0;

  public:
    Player();
    size_t getPlayerId() const;
    // void setPlayerId(size_t);
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
    // AI-unique functions
    // i dont know which of sortByPriorities and assignPriorities I will use yet
    // virtual void sortByPriorities(std::vector<std::pair<size_t, size_t>> &) const = 0;

  protected:
    static bool moveIsNotRepeated(std::vector<std::pair<size_t, size_t>>, size_t tokenId, size_t rollNum);
    // returns a tokenId and distance pair
    std::vector<std::pair<size_t, size_t>> findAllValidMoves(const GameState&) const;
    // returns the tokenID and distance pairs with a weight assigned to each and sorted in order of weight
    virtual std::vector<std::pair<int, std::pair<size_t, size_t>>> 
    assignPriorities(std::vector<std::pair<size_t, size_t>> &);
    // findMove algorithm for each of the AIs
    virtual std::pair<size_t, size_t> findMove(const GameState&) const = 0;
  public:
    AI();
    virtual ~AI();
};

class Level1AI : public AI {
  protected:
    std::pair<size_t, size_t> findMove(const GameState&) const override;
  public:
    Level1AI();
    ~Level1AI();
};

class Level2AI : public AI {
    // void sortByPriorities(std::vector<std::pair<size_t, size_t>> &) const override;
  protected:
    std::vector<std::pair<int, std::pair<size_t, size_t>>> 
    assignPriorities(std::vector<std::pair<size_t, size_t>> &) override;
    std::pair<size_t, size_t> findMove(const GameState&) const override;
  public:
    Level2AI();
    ~Level2AI();
};

class Level3AI : public AI {
    // void sortByPriorities(std::vector<std::pair<size_t, size_t>> &) const override;
  protected:
    std::vector<std::pair<int, std::pair<size_t, size_t>>> 
    assignPriorities(std::vector<std::pair<size_t, size_t>> &) override;
    std::pair<size_t, size_t> findMove(const GameState&) const override;
  public:
    Level3AI();
    ~Level3AI();
};


#endif
