#ifndef URPLUSPLUS_ENTITYVISITOR_H_
#define URPLUSPLUS_ENTITYVISITOR_H_

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


class EntityVisitor {
  public:
    virtual void visitTileTornado(const TileTornado&) = 0;
    virtual void visitTileBlackHole(const TileBlackHole&) = 0;
    virtual void visitTileLucky(const TileLucky&) = 0;
    virtual void visitTileRosette(const TileRosette&) = 0;
    virtual void visitTileBasic(const TileBasic&) = 0;
    virtual void visitTileNull(const TileNull&) = 0;

    virtual void visitTokenBasic(const TokenBasic&) = 0;
    virtual void visitTokenAssassin(const TokenAssassin&) = 0;
    virtual void visitTokenFlexible(const TokenFlexible&) = 0;
    virtual void visitTokenSpeedster(const TokenSpeedster&) = 0;
    virtual void visitTokenSupporter(const TokenSupporter&) = 0;

    virtual ~EntityVisitor();
};



#endif
