// Corresponding header
#include "game/buttons/WheelButton.h"
// C system header
// C++ system header
#include <iostream>
// Third-party header
// Own header
#include "sdl_utils/InputEvent.h"
#include "game/proxies/GameProxy.h"

void WheelButton::handleEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_PRESS==e.type){
        Image::setFrame(CLICKED);
    } else if (TouchEvent::TOUCH_RELEASE==e.type) {
        Image::setFrame(UNCLICKED);
        _gameProxy->onButtonPressed(_buttonId);
    }

}

int32_t WheelButton::init(GameProxy* gameProxy, int32_t buttonId){
    if(nullptr==gameProxy){
        std::cerr<<"nullptr provided for gameProxy. ButtonId: "<<buttonId<<std::endl;
        return EXIT_FAILURE;
    }
    _gameProxy=gameProxy;
    _buttonId=buttonId;
    return EXIT_SUCCESS;
}

