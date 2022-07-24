#include "tiletornado.h"
#include "entityvisitor.h"

// bool TileTornado::rollsDice() const { 
//     return true;
// }
// int Tile::avgPlayerMoveDistance(size_t location, const std::vector<Tile*> &path) const {
//     return 0;
// }

void TileTornado::acceptVisitor(EntityVisitor& g) const {
    g.visitTileTornado(*this);
}

TileTornado::TileTornado(size_t row, size_t col): Tile{row, col} {}
