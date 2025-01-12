// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include<iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "sdl_utils/containers/ImageContainer.h"
#include "sdl_utils/containers/TextContainer.h"
#include "utils/drawing/Color.h"

static int32_t gFontId;

int32_t Game::init(const GameCfg& cfg, const ImageContainer* imageContainerInterface, 
                                                TextContainer* textContainerInterface){
    if(nullptr==imageContainerInterface){
        std::cerr<<"Error, nullptr provided for imageContainerInterface"<<std::endl;
        return EXIT_FAILURE;
    }
    _imageContainer=imageContainerInterface;

    gFontId=cfg.textFontId;
    if(nullptr==textContainerInterface){
        std::cerr<<"Error, nullptr provided for textContainerInterface"<<std::endl;
        return EXIT_FAILURE;
    }
    _textContainer=textContainerInterface;

    _layer2Img.rsrcId=cfg.layer2RsrcId;
    Rectangle rect = _imageContainer->getImageFrame(_layer2Img.rsrcId);
    _layer2Img.width=rect.w;
    _layer2Img.height=rect.h;
    _layer2Img.pos=Point::ZERO;
    _layer2Img.widgetType=WidgetType::IMAGE;

    _pressKeysImg.rsrcId=cfg.pressKeysRsrcId;
    rect=_imageContainer->getImageFrame(_pressKeysImg.rsrcId);
    _pressKeysImg.width=rect.w;
    _pressKeysImg.height=rect.h;
    _pressKeysImg.pos=Point::ZERO;
    _pressKeysImg.widgetType=WidgetType::IMAGE;
    _pressKeysImg.pos.y+=20;

    // _helloText.textId=cfg.textFontId;
    _helloText.widgetType=WidgetType::TEXT;
    _textContainer->createText("Hello, C++ dudes",Colors::GREEN,gFontId,_helloText.textId,
                                                    _helloText.width,_helloText.height);
    _helloText.pos=Point::ZERO;

    _textContainer->createText("Press M to hide", Colors::GREEN,gFontId,_pressText.textId,
                                                    _pressText.width,_pressText.height);
    _pressText.pos=Point::ZERO;
    _pressText.pos.x+=100;
    _pressText.pos.y+=100;
    _pressText.widgetType=WidgetType::TEXT;

    _textContainer->createText("Press N to show",Colors::PURPLE,gFontId,_hideText.textId,
                                                    _hideText.width,_hideText.height);
    
    _hideText.pos=Point::ZERO;
    _hideText.pos.x+=200;
    _hideText.pos.y+=200;
    _hideText.widgetType=WidgetType::TEXT;

    return EXIT_SUCCESS;
}

void Game::deinit(){
    _textContainer->unloadText(_helloText.textId);
}

void Game::draw(std::vector<DrawParams>& outImages){
    outImages.push_back(_pressKeysImg);
    outImages.push_back(_layer2Img);
    outImages.push_back(_helloText);

    if(isPressTextHidden){
        outImages.push_back(_hideText);
    } else {
        outImages.push_back(_pressText);
    }
}

void Game::handleEvent(const InputEvent& e){

    if(TouchEvent::KEYBOARD_RELEASE!=e.type){
        return;
    }

    switch(e.key){
        case Keyboard::KEY_UP:
            _pressKeysImg.pos.y-=10;
            break;
        case Keyboard::KEY_DOWN:
            _pressKeysImg.pos.y+=10;
            break;
        case Keyboard::KEY_LEFT:
            _pressKeysImg.pos.x-=10;
            break;
        case Keyboard::KEY_RIGHT:
            _pressKeysImg.pos.x+=10;
            break;
        case Keyboard::KEY_Q:
            _pressKeysImg.width-=10;
            break;
        case Keyboard::KEY_W:
            _pressKeysImg.width+=10;
            break;
        case Keyboard::KEY_E:
            _pressKeysImg.height-=10;
            break;
        case Keyboard::KEY_R:
            _pressKeysImg.height+=10;
            break;
        case Keyboard::KEY_T:
            _pressKeysImg.opacity-=10;
            break;
        case Keyboard::KEY_Y:
            _pressKeysImg.opacity+=10;
            break;
        case Keyboard::KEY_B:
            _textContainer->reloadText("Kak ste, kolegi?",Colors::GREEN,gFontId,_helloText.textId,
                                                            _helloText.width,_helloText.height);
            break;
        case Keyboard::KEY_M:
            isPressTextHidden=true;
            break;
        case Keyboard::KEY_N:
            isPressTextHidden=false;
            break;
        default:
            // (void*)_currChosenImage;
            break;
    }
}