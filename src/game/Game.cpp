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

    _layer2Img.create(cfg.layer2RsrcId);
    _pressKeysImg.create(cfg.pressKeysRsrcId);
    _pressKeysImg.activateAlphaModulation();

    _helloText.create("Hello, C++ dudes",cfg.textFontId,Colors::GREEN);
    _pressText.create("Press M to hide",cfg.textFontId,Colors::GREEN,Point(100,100));
    _hideText.create("Press N to show",cfg.textFontId,Colors::PURPLE,Point(200,200));
    _hideText.hide();

    _mousePosText.create("_",cfg.textFontId,Colors::RED);
    _mousePosText.hide();

    return EXIT_SUCCESS;
}

void Game::deinit(){

}

void Game::draw(){
    // _pressKeysImg.draw();
    // _layer2Img.draw();
    // _helloText.draw();

    // _hideText.draw();
    // _pressText.draw();
    _mousePosText.draw();
}

void Game::handleEvent(const InputEvent& e){

    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    setMousePosText(e.pos);
}
    

    // if(TouchEvent::KEYBOARD_RELEASE!=e.type){
    //     return;
    // }

    // switch(e.key){
    //     case Keyboard::KEY_UP:
    //         _pressKeysImg.moveUp(10);
    //         break;
    //     case Keyboard::KEY_DOWN:
    //         _pressKeysImg.moveDown(10);
    //         break;
    //     case Keyboard::KEY_LEFT:
    //         _pressKeysImg.moveLeft(10);
    //         break;
    //     case Keyboard::KEY_RIGHT:
    //         _pressKeysImg.moveRight(10);
    //         break;
    //     case Keyboard::KEY_Q:
    //         _pressKeysImg.setWidth(_pressKeysImg.getWidth()-10);
    //         break;
    //     case Keyboard::KEY_W:
    //         _pressKeysImg.setWidth(_pressKeysImg.getWidth()+10);
    //         break;
    //     case Keyboard::KEY_E:
    //         _pressKeysImg.setHeight(_pressKeysImg.getHeight()-10);
    //         break;
    //     case Keyboard::KEY_R:
    //         _pressKeysImg.setHeight(_pressKeysImg.getHeight()+10);
    //         break;
    //     case Keyboard::KEY_T:
    //         _pressKeysImg.setOpacity(_pressKeysImg.getOpacity()-10);
    //         break;
    //     case Keyboard::KEY_Y:
    //         _pressKeysImg.setOpacity(_pressKeysImg.getOpacity()+10);
    //         break;
    //     case Keyboard::KEY_B:
    //         _helloText.setText("Kak ste, kolegi?");
    //         break;
    //     case Keyboard::KEY_M:
    //         _pressText.hide();
    //         _hideText.show();
    //         break;
    //     case Keyboard::KEY_N:
    //         _pressText.show();
    //         _hideText.hide();
    //         break;
    //     default:
    //         // (void*)_currChosenImage;
    //         break;
    // }

    void Game::setMousePosText(const Point& mousePos){
        _mousePosText.show();
        _mousePosText.setPosition(mousePos);
        std::string textContent="X: ";
        textContent.append(std::to_string(mousePos.x))
                    .append(", Y: ")
                    .append(std::to_string(mousePos.y));
        _mousePosText.setText(textContent);
    }