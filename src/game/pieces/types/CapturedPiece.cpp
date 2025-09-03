// Corresponding header
#include "game/pieces/types/CapturedPiece.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"

void CapturedPiece::createPieceHalves(int32_t whitePiecesHalvesRsrcId, int32_t blackPiecesHalvesRsrcId, int32_t playerId,
                                 PieceType pieceType, const Point& absTopHalfPos){
    const int32_t pieceTypeIdx=static_cast<int32_t>(pieceType);

    int32_t topFrameIdx=0;
    int32_t bottomFrameIdx=1;
    for(int32_t i=0;i<pieceTypeIdx;++i){
        topFrameIdx+=2;
        bottomFrameIdx+=2;
    }

    int32_t rsrcId{};
    Defines::WHITE_PLAYER_ID==playerId ? rsrcId=whitePiecesHalvesRsrcId : rsrcId=blackPiecesHalvesRsrcId;
    
    _topHalf.create(rsrcId,absTopHalfPos);
    _topHalf.setFrame(topFrameIdx);
    const Point absBottomHalfPos(absTopHalfPos.x,absTopHalfPos.y+_topHalf.getHeight());
    _bottomHalf.create(rsrcId,absBottomHalfPos);
    _bottomHalf.setFrame(bottomFrameIdx);

    _topHalf.activateAlphaModulation();
    _bottomHalf.activateAlphaModulation();
    _isActive=true;
}

void CapturedPiece::moveHalves(){
    _topHalf.moveUp(1);
    _bottomHalf.moveDown(1);
}

void CapturedPiece::setHalvesOpacity(){
    const int32_t topHalfCurrOpacity=_topHalf.getOpacity();
    const int32_t bottomHalfCurrOpacity=_bottomHalf.getOpacity();
    const int32_t topHalfNewOpacity=topHalfCurrOpacity-5;
    const int32_t bottomHalfNewOpacity=bottomHalfCurrOpacity-5;

    if(ZERO_OPACITY>topHalfNewOpacity && ZERO_OPACITY>bottomHalfNewOpacity){
        _topHalf.setOpacity(ZERO_OPACITY);
        _bottomHalf.setOpacity(ZERO_OPACITY);
        _isActive=false;
        return;
    }
    _topHalf.setOpacity(topHalfNewOpacity);
    _bottomHalf.setOpacity(bottomHalfNewOpacity);
}

int32_t CapturedPiece::getHalvesOpacity() const{
    return (_topHalf.getOpacity()+_bottomHalf.getOpacity());
}

void CapturedPiece::draw() const{
    if(!_isActive){
        return;
    }
    _topHalf.draw();
    _bottomHalf.draw();
}

bool CapturedPiece::isActive() const{
    return _isActive;
}


