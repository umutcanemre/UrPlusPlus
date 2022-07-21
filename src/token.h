#ifndef URPLUSPLUS_TOKEN_H_
#define URPLUSPLUS_TOKEN_H_
#include <iostream>
#include <vector>

class GameViewer;
class Tile;

class Token {
    // (row, col) pair
    size_t playerId, id;
    // is the tokens index on it's path + 1,
    // e.g. 0 is off the board, 1 is on the first tile on the path
    std::pair<size_t, size_t> position;
    size_t pathProgress = 0;
    bool isProtectedByTile = false;

    // virtual void setPathIndex();
    // NVI private methods
    // virtual void passiveAbility(const std::vector<Tile*>& path);
    // virtual bool checkValid(int diceroll, int flexroll, int move) const;
    // virtual bool captureAbility();
  public:
    // id getters
    size_t getPlayerId() const;
    size_t getTokenId() const;
    size_t getPathProgress() const;
    bool getIsProtected() const;
    void setIsProtected(bool);

    // Position get/set
    void updatePosition(std::pair<size_t, size_t>, const std::vector<Tile*>& );
    std::pair<size_t, size_t> getPosition();

    // Abilities
    // activates the passive ability of the token (if any)
    virtual void activatePassive(const std::vector<Tile*>& );
    // returns false if the player's turn ends - true if it has an 
    // on-capture ability
    virtual bool activateCapture();
    // Predicate for checking if move is valid from the perspective
    // of the token - different depending on the token type
    virtual bool isValidMove(int, int, int) const;
    // visitor design pattern
    virtual void acceptVisitor(GameViewer&) const = 0;
    virtual ~Token() = 0;

    Token(size_t playerId, size_t id, size_t row, size_t col, size_t pathProgress);
};

#endif
