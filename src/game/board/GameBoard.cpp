// Corresponding header
#include "game/board/GameBoard.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

GameBoard::~GameBoard(){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
    if(isActiveTimerId(_blinkTimerId)){
        stopTimer(_blinkTimerId);
    }
}

int32_t GameBoard::init(int32_t boardRsrcId, int32_t targetRsrcId, int32_t blinkTimerId){
    _boardImg.create(boardRsrcId);
    _targetImg.create(targetRsrcId);
    _targetImg.hide();

    _blinkTimerId=blinkTimerId;
    
    return EXIT_SUCCESS;
}

void GameBoard::draw() const{
    _boardImg.draw();
    _targetImg.draw();
}

void GameBoard::onPieceGrabbed(const BoardPos& boardPos){
    _targetImg.setPosition(BoardUtils::getAbsPos(boardPos));
    _targetImg.show();
    startTimer(500,_blinkTimerId,TimerType::PULSE);
}

void GameBoard::onPieceUngrabbed(){
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