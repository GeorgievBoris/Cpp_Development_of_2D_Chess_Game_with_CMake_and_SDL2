// Corresponding header
#include "game/board/GameBoardAnimator.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/proxies/GameProxy.h"
#include "manager_utils/drawing/Fbo.h"


// NOTE: write this down in the book: 
// In C and C++ we can but we MUST NOT DIRECTLY compare floating point numbers...
// ...this comes from the specifics of the standard for floating point numbers !!!

namespace{
constexpr auto ROT_ANIM_STEP_DEGREES=10;
}

GameBoardAnimator::~GameBoardAnimator(){
    // advice from Zhivko to use the dtor of the base class in order to stop all timers
    // this is a smart way of stopping all active timers...
    // ... compared to using the dtor of the class that inherits TimerClient    
    if(isActiveTimerId(_gameFboRotTimerId)){
        stopTimer(_gameFboRotTimerId);
    }
}

int32_t GameBoardAnimator::init(GameProxy* gameProxy, Fbo* gameFbo, int32_t gameFboRotTimerId){
    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
        return EXIT_FAILURE;
    }
    _gameProxy=gameProxy;

    if(nullptr==gameFbo){
        std::cerr<<"Error, nullptr provided for gameFbo"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameFbo=gameFbo;
    _gameFboRotTimerId=gameFboRotTimerId;

    const Point rotCenter(_gameFbo->getWidth()/2,_gameFbo->getHeight()/2);
    _gameFbo->setRotationCenter(rotCenter);

    return EXIT_SUCCESS;
}

void GameBoardAnimator::startAnim(int32_t playerId){
    if(Defines::WHITE_PLAYER_ID==playerId){
        _targetFlipType=WidgetFlip::HORIZONTAL_AND_VERTICAL;
        _targetRotation=FULL_ROTATION/2;
    } else {
        _targetFlipType=WidgetFlip::NONE;
        _targetRotation=FULL_ROTATION;
    }
    startTimer(20,_gameFboRotTimerId,TimerType::PULSE);
    _isActive=TimerClient::isActiveTimerId(_gameFboRotTimerId); // NOT added by Zhivko
    _gameProxy->setWidgetFlip(_targetFlipType);
}

bool GameBoardAnimator::isActive() const{
    // return isActiveTimerId(_gameFboRotTimerId); // original code used by Zhivko
    return _isActive; // NOT used by Zhivko
}

// GameBoardAnimator::restart() method is added by me
void GameBoardAnimator::restart(){
    _targetFlipType=WidgetFlip::NONE;
    _targetRotation=0;
    _currRotation=0;
    _gameFbo->setRotationAngle(_currRotation);
    _isActive=false;
    // about " _gameProxy->setWidgetFlip(_targetFlipType) "
    // perhaps no point to call this method , because when ::restart() is performed either 1) new chessPieces are created...
    // ... or 2) the "old" chessPieces have not changed at all

    // _gameProxy->setWidgetFlip(_targetFlipType); 
}

int32_t GameBoardAnimator::getTargetRotation()const{ // // this method is NOT added by Zhivko !
    return _targetRotation;
}

void GameBoardAnimator::onTimeout(int32_t timerId){
    if(timerId==_gameFboRotTimerId){
        processRotAnim();
    } else {
        std::cerr<<"Received unsupported TimerId: "<<timerId<<std::endl;
    }
}

void GameBoardAnimator::processRotAnim(){
    _currRotation+=ROT_ANIM_STEP_DEGREES;
    _gameFbo->setRotationAngle(_currRotation);

    if(_targetRotation==_currRotation){
        if(FULL_ROTATION==_targetRotation){
            _currRotation=0;
        }
        stopTimer(_gameFboRotTimerId);
        _isActive=TimerClient::isActiveTimerId(_gameFboRotTimerId); // NOT added by Zhivko
        _gameProxy->onBoardAnimFinished();
    }
}