// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include<iostream>
// Third-party headers
#include <SDL_render.h>
// Own headers
#include "sdl_utils/InputEvent.h"
#include "sdl_utils/Texture.h"


int32_t Game::init(const GameCfg& cfg){

    if(EXIT_SUCCESS!=loadResources(cfg.imgPaths)){
        std::cerr<<"Game::loadResources() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    _currChosenImage=_imageSurfaces[PRESS_KEYS];

    return EXIT_SUCCESS;
}

void Game::deinit(){
    for(int32_t i=0;i<COUNT;++i){
        Texture::freeTexture(_imageSurfaces[i]);
    }
}

void Game::draw(std::vector<SDL_Texture*>& outImages){
    outImages.push_back(_currChosenImage);
    // outImages.push_back(_imageSurfaces[LAYER_2]);
}



void Game::handleEvent(const InputEvent& e){

    // if(TouchEvent::KEYBOARD_RELEASE==_event.type){
    //     _currChosenImage=_imageSurfaces[ALL_KEYS];
    //     return;
    // }

    if(TouchEvent::KEYBOARD_PRESS!=e.type){
        TouchEvent::KEYBOARD_RELEASE==e.type ? _currChosenImage=_imageSurfaces[PRESS_KEYS] : (void*)_currChosenImage;
        return;
    }

    switch(e.key){
        case Keyboard::KEY_UP:
            _currChosenImage=_imageSurfaces[UP];
            break;
        case Keyboard::KEY_DOWN:
            _currChosenImage=_imageSurfaces[DOWN];
            break;
        case Keyboard::KEY_LEFT:
            _currChosenImage=_imageSurfaces[LEFT];
            break;
        case Keyboard::KEY_RIGHT:
            _currChosenImage=_imageSurfaces[RIGHT];
            break;
        default:
            // (void*)_currChosenImage;
            break;
    }
}

int32_t Game::loadResources(const std::unordered_map<GameImages,std::string>& res){

    for(const auto& pair:res){
        const auto resId=pair.first;
        const auto& resLocation=pair.second;
        if(EXIT_SUCCESS!=Texture::createTextureFromFile(resLocation.c_str(),_imageSurfaces[resId])){
            std::cerr<<"Texture::createTextureFromFile() failed for file: "<<resLocation<<std::endl;
            return EXIT_FAILURE;            
        }
    }
    
    return EXIT_SUCCESS;
}