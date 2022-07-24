#ifndef URPLUSPLUS_TILE_H_
#define URPLUSPLUS_TILE_H_

#include <vector>
#include "token.h"

class Tile {
    Token* occupant = nullptr;
    std::pair<size_t, size_t> position;

    // Specialized check for tile abilities
    // Currently used only for rosettes, but easily extendable
    // Returns true if movable
    virtual bool specialCheck();

    // NVI functions
    virtual bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path);

  public:
    bool tileAvailable(Token*);

    // returns false if we should advance to next player's turn and 
    // executes the functionality needed before advancing to next turn
    bool onMoveSuccess(Token*, const std::vector<Tile*> &path);

    Tile(size_t row, size_t col);

    Token* getOccupant() const;
    void setOccupant(Token*);
    std::pair<size_t, size_t> getPosition() const;



    virtual void acceptVisitor(EntityVisitor&) const = 0;
    virtual ~Tile();
};

#endif
