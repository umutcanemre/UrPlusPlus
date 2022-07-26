#ifndef URPLUSPLUS_GAMEVIEWER_H_
#define URPLUSPLUS_GAMEVIEWER_H_
#include "entityvisitor.h"
#include "gameobserver.h"
#include <vector>


class Tile;
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


enum class PathDirection {right, up, left, down, nulldir};

class GameViewer : public virtual EntityVisitor, public virtual GameObserver {
      virtual void processPathSegment(PathDirection currentDir,
        PathDirection nextDir, std::pair<size_t, size_t> pathCoord,
        const size_t playerId, const size_t tilesRemaining) = 0;
  protected:
      static char getTokenChar(const Token& t);
      void drawPath(const std::vector<std::vector<Tile*>> &gameboard,
          const std::vector<Tile*> &path, const size_t playerId);
  public:
    virtual ~GameViewer();
};

#endif
