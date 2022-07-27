#include "tokensupporter.h"
#include "entityvisitor.h"
#include "tile.h"

void TokenSupporter::passiveAbility(std::vector<Tile*>& path) {
    // remove isProtected status from currently protecting tile
    if (isProtecting) {
        isProtecting->setIsProtected(false);
    }
    if (getPathProgress() != 0) {
        // find the next tile to protect
        for (size_t i=1; i<4; i++) {
            try {
                Token * t = path.at(i + getPathProgress() - 1)->getOccupant();
                if (t && t->getPlayerId() == getPlayerId()) { 
                    // if found add isProtected status to it
                    isProtecting = t;
                    isProtecting->setIsProtected(true);
                    return;
                }
            } catch (std::out_of_range &) {
                isProtecting = nullptr;
                return; // no tokens to protect before end of path
            }
        }
    }
    
    // no tokens were found
    isProtecting = nullptr;
}


void TokenSupporter::acceptVisitor(EntityVisitor& g) const {
    g.visitTokenSupporter(*this);
};

TokenSupporter::TokenSupporter(size_t playerId, size_t id,
    size_t row, size_t col, size_t pathProgress):
    Token{playerId, id, row, col, pathProgress} {};

TokenSupporter::~TokenSupporter() {}
