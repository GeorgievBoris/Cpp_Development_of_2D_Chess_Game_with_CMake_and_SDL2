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
    if(EXIT_SUCCESS!=_hero.init(cfg.runningGirlRsrcId)){
        std::cerr<<"_hero.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_wheel.init(cfg.wheelRsrcId)){
        std::cerr<<"_wheel.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _blackBackgroundImg.create(cfg.blackBackgroundRsrcId); // added by Zhivko at the end of the lecture for demonstration purposes
    _blackBackgroundImg.activateAlphaModulation();
    _blackBackgroundImg.setOpacity(ZERO_OPACITY);

    _mousePosText.create("_",cfg.textFontId,Colors::RED);
    _mousePosText.hide();

    return EXIT_SUCCESS;
}

void Game::deinit(){
    _mousePosText.destroy();
    _blackBackgroundImg.destroy();
    _wheel.deinit();
    _hero.deinit();
}

void Game::draw(){
    _wheel.draw();
    _hero.draw();
    _mousePosText.draw();
    _blackBackgroundImg.draw(); // added by Zhivko at the end of the lecture for demonstration purposes
}

void Game::handleEvent(const InputEvent& e){
    _hero.handleEvent(e);
    _wheel.handleEvent(e);

    if(TouchEvent::KEYBOARD_PRESS==e.type){
        if(Keyboard::KEY_A==e.key){
            _mousePosText.rotateRight(30);
        } else if(Keyboard::KEY_K==e.key){ // this if statement is added for testing purposes by me, NOT by Zhivko !!!
            _mousePosText.setRotationCenter(Point::ZERO);
        } else if(Keyboard::KEY_L==e.key){ // this if statement is added for testing purposes by me, NOT by Zhivko !!!
            Point rotCenter(_mousePosText.getWidth()/2,_mousePosText.getHeight()/2);
            _mousePosText.setRotationCenter(rotCenter);
        } else if(Keyboard::KEY_LEFT_BRACKET==e.key){ // added by Zhivko at the end of the lecture for demonstration purposes
            _blackBackgroundImg.setOpacity(_blackBackgroundImg.getOpacity()-5);
        } else if(Keyboard::KEY_RIGHT_BRACKET==e.key){ // added by Zhivko at the end of the lecture for demonstration purposes
            _blackBackgroundImg.setOpacity(_blackBackgroundImg.getOpacity()+5);
        }
    }    

    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    setMousePosText(e.pos);
}

void Game::setMousePosText(const Point& mousePos){
    _mousePosText.show();
    _mousePosText.setPosition(mousePos);
    std::string textContent="X: ";
    textContent.append(std::to_string(mousePos.x))
                .append(", Y: ")
                .append(std::to_string(mousePos.y));
    _mousePosText.setText(textContent);
}