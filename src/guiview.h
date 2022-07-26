#ifndef URPLUSPLUS_GUIVIEW_H_
#define URPLUSPLUS_GUIVIEW_H_

#include <vector>
#include "gameviewer.h"

class Screen;
struct Colour;

class GUIView : public GameViewer {
    Screen* screen;
    int xPos, yPos;
    const int dimension = 50; // size of all square assets
    void doNotify(const GameState&) override;

    int boardStartX, boardStartY;

    void drawInfoTokens(const std::vector<std::vector<Token*>>& playersTokens,
        const std::pair<int, int> &startXY,
        const std::pair<int, int> &playerIncXY, const std::pair<int, int> &incXY,
        std::function<bool(const Token*)> pred);

    static Colour getTokenColour(const Token& t);

    void drawHeader(const std::string message, const std::pair<int, int> &startXY, const Colour &c);
    void drawToken(const std::string &key, const Token& t);
    void drawBoardTokens(const std::vector<std::vector<Tile*>> &gameboard);

    virtual void processPathSegment(PathDirection currentDir,
        PathDirection nextDir, std::pair<size_t, size_t> pathCoord,
        const size_t playerId, const size_t tilesRemaining) override;

    void drawBoard(const std::vector<std::vector<Tile*>> &gameboard);
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

    GUIView(Screen*);
    ~GUIView();
};

#endif
