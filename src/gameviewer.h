#ifndef URPLUSPLUS_GAMEVIEWER_H_
#define URPLUSPLUS_GAMEVIEWER_H_
#include "entityvisitor.h"

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


class GameViewer : public EntityVisitor {
    virtual void doNotify(const GameState&) = 0 ;
  public:
    void notify(const GameState&);

    virtual ~GameViewer();
};

#endif
