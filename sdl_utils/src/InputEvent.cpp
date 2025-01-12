// Corresponding header
#include "sdl_utils/InputEvent.h"
// C system headers
// C++ system headers
#include <cstring>
#include <iostream>
// Third-party headers
#include <SDL_events.h>
// Own headers


int32_t InputEvent::init(){
    // since SDL_Event is a "C-language" structure, a good practice is to assign initial values to it
    
    // // assign initial values via the zero initializer
    // _sdlEvent=new SDL_Event{}; 

    // assign initial values via a function 
    _sdlEvent=new SDL_Event();
    
    if(nullptr==_sdlEvent){ // NOTE: In theory "new" can fail, so always do this check !!!
        std::cerr<<"Bad allocation for SDL_Event()"<<std::endl;
        return EXIT_FAILURE;
    }

    // assign initial values via a function (do it after we know that "new" has not failed !!! ) 
    memset(_sdlEvent,0,sizeof(SDL_Event)); // #include <cstring>

    return EXIT_SUCCESS;
}

void InputEvent::deinit(){
    if(nullptr!=_sdlEvent){ // always good to do sanity checking, becayse deleting a nullptr is not cool !!!
        delete _sdlEvent;
        _sdlEvent=nullptr;
    }
}

bool InputEvent::pollEvent(){

    if(0==SDL_PollEvent(_sdlEvent)){
        // no events currently
        return false;
    }

    SDL_GetMouseState(&pos.x,&pos.y); // obtain what is the position of the mouse
    setEventTypeInternal();

    return true;
}

void InputEvent::setEventTypeInternal(){
    switch(_sdlEvent->type){
        case EventType::KEYBOARD_PRESS:
            key=_sdlEvent->key.keysym.sym;
            mouseButton=Mouse::UNKNOWN;
            type=TouchEvent::KEYBOARD_PRESS;
            break;
        case EventType::KEYBOARD_RELEASE:
            key=_sdlEvent->key.keysym.sym;
            mouseButton=Mouse::UNKNOWN;
            type=TouchEvent::KEYBOARD_RELEASE;
            break;
        case EventType::MOUSE_PRESS:
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=_sdlEvent->button.button;
            type=TouchEvent::TOUCH_PRESS;
            break;
        case EventType::FINGER_PRESS:
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=Mouse::UNKNOWN;;
            type=TouchEvent::TOUCH_PRESS;
            break;
        case EventType::MOUSE_RELEASE:
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=_sdlEvent->button.button;
            type=TouchEvent::TOUCH_RELEASE;
            break;
        case EventType::FINGER_RELEASE:
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=Mouse::UNKNOWN;
            type=TouchEvent::TOUCH_RELEASE;
            break;
        case EventType::QUIT:
            // X is pressed on the window (or CTRL + C signal is sent)
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=Mouse::UNKNOWN;
            type=TouchEvent::UNKNOWN;
            break;
        default:
            key=Keyboard::KEY_UNKNOWN;
            mouseButton=Mouse::UNKNOWN;
            type=TouchEvent::UNKNOWN;
            break;
    }
}

bool InputEvent::checkForExitRequest() const{
    return (EventType::QUIT ==_sdlEvent->type) || (TouchEvent::KEYBOARD_PRESS==type && Keyboard::KEY_ESCAPE==key);
}
