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

    if(_enPassantMoveTile){ // NOT added by Zhivko
        _enPassantMoveTile->show(); // NOT added by Zhivko
        _enPassantMoveTile=nullptr; // NOT added by Zhivko
    }
    if(_enPassantTakeTile){ // NOT added by Zhivko
        _enPassantTakeTile->show(); // NOT added by Zhivko
        _enPassantTakeTile=nullptr; // NOT added by Zhivko
    }
    if(_castlingKingTile){ // NOT added by Zhivko
        _castlingKingTile->show(); // NOT added by Zhivko
        _castlingKingTile=nullptr; // NOT added by Zhivko
    }

}

void MoveSelector::shiftMoveTilesPos(const Point& posShift) { // MoveSelector::shiftMoveTilesPos() is NOT added by Zhivko
    for(size_t i=0;i<_activeTiles;++i){
        const Point pos=_tileImgs[i].getPosition();
        _tileImgs[i].setPosition(pos.x+posShift.x,pos.y+posShift.y);
    }
}

void MoveSelector::onEnPassant(const BoardPos& boardPos){ // MoveSelector::onEnPassant() is NOT added by Zhivko
    for(size_t idx=0;idx<_activeTiles;++idx){
        if(boardPos!=BoardUtils::getBoardPos(_tileImgs[idx].getPosition())){
            continue;
        }

        if(!_enPassantTakeTile){
            _enPassantTakeTile=&_tileImgs[idx];
            return;
        }

        if(!_enPassantMoveTile){
            _enPassantMoveTile=&_tileImgs[idx];
            return;
        }
    }
}

void MoveSelector::onCastling(const BoardPos& boardPos){ // MoveSelector::onCastling() is NOT added by Zhivko
    for(size_t idx=0;idx<_activeTiles;++idx){
        if(boardPos!=BoardUtils::getBoardPos(_tileImgs[idx].getPosition())){
            continue;
        }
        _castlingKingTile=&_tileImgs[idx];
        break;
    }
}

void MoveSelector::blinkCastling(){ // MoveSelector::blinkCastling() is NOT added by Zhivko
    if(_castlingKingTile->isVisible()){
        _castlingKingTile->hide();
        return;
    }
    _castlingKingTile->show();
}

void MoveSelector::blinkEnPassant(){ // MoveSelector::blinkEnPassant() is NOT added by Zhivko
    if(_enPassantTakeTile->isVisible()){
        _enPassantTakeTile->hide();
        _enPassantMoveTile->hide();
        return;
    }
    _enPassantTakeTile->show();
    _enPassantMoveTile->show();
}

const Image* MoveSelector::getCastlingKingTilePtr() const { // MoveSelector::getCastlingKingTilePtr() is NOT added by Zhivko
    return _castlingKingTile;
}