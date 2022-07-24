#ifndef URPLUSPLUS_LEVEL3AI_H_
#define URPLUSPLUS_LEVEL3AI_H_

#include <vector>
#include "ai.h"

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


class Level3AI : public AI {
    bool tileRollsDice = false;
    bool tileRepeatsTurn = false;
    bool tileGrantsInvulnerability = false;
    float tileAvgMoveDist = 0;
    
  protected:
    std::vector<std::pair<int, std::pair<size_t, size_t>>> 
    assignPriorities(std::vector<std::pair<size_t, size_t>> &, const GameState&) override;
    std::pair<size_t, size_t> findMove(const GameState&) override;
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

    Level3AI();
    ~Level3AI();
};

#endif 