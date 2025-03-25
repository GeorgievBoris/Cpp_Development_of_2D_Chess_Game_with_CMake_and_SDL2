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

    if(EXIT_SUCCESS!=_startScreen.init(cfg.startScreenCfg,[&](){_game.hide();},
                                                            [&](){_game.start();},
                                                            [&](){_game.startPlayersTimer();})){ // _startScreen object is NOT added by Zhivko
        std::cerr<<"_startScreen.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_game.init(cfg.gameCfg, [&](){_startScreen.show();})){ // the "lambda funciton" is NOT added by Zhivko
        std::cerr<<"_game.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_debugConsole.init(cfg.managerHandlerCfg.drawMgrCfg.maxFrameRate,cfg.debugConsoleFontId)){
        std::cerr<<"_debugConsole.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    gTimerMgr->onInitEnd(); // listen to this part of lecture "Buttons_and_Timers" again -> around time 2:19:00 

    return EXIT_SUCCESS;
}

void Engine::deinit(){
    // the deinitialization is performed...
    // ... in a reverse order to the initialization

    _game.deinit();
    _startScreen.deinit(); // NOT added by Zhivko
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
        const auto elapsedTime=time.getElapsed().toMicroseconds();
        if(_debugConsole.isActive()){
            _debugConsole.update(elapsedTime,gTimerMgr->getActiveTimersCount());
        }
        limitFPS(elapsedTime);
    }
}

void Engine::drawFrame(){
    gDrawMgr->clearScreen();

    _game.draw();

    _startScreen.draw(); // NOT added by Zhivko

    if(_debugConsole.isActive()){
        // _debugConsole.updateActiveWidgets(gDrawMgr->getActiveWidgets()); // originally placed here by Zhivko
        _debugConsole.draw();
        _debugConsole.updateActiveWidgets(gDrawMgr->getActiveWidgets()); // moved here by me, in order to "count" the "Active Widgets" in a more accurate way
    }

    gDrawMgr->finishFrame();
}

bool Engine::processFrame(){
    _managerHandler.process();

    while(_event.pollEvent()){
        if(_event.checkForExitRequest()){
            return true;
        }

        handleEvent();

        if(_startScreen.shouldExit()){ // this "if-statement" is NOT added by Zhivko
            return true;
        }
    }
    
    drawFrame();
    return false;
}

void Engine::handleEvent(){
    _startScreen.handleEvent(_event); // NOT added by Zhivko

    _game.handleEvent(_event); // added by Zhivko

    _debugConsole.handleEvent(_event);
}

void Engine::limitFPS(int64_t elapsedTimeMicroseconds){

    constexpr auto microsecondsInASecond=1000000;
    const auto microsecondsPerFrame=microsecondsInASecond/gDrawMgr->getMaxFrameRate();

    const int64_t sleepDurationMicroseconds=microsecondsPerFrame-elapsedTimeMicroseconds;
    if(0<sleepDurationMicroseconds){
        ThreadUtils::sleepFor(sleepDurationMicroseconds);
    }
}