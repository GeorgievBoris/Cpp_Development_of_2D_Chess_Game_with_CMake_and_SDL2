// Corresponding header
#include "game/buttons/QuitGameButton.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "manager_utils/drawing/Fbo.h"


int32_t QuitGameButton::init(int32_t rsrcId, const std::function<void()>& showStartScreenCallBack,
                                             const std::function<void()>& gameLogicAndWinnerAnimatorCallBack){
    if(INVALID_RSRC_ID==rsrcId){
        std::cerr<<"QuitGameButton::init() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    // const Point pos = Point::ZERO;
    const Point pos(1050,0);
    Image::create(rsrcId,pos);
    Widget::show();
    _showStartScreenCallBack=showStartScreenCallBack;
    _gameLogicAndWinnerAnimatorCallBack=gameLogicAndWinnerAnimatorCallBack;

    return EXIT_SUCCESS;
}

void QuitGameButton::handleEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    if(ButtonBase::containsEvent(e)){
        _showStartScreenCallBack();
        _gameLogicAndWinnerAnimatorCallBack();
        ButtonBase::lockInput();
    }
}

void QuitGameButton::restart(){
    ButtonBase::unlockInput();
}

// void QuitGameButton::drawOnFbo(Fbo& fbo) const{
//     fbo.addWidget(static_cast<Widget>(*this));
// }
