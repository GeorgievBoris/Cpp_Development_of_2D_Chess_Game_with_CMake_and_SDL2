// Corresponding header
#include "game/animations/WinnerAnimator.h"
// C system headers
// #include <cstdlib>
// #include <ctime>
// C++ system headers
#include <iostream>
#include <algorithm>
// Third-party headers
// Own headers
#include "utils/drawing/Color.h"
#include "game/defines/ChessDefines.h"
#include "game/proxies/PieceHandlerProxy.h"


int32_t WinnerAnimator::init(PieceHandlerProxy* pieceHandlerProxy, int32_t timerId, int32_t rsrcId, int32_t fontId){
    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, nullptr received for PieceHandlerProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==rsrcId){
        std::cerr<<"Error, invalid resource Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==timerId){
        std::cerr<<"Error, invalid timer Id provided"<<std::endl;
        return EXIT_FAILURE;
    }

    _pieceHandlerProxy=pieceHandlerProxy;
    _winnerStarRsrcId=rsrcId;
    _blinkWinnerTextTimerId=timerId;
    yRange.resize(90);
    std::generate(yRange.begin(),yRange.end(),Generator());
    _yRangeIter=yRange.begin();
    _onWinText.create("0",fontId,Colors::WHITE,Point(800,*_yRangeIter));
    _onWinText.hide();
    return EXIT_SUCCESS;
}

void WinnerAnimator::draw() const{
    _onWinText.draw();
}


void WinnerAnimator::activate(int32_t playerId){
    Defines::WHITE_PLAYER_ID == playerId ? _onWinText.setText("White pieces win the game!") : _onWinText.setText("Black pieces win the game!");
    TimerClient::startTimer(50,_blinkWinnerTextTimerId,TimerType::PULSE);
    _isActive=TimerClient::isActiveTimerId(_blinkWinnerTextTimerId);
    _onWinText.show();
}

void WinnerAnimator::deactivate(){
    if(!_isActive){
        return;
    }
    TimerClient::stopTimer(_blinkWinnerTextTimerId);
    // _isActive=TimerClient::isActiveTimerId(_blinkWinnerTextTimerId); // commented on purpose!!! 
}

void WinnerAnimator::onTimeout(int32_t timerId){
    if(timerId!=_blinkWinnerTextTimerId){
        std::cerr<<"Error, WinnerAnimator::onTimeout() received unsupported timerId: "<<timerId<<std::endl;
        return;
    }

    setRotation();
    setWinText();
}

bool WinnerAnimator::isActive() const{
    return _isActive;
}

void WinnerAnimator::restart(){
    _isActive=false;
    _piecesCurrRotAngle=0.0;
    _deltaRotAngle=1;
    _isClockwiseRotation=true;
    _isIteratorAtEnd=false;
    _yRangeIter=yRange.begin();
    _onWinText.setPosition(800,*_yRangeIter);
    _onWinText.hide();
}

void WinnerAnimator::setRotation(){

    if(10.0==_piecesCurrRotAngle){
        _deltaRotAngle=-1;
    }

    if(-10.0==_piecesCurrRotAngle){
        _deltaRotAngle=1;
    }

    _piecesCurrRotAngle+=_deltaRotAngle;
    
    _pieceHandlerProxy->rotateWinnerPieces(_deltaRotAngle);
}
void WinnerAnimator::setWinText(){
    if(yRange.end()==_yRangeIter){
        _isIteratorAtEnd=true;
        --_yRangeIter;
    }

    if(_isIteratorAtEnd){
        --_yRangeIter;
    }

    _onWinText.setPosition(Point(800,*_yRangeIter));
    // _onWinText.isVisible() ? _onWinText.hide() : _onWinText.show();

    if(!_isIteratorAtEnd){
        ++_yRangeIter;
    }

    if(yRange.begin()==_yRangeIter){
        _isIteratorAtEnd=false;
        ++_yRangeIter;
    }    
}