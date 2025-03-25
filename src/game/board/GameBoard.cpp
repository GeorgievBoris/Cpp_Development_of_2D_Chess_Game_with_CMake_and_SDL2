// Corresponding header
#include "game/board/GameBoard.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "manager_utils/drawing/Fbo.h"

extern const int32_t GAME_X_POS_SHIFT;
extern const int32_t GAME_Y_POS_SHIFT;

GameBoard::~GameBoard(){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
// check if the the bug of Lecture 11 Game 2/4 is fixed as shown in the ~GameBoard() below...
// .. then delete the entire dtor here!!
    if(isActiveTimerId(_blinkTimerId)){
        stopTimer(_blinkTimerId);
    }

    if(isActiveTimerId(_enPassantTimerId)){ // NOT added by Zhivko
        stopTimer(_enPassantTimerId); // NOT added by Zhivko
    } // NOT added by Zhivko
}

int32_t GameBoard::init(int32_t boardRsrcId, int32_t targetRsrcId,
                                int32_t moveTilesRsrcId, int32_t blinkTimerId, int32_t enPassantTimerId){ // "int32_t enPassantTimerId" is NOT added by Zhivko

    _boardImg.create(boardRsrcId, Point(GAME_X_POS_SHIFT,GAME_Y_POS_SHIFT)); // Point(FIRST_TILE_X_POS_SHIFT,FIRST_TILE_Y_POS_SHIFT) added by me
    _targetImg.create(targetRsrcId);
    _targetImg.hide();

    _blinkTimerId=blinkTimerId;
    _enPassantTimerId=enPassantTimerId; // NOT added by Zhivko

    if(EXIT_SUCCESS!=_moveSelector.init(moveTilesRsrcId)){
        std::cerr<<"_moveSelector.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

void GameBoard::drawGameBoardOnFbo(Fbo& fbo) const{
    const Point pos=_boardImg.getPosition(); // a correction - NOT added by Zhivko
    _boardImg.setPosition(Point::ZERO); // a correction - NOT added by Zhivko

    fbo.addWidget(_boardImg); // original code by Zhivko

    _boardImg.setPosition(pos);// a correction - NOT added by Zhivko
}

void GameBoard::drawGameBoardOnly() const{
    _boardImg.draw();
}

void GameBoard::draw() const{
    _targetImg.draw();
    _moveSelector.draw();
}

void GameBoard::restart(){ // GameBoard::restart() method is NOT added by Zhivko    
    if(isActiveTimerId(_blinkTimerId)){
        GameBoard::onPieceUngrabbed();
    }
}

void GameBoard::show(){ // GameBoard::show() method is NOT added by Zhivko
    _boardImg.show(); 
}

void GameBoard::hide(){ // GameBoard::hide() method is NOT added by Zhivko
    _boardImg.hide();
}

void GameBoard::onPieceGrabbed(const BoardPos& boardPos, const std::vector<TileData>& moveTiles){
    _currMoveTiles=moveTiles;
    _moveSelector.markTiles(_currMoveTiles);
    _targetImg.show();

    GameBoard::shiftPositions(boardPos); // NOT added by Zhivko
    //_targetImg.setPosition(BoardUtils::getAbsPos(boardPos)); // original code by Zhivko
    
    startTimer(500,_blinkTimerId,TimerType::PULSE);
}

void GameBoard::onPieceUngrabbed(){
    _moveSelector.unmarkTiles();

    _currMoveTiles.clear();
    _targetImg.hide();
    if(isActiveTimerId(_blinkTimerId)){
        stopTimer(_blinkTimerId);
    }

    if(isActiveTimerId(_enPassantTimerId)){
        stopTimer(_enPassantTimerId);
    }
}

void GameBoard::onTimeout(int32_t timerId){

    // if(timerId!=_blinkTimerId){ // added originally by Zhivko
    //     std::cerr<<"Received unsupported timerId: "<<timerId<<std::endl; // added originally by Zhivko
    //     return; // added originally by Zhivko
    // } // added originally by Zhivko

    // if(_targetImg.isVisible()){ // added originally by Zhivko
    //     _targetImg.hide(); // added originally by Zhivko
    //     return; // added originally by Zhivko
    // }
    // _targetImg.show(); // added originally by Zhivko


    if(timerId!=_blinkTimerId && timerId!=_enPassantTimerId){
        std::cerr<<"Received unsupported timerId: "<<timerId<<std::endl;
        return;
    }

    if(timerId==_blinkTimerId){
        if(_targetImg.isVisible()){
            _targetImg.hide();
            return; 
        }
        _targetImg.show();
        return;
    }
    
    _moveSelector.blinkEnPassant();

}

bool GameBoard::isMoveAllowed(const BoardPos& pos) const {
    // the below for loop can be alternatively replaced with "std::find_if()" algorithm - try it later!
    for(const TileData& moveTile:_currMoveTiles){
        if(pos==moveTile.boardPos){
            if(TileType::GUARD!=moveTile.tileType){
                return true;
            } 
        }
    }
    return false;
}

void GameBoard::shiftPositions(const BoardPos& boardPos){ // GameBoard::shiftPositions() is NOT added by Zhivko
    const Point posBoardImg=_boardImg.getPosition(); // this correction is NOT added by Zhivko
    _moveSelector.shiftMoveTilesPos(posBoardImg); // this correction is NOT added by Zhivko

    const Point absPos=BoardUtils::getAbsPos(boardPos); // this correction is NOT added by Zhivko
    _targetImg.setPosition(absPos.x+posBoardImg.x,absPos.y+posBoardImg.y); // this correction is NOT added by Zhivko
}

void GameBoard::onEnPassant(const BoardPos& boardPos) { // GameBoard::onEnPassant() is NOT added by Zhivko

    const Point boardPosToPoint=BoardUtils::getAbsPos(boardPos);
    const Point boardImgPos=_boardImg.getPosition();
    const BoardPos shiftedBoardPos=BoardUtils::getBoardPos((Point(boardPosToPoint.x+boardImgPos.x,boardPosToPoint.y+boardImgPos.y)));

    _moveSelector.onEnPassant(shiftedBoardPos);
    startTimer(500,_enPassantTimerId,TimerType::PULSE);
}