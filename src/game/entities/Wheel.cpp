// Corresponding header
#include "game/entities/Wheel.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

Wheel::~Wheel(){
    if(isActiveTimerId(_rotAnimTimerId)){
        stopTimer(_rotAnimTimerId);
        stopTimer(_scaleAnimTimerId);
    }
}

int32_t Wheel::init(int32_t wheelRsrcId, int32_t rotAnimTimerId, int32_t scaleAnimTimerId){
    _wheelImg.create(wheelRsrcId);
    Point rotCenter(_wheelImg.getWidth()/2,_wheelImg.getHeight()/2);
    _wheelImg.setRotationCenter(rotCenter);

    _rotAnimTimerId=rotAnimTimerId;
    _scaleAnimTimerId=scaleAnimTimerId;
    return EXIT_SUCCESS;
}

void Wheel::deinit(){

}

void Wheel::draw(){
    _wheelImg.draw();
}

void Wheel::handleEvent(const InputEvent& e){
    if(TouchEvent::KEYBOARD_PRESS!=e.type){
        return;
    }

    switch(e.key){
    case Keyboard::KEY_P:
        _wheelImg.rotateRight(20);
        break;
    case Keyboard::KEY_O:
        _wheelImg.rotateLeft(20);
        break;
    case Keyboard::KEY_K:
        _wheelImg.setRotationCenter(Point::ZERO);
        break;
    case Keyboard::KEY_L: {
        Point rotCenter(_wheelImg.getWidth()/2,_wheelImg.getHeight()/2);
        _wheelImg.setRotationCenter(rotCenter);
        break;
    }
    default:
        break;
    }
}

void Wheel::startAnimation(){
    if(_isAnimActive){
        std::cerr<<"Wheel animation is already active"<<std::endl;
        return;
    }
    startTimer(50,_rotAnimTimerId,TimerType::PULSE);
    startTimer(100,_scaleAnimTimerId,TimerType::PULSE);
    _isAnimActive=true;
    std::cerr<<"Wheel animation started"<<std::endl;
}

void Wheel::stopAnimation(){
    if(_isAnimActive){
        _isAnimActive=false;
        stopTimer(_rotAnimTimerId);
        stopTimer(_scaleAnimTimerId);
        std::cerr<<"Wheel animation stopped"<<std::endl;
        return;
    }
    std::cerr<<"Wheel animation is not active in the first place."<<std::endl;
}

void Wheel::onTimeout(int32_t timerId){
    if(timerId==_rotAnimTimerId){
        processRotAnim();
    } else if(timerId==_scaleAnimTimerId){
        processScaleAnim();
    } else {
        std::cerr<<"Received unsupported timerId: "<<timerId<<std::endl;
    }
}

void Wheel::processRotAnim(){
    _wheelImg.rotateRight(2);
}

void Wheel::processScaleAnim(){
    --_scaleSteps;

    if(0==_scaleSteps){
        _isShrinking=!_isShrinking;
        _scaleSteps=50;
    }

    if(_isShrinking){
        _wheelImg.setWidth(_wheelImg.getWidth()-5);
        _wheelImg.setHeight(_wheelImg.getHeight()-5);
    } else {
        _wheelImg.setWidth(_wheelImg.getWidth()+5);
        _wheelImg.setHeight(_wheelImg.getHeight()+5);
    }

    const Point currCenter(_wheelImg.getWidth()/2,_wheelImg.getHeight()/2); // added by me (NOT by Zhivko) after the lecture
    _wheelImg.setRotationCenter(currCenter); // added by me (NOT by Zhivko) after the lecture
}