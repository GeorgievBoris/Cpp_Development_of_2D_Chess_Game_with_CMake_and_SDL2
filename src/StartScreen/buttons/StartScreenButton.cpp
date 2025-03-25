// Corresponding header
#include "StartScreen/buttons/StartScreenButton.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

int32_t StartScreenButton::init(const StartScreenButtonCfg& cfg){

    if(StartScreenButtonsType::UNKNOWN==cfg.btnType){
        std::cerr<<"Error, received unsupported StartScreenButtonsType: "<<
            static_cast<int32_t>(cfg.btnType)<<" for rsrcId: "<<cfg.startScreenBtnsRsrcId <<std::endl;
        return EXIT_FAILURE;
    }
    _cfg=cfg;
    return EXIT_SUCCESS;
}

void StartScreenButton::deinit(){

}

void StartScreenButton::handleEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }
    _cfg.btnCallBack(); // depending on the callback - the whole applicaiton gets closed OR the chess game appears and starts
}

void StartScreenButton::draw() const{
    ButtonBase::draw();
}

void StartScreenButton::activate(){
    StartScreenButton::deactivate();

    ButtonBase::create(_cfg.startScreenBtnsRsrcId,_cfg.btnPos);
    ButtonBase::setFrame(static_cast<int32_t>(_cfg.btnType));
    ButtonBase::activateAlphaModulation();
}

void StartScreenButton::deactivate(){
    if(ButtonBase::isCreated()){
        ButtonBase::deactivateAlphaModulation();
        ButtonBase::destroy();
    }
}

