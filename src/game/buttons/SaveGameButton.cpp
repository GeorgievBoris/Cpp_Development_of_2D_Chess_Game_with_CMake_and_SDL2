// Corresponding header
#include "game/buttons/SaveGameButton.h"
// C system headers
#include <cstdlib>
// C++ system headers
#include <iostream>
#include <fstream>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "sdl_utils/InputEvent.h"
#include "game/proxies/PieceHandlerProxy.h"

int32_t SaveGameButton::init(int32_t rsrcId, PieceHandlerProxy* pieceHandlerProxy,const std::function<void(std::ofstream&)>& funcCallback){
    if(INVALID_RSRC_ID==rsrcId){
        std::cerr<<"Error, received invalid rsrcId!\n";
        return EXIT_FAILURE;
    }

    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, received nullptr!\n";
        return EXIT_FAILURE;
    }

    _pieceHandlerProxy=pieceHandlerProxy;

    const Point pos (1225,0);
    ButtonBase::Image::create(rsrcId,pos);
    ButtonBase::Widget::show();

    _funcCallback=funcCallback;

    return EXIT_SUCCESS;
}

void SaveGameButton::handleEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    if(ButtonBase::containsEvent(e)){
        std::ofstream outputFile("piecesStateSaved.txt",std::fstream::app);
        _pieceHandlerProxy->savePiecesState(outputFile);
        _funcCallback(outputFile);
        outputFile.close();
        ButtonBase::lockInput();
    }
}

void SaveGameButton::restart(){
    ButtonBase::unlockInput();
}