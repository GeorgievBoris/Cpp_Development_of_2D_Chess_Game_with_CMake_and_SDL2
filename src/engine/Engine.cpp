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

int32_t Engine::init(const EngineCfg& cfg){


    if(EXIT_SUCCESS!=_window.init(cfg.windowCfg)){
        std::cerr<<"_window.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_renderer.init(_window.getWindow())){
        std::cerr<<"_renderer.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_imgContainer.init(cfg.imgContainerCfg)){
        std::cerr<<"_imageContainer::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_textContainer.init(cfg.textContainerCfg)){
        std::cerr<<"_textContainer.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_event.init()){
        std::cerr<<"_event.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_game.init(cfg.gameCfg,&_imgContainer,&_textContainer)){
        std::cerr<<"_game.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Engine::deinit(){
    // the deinitialization is performed...
    // ... in a reverse order to the initialization

    _game.deinit();
    _event.deinit();
    _textContainer.deinit();
    _imgContainer.deinit();
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

    std::vector<DrawParams> images;
    _game.draw(images);

    SDL_Texture* texture=nullptr;
    for(const DrawParams& image:images){
        if(WidgetType::IMAGE==image.widgetType){
            texture=_imgContainer.getImageTexture(image.rsrcId);
        } else if (WidgetType::TEXT==image.widgetType){
            texture=_textContainer.getTextTexture(image.textId);
        } else {
            std::cerr<<"Error, received unsupported WidgetType: "
            <<static_cast<int32_t>(image.widgetType)<<" for rsrcId: "
            <<image.rsrcId<<std::endl;
            continue;
        }
        _renderer.renderTexture(texture,image);
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