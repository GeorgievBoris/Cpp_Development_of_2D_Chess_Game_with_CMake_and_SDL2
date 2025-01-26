// Corresponding header
#include "engine/Engine.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "engine/config/EngineCfg.h"
#include "utils/thread/ThreadUtils.h"
#include "utils/time/Time.h"
#include "sdl_utils/Texture.h"
#include "utils/drawing/Color.h"
#include "manager_utils/managers/DrawMgr.h"
#include "manager_utils/managers/TimerMgr.h"

int32_t Engine::init(const EngineCfg& cfg){

    if(EXIT_SUCCESS!=_managerHandler.init(cfg.managerHandlerCfg)){
        std::cerr<<"_managerHandler.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_event.init()){
        std::cerr<<"_event.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_game.init(cfg.gameCfg)){
        std::cerr<<"_game.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    gTimerMgr->onInitEnd(); // listed to this part of lecture "Buttons_and_Timers" again -> around time 2:19:00 

    return EXIT_SUCCESS;
}

void Engine::deinit(){
    // the deinitialization is performed...
    // ... in a reverse order to the initialization

    _game.deinit();
    _event.deinit();
    _managerHandler.deinit();
}

void Engine::start(){
    mainLoop();
}

void Engine::mainLoop(){
    Time time;

    while(true){
        time.getElapsed(); // this line is needed, in order to reset the value of the "_now" variable and "kill" the time needed for the last sleep state.
        const bool shouldExit=processFrame();
        if(shouldExit){
            break;
        }
        const int64_t timeElapsedMicroseconds=time.getElapsed().toMicroseconds();
        limitFPS(timeElapsedMicroseconds);
    }
}

void Engine::drawFrame(){
    gDrawMgr->clearScreen();

    _game.draw();

    gDrawMgr->finishFrame();
}

bool Engine::processFrame(){
    _managerHandler.process();

    while(_event.pollEvent()){
        if(_event.checkForExitRequest()){
            return true;
        }
        handleEvent();
    }

    drawFrame();

    return false;
}

void Engine::handleEvent(){
    _game.handleEvent(_event);
}

void Engine::limitFPS(int64_t elapsedTimeMicroseconds){

    constexpr auto maxFramesPerSecond=30;
    constexpr auto microsecondsInASecond=1000000;
    constexpr auto microsecondsPerFrame=microsecondsInASecond/maxFramesPerSecond;

    const int64_t sleepDurationMicroseconds=microsecondsPerFrame-elapsedTimeMicroseconds;
    if(0<sleepDurationMicroseconds){
        ThreadUtils::sleepFor(sleepDurationMicroseconds);
    }
}