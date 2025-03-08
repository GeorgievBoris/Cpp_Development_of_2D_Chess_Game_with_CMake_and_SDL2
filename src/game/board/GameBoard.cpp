// Corresponding header
#include "game/board/GameBoard.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "manager_utils/drawing/Fbo.h"


GameBoard::~GameBoard(){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
// check if the the bug of Lecture 11 Game 2/4 is fixed as shown in the ~GameBoard() below...
// .. then delete the entire dtor here!!
    if(isActiveTimerId(_blinkTimerId)){
        stopTimer(_blinkTimerId);
    }
}

int32_t GameBoard::init(int32_t boardRsrcId, int32_t targetRsrcId,
                                int32_t moveTilesRsrcId, int32_t blinkTimerId){
    _boardImg.create(boardRsrcId);
    _targetImg.create(targetRsrcId);
    _targetImg.hide();

    _blinkTimerId=blinkTimerId;

    if(EXIT_SUCCESS!=_moveSelector.init(moveTilesRsrcId)){
        std::cerr<<"_moveSelector.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}

void GameBoard::drawGameBoardOnFbo(Fbo& fbo) const{
    fbo.addWidget(_boardImg);
}

void GameBoard::drawGameBoardOnly() const{
    _boardImg.draw();
}

void GameBoard::draw() const{
    _targetImg.draw();
    _moveSelector.draw();
}

void GameBoard::onPieceGrabbed(const BoardPos& boardPos, const std::vector<TileData>& moveTiles){
    _currMoveTiles=moveTiles;
    _moveSelector.markTiles(_currMoveTiles);

    _targetImg.show();
    _targetImg.setPosition(BoardUtils::getAbsPos(boardPos));
    
    startTimer(500,_blinkTimerId,TimerType::PULSE);
}

void GameBoard::onPieceUngrabbed(){
    _moveSelector.unmarkTiles();

    _currMoveTiles.clear();
    _targetImg.hide();
    if(isActiveTimerId(_blinkTimerId)){
        stopTimer(_blinkTimerId);
    }
}

void GameBoard::onTimeout(int32_t timerId){
    if(timerId!=_blinkTimerId){
        std::cerr<<"Received unsupported timerId: "<<timerId<<std::endl;
        return;
    }

    if(_targetImg.isVisible()){
        _targetImg.hide();
        return;
    }
    _targetImg.show();
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