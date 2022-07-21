#ifndef URPLUSPLUS_CLIVIEW_H_
#define URPLUSPLUS_CLIVIEW_H_
#include <iostream>
#include <vector>
#include <functional>
#include "gameviewer.h"

class Token;
class Tile;

class CLIView : public GameViewer {
    static char getTokenChar(const Token& t);
    std::ostream* out;
    std::vector<std::vector<char>> outBuffer;
    void doNotify(const GameState&) override;

    void drawInfoTokens
        (const std::vector<std::vector<Token*>>& playersTokens,
        const std::string &info,
        std::function<bool(const Token*)> pred) const;
    void drawPath(const std::vector<std::vector<Tile*>> &gameboard,
        const std::vector<Tile*> &path) const;
    void drawBoard(const std::vector<std::vector<Tile*>> &gameboard);
  public:
    void visitTileStart(const TileStart&) override;
    void visitTileEnd(const TileEnd&) override;
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

    CLIView(std::ostream*);
    ~CLIView();
};

#endif
