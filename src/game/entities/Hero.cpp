// Corresponding header
#include "game/entities/Hero.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

int32_t Hero::init(int32_t heroRsrcId){

    _heroImg.create(heroRsrcId);
    return EXIT_SUCCESS;
}

void Hero::deinit(){
    _heroImg.destroy();
}

void Hero::draw(){
    _heroImg.draw();
}

void Hero::handleEvent(const InputEvent& e){
    if(TouchEvent::KEYBOARD_PRESS!=e.type){
        return;
    }

    switch(e.key){
    case Keyboard::KEY_RIGHT:
        _heroImg.setFlipType(WidgetFlip::NONE);
        _heroImg.setNextFrame();
        _heroImg.moveRight(8);
        break;
    case Keyboard::KEY_LEFT:
        _heroImg.setFlipType(WidgetFlip::HORIZONTAL);
        // _heroImg.setPrevFrame();
        _heroImg.setNextFrame();
        _heroImg.moveLeft(8);
        break;
    case Keyboard::KEY_DOWN:
        _heroImg.setNextFrame();
        _heroImg.moveDown(8);
        break;
    case Keyboard::KEY_UP:
        _heroImg.setNextFrame();
        _heroImg.moveUp(8);
        break;
    default:
        break;
    }
}