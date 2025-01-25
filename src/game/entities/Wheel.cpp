// Corresponding header
#include "game/entities/Wheel.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

int32_t Wheel::init(int32_t wheelRsrcId){
    _wheelImg.create(wheelRsrcId);
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