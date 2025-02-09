// Corresponding header
#include "manager_utils/debug/DebugConsole.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/Color.h"
#include "sdl_utils/InputEvent.h"


int32_t DebugConsole::init(int64_t maxFrames, int32_t fontId){
    _maxFrames=maxFrames;

    Point textPos=Point(0,0);
    _debugTexts[FPS_TEXT_IDX].create("0",fontId,Colors::YELLOW,textPos);

    textPos.y+=40;
    _debugTexts[ACTIVE_TIMER_IDX].create("0",fontId,Colors::YELLOW,textPos);

    return EXIT_SUCCESS;
}

void DebugConsole::handleEvent(const InputEvent& e){
    if(TouchEvent::KEYBOARD_PRESS!=e.type){
        return;
    }

    if(Keyboard::KEY_TILDA==e.key){
        _isActive=!_isActive;
    }
}

void DebugConsole::update(int64_t elapsedMicroseconds, size_t activeTimers){
    constexpr auto microsecondsInASecond=1000000;
    auto currFps=microsecondsInASecond/elapsedMicroseconds;
    if(_maxFrames<currFps){
        currFps=_maxFrames;
    }
    std::string textContent="FPS: ";
    textContent.append(std::to_string(currFps));
    _debugTexts[FPS_TEXT_IDX].setText(textContent);

    textContent.clear();
    textContent="Active Timers: ";
    textContent.append(std::to_string(activeTimers));

    _debugTexts[ACTIVE_TIMER_IDX].setText(textContent);
}

void DebugConsole::draw() const{
    for(int32_t i=0;i<DEBUG_TEXTS_COUNT;++i){
        _debugTexts[i].draw();
    }
}

bool DebugConsole::isActive() const{
    return _isActive;
}