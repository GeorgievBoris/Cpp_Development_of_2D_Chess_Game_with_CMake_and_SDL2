// Corresponding header
#include "Engine.h"
// C system headers
// C++ system headers
#include <iostream>
// #include <array>
// Third-party headers
#include <SDL_surface.h>
// Own headers
#include "sdl_utils/Texture.h"
#include "utils/thread/ThreadUtils.h"
#include "utils/time/Time.h"

int32_t Engine::init(){

    MonitorWindowCfg cfg;
    cfg.windowName="SDL_Runtime";
    cfg.windowHeight=480;
    cfg.windowWidth=640;
    cfg.windowFlags=WINDOW_SHOWN;

    if(EXIT_SUCCESS!=_window.init(cfg)){
        std::cerr<<"_window.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=loadResources()){
        std::cerr<<"Engine::loadResources() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_event.init()){
        std::cerr<<"_event.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _screenSurface=_window.getWindowSurface();
    if(nullptr==_screenSurface){
        std::cerr<<"_window.getWindowSurface() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _currChosenImage=_imageSurfaces[ALL_KEYS];

    return EXIT_SUCCESS;
}

void Engine::deinit(){
    // the deinitialization is performed...
    // ... in a reverse order to the initialization

    Texture::freeSurface(_screenSurface); // not mandatory !!!
    _event.deinit();
    for(int32_t i=0;i<COUNT;++i){
        Texture::freeSurface(_imageSurfaces[i]);
    }
    _window.deinit();
}

void Engine::start(){
    mainLoop();
}

int32_t Engine::loadResources(){
    const char* filePaths [COUNT];
    // the system path is given relative to the "build" directory
    filePaths[UP]="../resources/up.png";
    filePaths[DOWN]="../resources/down.png";
    filePaths[LEFT]="../resources/left.png";
    filePaths[RIGHT]="../resources/right.png";
    filePaths[ALL_KEYS]="../resources/press_keys.png";

    // // Alternative 
    // const std::array<std::string,COUNT> filePathss = {
    //     "../resources/up.png",
    //     "../resources/down.png",
    //     "../resources/left.png",
    //     "../resources/right.png",
    //     "../resources/press_keys.png"
    // };

    // for (const auto& file:filePathss){
        // // etc...etc...etc..
    // }

    for(int32_t i=0;i<COUNT;++i){
        if(EXIT_SUCCESS!=Texture::createSurfaceFromFile(filePaths[i],_imageSurfaces[i])){
            std::cerr<<"Texture::createSurfaceFromFile() failed for file: "<<filePaths[i]<<std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
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
    if(EXIT_SUCCESS!=SDL_BlitSurface(_currChosenImage,nullptr,_screenSurface,nullptr)){
        std::cerr<<"SDL_BlitSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return;
    }

    if(EXIT_SUCCESS!=_window.updateWindowSurface()){
        std::cerr<<"_window.updateWindowSurface() failed."<<std::endl;
        return;
    }
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

    // if(TouchEvent::KEYBOARD_RELEASE==_event.type){
    //     _currChosenImage=_imageSurfaces[ALL_KEYS];
    //     return;
    // }
    
    if(TouchEvent::KEYBOARD_PRESS!=_event.type){
        TouchEvent::KEYBOARD_RELEASE==_event.type ? _currChosenImage=_imageSurfaces[ALL_KEYS] : (void*)_currChosenImage;
        return;
    }

    switch(_event.key){
        case Keyboard::KEY_DOWN:
            _currChosenImage=_imageSurfaces[DOWN];
            break;
        case Keyboard::KEY_UP:
            _currChosenImage=_imageSurfaces[UP];
            break;
        case Keyboard::KEY_LEFT:
            _currChosenImage=_imageSurfaces[LEFT];
            break;
        case Keyboard::KEY_RIGHT:
            _currChosenImage=_imageSurfaces[RIGHT];
            break;
        default:
            // (void*)_currChosenImage;
            break;
    }
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