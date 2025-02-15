// Corresponding header
#include "game/board/MoveSelector.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

int32_t MoveSelector::init(int32_t rsrcId){
    for(Image& img:_tileImgs){
        img.create(rsrcId);
    }
    return EXIT_SUCCESS;
}

void MoveSelector::draw() const {
    for(size_t i=0;i<_activeTiles;++i){
        _tileImgs[i].draw();
    }
}

void MoveSelector::markTiles(const std::vector<TileData>& markedTiles) {
    _activeTiles=markedTiles.size();
    for(size_t i=0;i<_activeTiles;++i){
        _tileImgs[i].setPosition(BoardUtils::getAbsPos(markedTiles[i].boardPos));
        _tileImgs[i].setFrame(static_cast<int32_t>(markedTiles[i].tileType));
    }
}

void MoveSelector::unmarkTiles(){
    _activeTiles=0;
}