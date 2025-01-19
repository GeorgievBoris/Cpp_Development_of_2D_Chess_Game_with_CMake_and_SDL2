// Corresponding header
#include "sdl_utils/containers/ImageContainer.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include "SDL_render.h"
// Own headers
#include "sdl_utils/Texture.h"

int32_t ImageContainer::init(const ImageContainerCfg& cfg){

    for(const auto& pair:cfg.imageConfigs){
        const auto& elem=pair.second;
        const auto rsrcId=pair.first;
        if(EXIT_SUCCESS!=loadSingleResource(elem,rsrcId)){
            std::cerr<<"ImageContainer::loadSingleResource() failed for file: "<<elem.location<<std::endl;
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}


void ImageContainer::deinit(){
    for(auto& pair:_textures){
        Texture::freeTexture(pair.second);
    }
}

SDL_Texture* ImageContainer::getImageTexture(int32_t rsrcId) const{
    auto it=_textures.find(rsrcId);
    if(_textures.end()==it){
        std::cerr<<"Error, invalid rsrcId: "<<rsrcId<<" requested. Returning NULLPTR"<<std::endl;
        return nullptr;
    }
    // return (*it).second;
    return it->second;
}

Rectangle ImageContainer::getImageFrame(int32_t rsrcId) const{
    auto it=_textureFrames.find(rsrcId);
    if(_textureFrames.end()==it){
        std::cerr<<"Error, invalid rsrcId: "<<rsrcId<<" requested. Returning Rectangle::ZERO"<<std::endl;
        return Rectangle::ZERO;
    }
    // return (*it).second;
    return it->second;
}

int32_t ImageContainer::loadSingleResource(const ImageCfg& resCfg, int32_t rsrcId){
    SDL_Texture* texture=nullptr;
    if(EXIT_SUCCESS!=Texture::createTextureFromFile(resCfg.location.c_str(),texture)){
        std::cerr<<"Texture::createTextureFromFile() failed for file: "<<resCfg.location<<std::endl;
        return EXIT_FAILURE;
    }

    _textures[rsrcId]=texture;

    Rectangle& rect = _textureFrames[rsrcId];
    rect.x=0;
    rect.y=0;
    rect.w=resCfg.width;
    rect.h=resCfg.height;

    return EXIT_SUCCESS;
}