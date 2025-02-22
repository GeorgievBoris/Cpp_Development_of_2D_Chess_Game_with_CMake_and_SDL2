// Corresponding header
#include "sdl_utils/containers/FboContainer.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/Texture.h"


int32_t FboContainer::init(){
    return EXIT_SUCCESS;
}

void FboContainer::deinit(){
    for(SDL_Texture*& texture:_textures){
        Texture::freeTexture(texture);
    }
}

void FboContainer::createFbo(int32_t fboWidth, int32_t fboHeight, int32_t& outFboId){
    
    SDL_Texture* texture=nullptr;
    if(EXIT_SUCCESS!=Texture::createEmptyTexture(fboWidth,fboHeight,texture)){
        std::cerr<<"Texture::createEmptyTexture() failed"<<std::endl;
        return;
    }

    occupyFreeSlotIndex(outFboId,texture);
}

void FboContainer::unloadFbo(int32_t fboId){
    if(0>fboId || static_cast<int32_t>(_textures.size())<=fboId){
        std::cerr<<"Error, trying to unload a non-existing fboId: "<<fboId<<std::endl;
        return;
    }
    freeSlotIndex(fboId);
}

SDL_Texture* FboContainer::getFboTexture(int32_t fboId) const{
    if(0>fboId || static_cast<int32_t>(_textures.size())<=fboId){
        std::cerr<<"Error, trying to get a non-existing fboId: "<<fboId<<std::endl;
        return nullptr;
    }
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    return _textures[static_cast<size_t>(fboId)];
}

void FboContainer::occupyFreeSlotIndex(int32_t& outIdx, SDL_Texture* texture){
    const size_t size=_textures.size();

    for(size_t i=0;i<size;++i){
        if(nullptr==_textures[i]){
            // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
            outIdx=static_cast<int32_t>(i);
            _textures[i]=texture;
            return;
        }
    }
    
    _textures.push_back(texture);
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    outIdx=static_cast<int32_t>(size);
}

void FboContainer::freeSlotIndex(int32_t index){
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    Texture::freeTexture(_textures[static_cast<size_t>(index)]);
}