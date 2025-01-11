// Corresponding header
#include "MonitorWindow.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_video.h>
// Own headers
#include "utils/drawing/Point.h"


int32_t MonitorWindow::init(const MonitorWindowCfg& cfg){

    Point finalPos;
    if(Point::UNDEFINED==cfg.windowPos){
        finalPos.x=SDL_WINDOWPOS_UNDEFINED;
        finalPos.y=SDL_WINDOWPOS_UNDEFINED;
    } else {
        finalPos=cfg.windowPos;
    }
    

    _window=SDL_CreateWindow(cfg.windowName.c_str(),finalPos.x,finalPos.y,
                                cfg.windowWidth,cfg.windowHeight,cfg.windowFlags);

    if(nullptr==_window){
        std::cerr<<"SDL_CreateWindow() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

MonitorWindow::~MonitorWindow(){
    deinit();
}

void MonitorWindow::deinit(){
    if(nullptr!=_window){
        SDL_DestroyWindow(_window);
        _window=nullptr;
    }
}

int32_t MonitorWindow::updateWindowSurface(){
    if(EXIT_SUCCESS!=SDL_UpdateWindowSurface(_window)){
        std::cerr<<"SDL_UpdateWindowSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

SDL_Window* MonitorWindow::getWindow() const {
    return _window;
}




