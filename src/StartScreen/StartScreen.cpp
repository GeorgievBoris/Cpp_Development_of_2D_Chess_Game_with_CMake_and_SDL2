// Corresponding header
#include "StartScreen/StartScreen.h"
// C system headers
// C++ system headers
#include <iostream>
#include <array>
// Third-party headers
// Own headers
#include "StartScreen/config/StartScreenCfg.h"
#include "StartScreen/buttons/config/StartScreenButtonCfg.h"

namespace{
     constexpr int32_t DELTA_OPACITY_INCREASE=5;
     constexpr int32_t DELTA_OPACITY_DECREASE=-5;
}

int32_t StartScreen::init(const StartScreenCfg& cfg, const std::function<void()>& hideGameCallBack,
                                                     const std::function<void()>& startGameCallBack,
                                                     const std::function<void()>& startPlayersTimerCallBack){

    StartScreenButtonCfg btnCfg;

    constexpr int32_t btnsOffsetDistance=100;

    const int32_t startX=(cfg.gameBoardWidth - ((BUTTONS_COUNT*cfg.btnWidth)+(BUTTONS_COUNT-1)*btnsOffsetDistance))/2;

    btnCfg.btnWidth=cfg.btnWidth;
    btnCfg.btnHeight=cfg.btnHeight;
    btnCfg.startScreenBtnsRsrcId=cfg.buttonsRsrcId;
    btnCfg.btnPos.y=(cfg.gameBoardHeight-cfg.btnHeight)/2;

    
    constexpr std::array<StartScreenButtonsType,BUTTONS_COUNT>btnsType{StartScreenButtonsType::EXIT,StartScreenButtonsType::START_CHESS};

    for(int32_t i=0;i<BUTTONS_COUNT;++i){
        btnCfg.btnType=btnsType[i];
        btnCfg.btnPos.x=startX+i*(btnsOffsetDistance+btnCfg.btnWidth);

        StartScreenButtonsType::EXIT==btnCfg.btnType ? btnCfg.btnCallBack=[&](){setShouldExit();} : 
        btnCfg.btnCallBack=[&](){hide();};

        if(EXIT_SUCCESS!=_screenBtns[i].init(btnCfg)){
            std::cerr<<"_screenBtns["<<i<<"].init() failed"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    _backgroundRsrcId=cfg.backgroundRsrcId;
    _timerId=cfg.timerId;

    _hideGameCallBack=hideGameCallBack;
    _startGameCallBack=startGameCallBack;
    _startPlayersTimerCallBack=startPlayersTimerCallBack;
    _deltaOpacity=DELTA_OPACITY_DECREASE;

    StartScreen::activate();

    return EXIT_SUCCESS;
}

void StartScreen::deinit(){
    _shouldExit=true;
}

void StartScreen::draw() const{
    if(!_background.isCreated()){
        return;
    }

    _background.draw();
    for(const StartScreenButton& btn:_screenBtns){
        btn.draw();
    }
}

void StartScreen::handleEvent(const InputEvent& e){

    if(_maxTotalOpacity!=_currTotalOpacity){
        return;
    }


    for(StartScreenButton& btn:_screenBtns){
        if(btn.isInputUnlocked() && btn.containsEvent(e)){
            btn.handleEvent(e);
        }
    }
}

void StartScreen::activate(){
    for(StartScreenButton& btn:_screenBtns){
        btn.activate();
    }
    _background.create(_backgroundRsrcId);
    _background.activateAlphaModulation();
}

void StartScreen::deactivate(){
    for(StartScreenButton& btn:_screenBtns){
        btn.deactivate();
    }
    _background.deactivateAlphaModulation();
    _background.destroy();
}

bool StartScreen::shouldExit()const{
    return _shouldExit;
}

void StartScreen::setShouldExit(bool shouldExit){
    _shouldExit=shouldExit;
}

void StartScreen::changeOpacity(int32_t opacity){
    const int32_t backgroundOpacity=_background.getOpacity();
    _currTotalOpacity=backgroundOpacity;

    if(0>opacity){
        for(size_t i=0;i<BUTTONS_COUNT;++i){
            _currTotalOpacity+=_screenBtns[i].getOpacity();
        }

        if(FULL_OPACITY<_currTotalOpacity){
            for(size_t i=0;i<BUTTONS_COUNT;++i){
                _screenBtns[i].setOpacity(_screenBtns[i].getOpacity()+opacity);
                _currTotalOpacity+=opacity;
            }
            return;
        }

        _currTotalOpacity+=opacity;
        _background.setOpacity(_currTotalOpacity);
        return;
    }

    
    if(FULL_OPACITY!=backgroundOpacity){
        _currTotalOpacity+=opacity;
        _background.setOpacity(_currTotalOpacity);
        return;
    }

    for(size_t i=0;i<BUTTONS_COUNT;++i){
        const int32_t btnOpacity=_screenBtns[i].getOpacity();
        _screenBtns[i].setOpacity(btnOpacity+opacity);
        _currTotalOpacity+=btnOpacity+opacity;
    }
}

void StartScreen::onTimeout(int32_t timerId){
    if(timerId!=_timerId){
        std::cerr<<"Error, received unsupported timerId: "<<timerId<<std::endl;
        return;
    }

    changeOpacity(_deltaOpacity);

    if(DELTA_OPACITY_DECREASE==_deltaOpacity){

        if(FULL_OPACITY==_currTotalOpacity){
            _startGameCallBack();
        }

        if(ZERO_OPACITY==_currTotalOpacity){
            TimerClient::stopTimer(timerId);
            deactivate();
            _startPlayersTimerCallBack();            
        }
        return;
    }

    if(FULL_OPACITY==_currTotalOpacity){
        _hideGameCallBack();
    }


    if(_maxTotalOpacity==_currTotalOpacity){
        TimerClient::stopTimer(timerId);
    }
}

void StartScreen::show(){
    TimerClient::startTimer(50,_timerId,TimerType::PULSE);
    _deltaOpacity=DELTA_OPACITY_INCREASE;
    activate();

    while(true){
        changeOpacity(DELTA_OPACITY_DECREASE);
        if(ZERO_OPACITY==_currTotalOpacity){
            break;
        }
        
    }
}

void StartScreen::hide(){
    TimerClient::startTimer(50,_timerId,TimerType::PULSE);
    _deltaOpacity=DELTA_OPACITY_DECREASE;
}