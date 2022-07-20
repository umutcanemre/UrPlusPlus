#ifndef URPLUSPLUS_TOKEN_H_
#define URPLUSPLUS_TOKEN_H_
#include <iostream>
#include <vector>

class GameViewer;
class Tile;

class Token {
    size_t pathIndex = 0;
    size_t playerId, id;
    std::pair<size_t, size_t> position;

    // virtual void setPathIndex();
    // NVI private methods
    // virtual void passiveAbility(const std::vector<Tile*>& path);
    // virtual bool checkValid(int diceroll, int flexroll, int move) const;
    // virtual bool captureAbility();
  public:
    // id getters
    size_t getPlayerId() const;
    size_t getTokenId() const;

    // Position get/set
    void updatePosition(std::pair<size_t, size_t>);
    std::pair<size_t, size_t> getPosition();
    
    // Abilities
    // void activatePassive(const std::vector<Tile*>& path);
    
    // returns true if the player's turn ends
    // bool activateCapture();
    // Predicate for checking if move is valid from the perspective
    // of the token
    // bool isValidMove(int diceroll, int flexroll, int move) const;
    // visitor design pattern
    virtual void acceptVisitor(GameViewer&) const = 0;
    virtual ~Token() = 0;

    Token(size_t playerId, size_t id, size_t row, size_t col);
};

#endif
