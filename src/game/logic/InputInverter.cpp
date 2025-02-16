// Corresponding header
#include "game/logic/InputInverter.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

int32_t InputInverter::init(int32_t boardWidth, int32_t boardHeight){
    _boardWidth=boardWidth;
    _boardHeight=boardHeight;
    return EXIT_SUCCESS;
}

void InputInverter::setBoardFlipType(WidgetFlip flipType){
    _flipType=flipType;
}

void InputInverter::invertEvent(InputEvent& inputEvent){

    // NOTE: "_flipType" is an enum class
    // the basic idea behind the enum classes is to use them in IF and SWITCH statements...
    // ... beucase they are infinitely readable...AND are also heavily typed fixed (i.e. tipizirani na Bulgarski)

    switch(_flipType){
    case WidgetFlip::NONE:
        break;
    case WidgetFlip::HORIZONTAL:
        inputEvent.pos.y=_boardHeight-inputEvent.pos.y;
        break;
    case WidgetFlip::VERTICAL:
        inputEvent.pos.x=_boardWidth-inputEvent.pos.x;
        break;
    case WidgetFlip::HORIZONTAL_AND_VERTICAL:
        inputEvent.pos.x=_boardWidth-inputEvent.pos.x;
        inputEvent.pos.y=_boardHeight-inputEvent.pos.y;
        break;
    }
}