// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include<iostream>
// Third-party headers
#include <SDL_render.h>
// Own headers
#include "sdl_utils/InputEvent.h"
#include "sdl_utils/containers/ImageContainer.h"

int32_t Game::init(const GameCfg& cfg, const ImageContainer* imageContainerInterface){
    if(nullptr==imageContainerInterface){
        std::cerr<<"Error, nullptr provided for imageContainerInterface"<<std::endl;
        return EXIT_FAILURE;
    }
    _imageContainer=imageContainerInterface;

    _layer2Img.rsrcId=cfg.layer2RsrcId;
    Rectangle rect = _imageContainer->getImageFrame(_layer2Img.rsrcId);
    _layer2Img.width=rect.w;
    _layer2Img.height=rect.h;
    _layer2Img.pos=Point::ZERO;

    _pressKeysImg.rsrcId=cfg.pressKeysRsrcId;
    rect=_imageContainer->getImageFrame(_pressKeysImg.rsrcId);
    _pressKeysImg.width=rect.w;
    _pressKeysImg.height=rect.h;
    _pressKeysImg.pos=Point::ZERO;
    // _pressKeysImg.pos.y+=20;

    _pressKeysImgDuplicate=_pressKeysImg; // for testing purposes - to check if opacity changes for the created SDL_Texture  

    return EXIT_SUCCESS;
}

void Game::deinit(){
    
}

void Game::draw(std::vector<DrawParams>& outImages){
    // outImages.push_back(_pressKeysImg);
    outImages.push_back(_pressKeysImgDuplicate); // for testing purposes - to check if opacity changes for the created SDL_Texture
    outImages.push_back(_pressKeysImg);
    // outImages.push_back(_layer2Img);
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
        default:
            break;
    }
}