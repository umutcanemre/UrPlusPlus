#ifndef URPLUSPLUS_AI_H_
#define URPLUSPLUS_AI_H_

#include <vector>
#include "entityvisitor.h"
#include "player.h"

class GameState;

class TileStart;
class TileEnd;
class TileTornado;
class TileBlackHole;
class TileLucky;
class TileRosette;
class TileBasic;
class TileNull;

class Token;
class TokenBasic;
class TokenAssassin;
class TokenFlexible;
class TokenSpeedster;
class TokenSupporter;


class AIMadeInvalidMoveException : public std::exception {
  public:
    char *what();
};

class AI : public virtual Player, public virtual EntityVisitor {
    float visitedTileScore = 0;
    float visitedTokenScore = 0;

    void makeDiceRoll(GameState&) const override;
    void determineAndMakeMove(GameState&) override;
    void skipTurn(GameState&) const override;

  protected:
    void setTokenScore(float);
    float getTokenScore() const;
    void setTileScore(float);
    float getTileScore() const;
    
    static bool moveIsNotRepeated(const 
      std::vector<std::pair<size_t, size_t>> &, size_t tokenId, size_t rollNum);
    // returns a tokenId and distance pair
    std::vector<std::pair<size_t, size_t>> findAllValidMoves(const GameState&) const;
    // returns the tokenID and distance pairs with a weight assigned to each and sorted in order of weight
    virtual std::vector<std::pair<float, std::pair<size_t, size_t>>> 
    assignPriorities(const std::vector<std::pair<size_t, size_t>> &, const GameState&);
    // findMove algorithm for each of the AIs
    virtual std::pair<size_t, size_t> findMove(const GameState&) = 0;
  public:
    void visitTileTornado(const TileTornado&) override;
    void visitTileBlackHole(const TileBlackHole&) override;
    void visitTileLucky(const TileLucky&) override;
    void visitTileRosette(const TileRosette&) override;
    void visitTileBasic(const TileBasic&) override;
    void visitTileNull(const TileNull&) override;

    void visitTokenBasic(const TokenBasic&) override;
    void visitTokenAssassin(const TokenAssassin&) override;
    void visitTokenFlexible(const TokenFlexible&) override;
    void visitTokenSpeedster(const TokenSpeedster&) override;
    void visitTokenSupporter(const TokenSupporter&) override;

    AI();
    virtual ~AI();
};


#endif 