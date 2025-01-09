// Corresponding header
#include "Engine.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_render.h>
// Own headers
#include "engine/config/EngineCfg.h"
#include "utils/thread/ThreadUtils.h"
#include "utils/time/Time.h"

int32_t Engine::init(const EngineCfg& cfg){


    if(EXIT_SUCCESS!=_window.init(cfg.windowCfg)){
        std::cerr<<"_window.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_renderer.init(_window.getWindow())){
        std::cerr<<"_renderer.init() failed"<<std::endl;
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

    return EXIT_SUCCESS;
}

void Engine::deinit(){
    // the deinitialization is performed...
    // ... in a reverse order to the initialization

    // Texture::freeSurface(_screenSurface); // not 100% mandatory !!!
    _game.deinit();
    _event.deinit();
    _renderer.deinit();
    _window.deinit();
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

    _renderer.clearScreen();

    std::vector<SDL_Texture*> images;
    _game.draw(images);

    for(const auto& image:images){
        _renderer.renderTexture(image);
    }

    _renderer.finishFrame();
}

bool Engine::processFrame(){

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