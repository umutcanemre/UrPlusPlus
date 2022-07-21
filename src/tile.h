#ifndef URPLUSPLUS_TILE_H_
#define URPLUSPLUS_TILE_H_

#include <vector>
#include "token.h"

class Tile {
    Token* occupant = nullptr;
    std::pair<size_t, size_t> position;

    // virtual bool checkMoveAvailable(Token*);
    // virtual bool doOnMoveSuccess(Token*, const std::vector<Token*> &path);

  public:
    // bool moveAvailable(Token*);
    // return true if we should advance to next player's turn
    // bool onMoveSuccess(Token*, const std::vector<Token*> &path);
    // return

    Tile(size_t row, size_t col);

    const Token* getOccupant() const;
    virtual void acceptVisitor(GameViewer&) const = 0;
    virtual ~Tile();
};

#endif
