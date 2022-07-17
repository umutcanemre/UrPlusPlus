#ifndef URPLUSPLUS_CLIVIEW_H_
#define URPLUSPLUS_CLIVIEW_H_
#include <iostream>
#include <vector>
#include "gameviewer.h"


class CLIView : public GameViewer {
    static char getTokenChar(const Token& t);
    std::ostream* out;
    std::vector<std::vector<char>> outBuffer;
    void doNotify(const GameState&) override;
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
