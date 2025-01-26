// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include<iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "utils/drawing/Color.h"

int32_t Game::init(const GameCfg& cfg){
    if(EXIT_SUCCESS!=_hero.init(cfg.runningGirlRsrcId,cfg.girlMoveTimerId)){
        std::cerr<<"_hero.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_wheel.init(cfg.wheelRsrcId,cfg.wheelRotAnimTimerId,cfg.wheelScaleAnimTimerId)){
        std::cerr<<"_wheel.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    const int32_t buttonRsrcIds[WHEEL_BTNS_COUNT]={cfg.startButtonRsrcId, cfg.stopButtonRsrcId};
    const Point buttonStartPos[WHEEL_BTNS_COUNT]={Point(650,100), Point(830,100)};
    for(int32_t i=0;i<WHEEL_BTNS_COUNT;++i){
        if(EXIT_SUCCESS!=_wheelBtns[i].init(this,i)){
            std::cerr<<"_wheelBtns["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
        _wheelBtns[i].create(buttonRsrcIds[i],buttonStartPos[i]);
    }
    _wheelBtns[WHEEL_STOP_BTN_IDX].lockInput();

    _blackBackgroundImg.create(cfg.blackBackgroundRsrcId); // added by Zhivko at the end of the lecture for demonstration purposes
    _blackBackgroundImg.activateAlphaModulation();
    _blackBackgroundImg.setOpacity(ZERO_OPACITY);

    return EXIT_SUCCESS;
}

void Game::deinit(){
    _blackBackgroundImg.destroy();
    _wheel.deinit();
    _hero.deinit();
}

void Game::draw(){
    _wheel.draw();
    _hero.draw();
    for(int32_t i=0;i<WHEEL_BTNS_COUNT;++i){
        _wheelBtns[i].draw();
    }

    _blackBackgroundImg.draw(); // added by Zhivko at the end of the lecture for demonstration purposes
}

void Game::handleEvent(const InputEvent& e){
    for(int32_t i=0;i<WHEEL_BTNS_COUNT;++i){
        if(_wheelBtns[i].isInputUnlocked() && _wheelBtns[i].containsEvent(e)){
            _wheelBtns[i].handleEvent(e);
            return;
        }
    }

    _hero.handleEvent(e);
    _wheel.handleEvent(e);

    if(TouchEvent::KEYBOARD_PRESS==e.type){
        if(Keyboard::KEY_LEFT_BRACKET==e.key){ // added by Zhivko at the end of the lecture for demonstration purposes
            _blackBackgroundImg.setOpacity(_blackBackgroundImg.getOpacity()-5);
        } else if(Keyboard::KEY_RIGHT_BRACKET==e.key){ // added by Zhivko at the end of the lecture for demonstration purposes
            _blackBackgroundImg.setOpacity(_blackBackgroundImg.getOpacity()+5);
        }
    }    
}

void Game::onButtonPressed(int32_t buttonId){
    switch(buttonId){
    case WHEEL_START_BTN_IDX:
        _wheelBtns[WHEEL_START_BTN_IDX].lockInput();
        _wheelBtns[WHEEL_STOP_BTN_IDX].unlockInput();
        _wheel.startAnimation();
        _hero.startMoveAnim();
        break;
    case WHEEL_STOP_BTN_IDX:
        _wheelBtns[WHEEL_START_BTN_IDX].unlockInput();
        _wheelBtns[WHEEL_STOP_BTN_IDX].lockInput();
        _wheel.stopAnimation();
        break;
    default:
        std::cerr<<"Received unsupported buttonId: "<<buttonId<<std::endl;
        break;
    }
}