#ifndef URPLUSPLUS_TILE_H_
#define URPLUSPLUS_TILE_H_

#include <vector>
#include "token.h"
#include "player.h"

class Tile {
    Token* occupant = nullptr;
    std::pair<size_t, size_t> position;

    // Specialized check for tile abilities
    // Currently used only for rosettes, but easily extendable
    // Returns true if movable
    virtual bool specialCheck();

    // NVI functions
    virtual bool doOnMoveSuccess(Token*, const std::vector<Tile*> &path);
    // virtual bool rollsDice() const; 
    // virtual bool extraTurn() const;
    // virtual bool makesInvulnerable() const;
    // virtual int avgPlayerMoveDistance(size_t location, const std::vector<Tile*> &path) const;

  public:
    bool tileAvailable(Token*);

    // returns false if we should advance to next player's turn and 
    // executes the functionality needed before advancing to next turn
    bool onMoveSuccess(Token*, const std::vector<Tile*> &path);

    // // tile characterization functions
    // // returns true if this tile grants an extra turn on move success
    // bool grantsExtraTurn() const;
    // // returns true if this tile grants an extra dice roll (but not turn) 
    // bool grantsDiceRoll() const;
    // // returns true if this tile grants invulnerability 
    // bool grantsInvulnerability() const;
    // // returns -3 if this tile sends token to start of path
    // //         -1 if this tile sends token backward in the path
    // //         0 if this tile has no effect on token position
    // //         +1 if this tile sends token forward in the path
    // //         +4 if this tile sends token to the end of the path 
    // // since this is up to chance this is calculated based on the likelihood of occurrence 
    // int sendsPlayerForward(size_t location, const std::vector<Tile*> &path) const;

    Tile(size_t row, size_t col);

    Token* getOccupant() const;
    void setOccupant(Token*);
    std::pair<size_t, size_t> getPosition() const;



    virtual void acceptVisitor(GameViewer&) const = 0;
    virtual void acceptVisitor(AI&) const = 0;
    virtual ~Tile();
};

#endif
