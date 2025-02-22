// Corresponding header
#include "sdl_utils/containers/TextContainer.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_ttf.h>
// Own headers
#include "utils/drawing/Color.h"
#include "sdl_utils/Texture.h"


int32_t TextContainer::init(const TextContainerCfg& cfg){
    TTF_Font* currFont=nullptr;
    for (const auto& [key, fontCfg]:cfg.fontConfigs){
        currFont=TTF_OpenFont(fontCfg.location.c_str(),fontCfg.fontSize);

        if(nullptr==currFont){
            std::cerr<<"Error, TTF_OpenFont() failed for fontLocation: "
                    <<fontCfg.location<<". Reason: "<<SDL_GetError()<<std::endl;
            return EXIT_FAILURE;
        }
        _fonts[key]=currFont;
    }

    return EXIT_SUCCESS;
}


void TextContainer::deinit(){
    for(auto& pair:_fonts){
        TTF_CloseFont(pair.second); // usually it makes sense to close the font, only when we close the entire application 
        pair.second=nullptr;
    }

    // IMPORTANT - propagate the below range-based for loop back to the previos lectures...
    // ... Zhivko spotted it in this Lecture !!!
    for(SDL_Texture*& texture:_textures){
        if(nullptr!=texture){
            Texture::freeTexture(texture);
        }
    }
}


void TextContainer::createText(const char* text, const Color& color, int32_t fontId,
                                    int32_t& outTextId, int32_t& outTextWidth,
                                    int32_t& outTextHeight){

    auto it=_fonts.find(fontId);
    if(_fonts.end()==it){
        std::cerr<<"Error, fontId: "<<fontId<<"could not be found. Will not create text: "<<text<<std::endl;
        return;
    }
    
    TTF_Font* font=it->second;
    SDL_Texture* textTexture=nullptr;
    if(EXIT_SUCCESS!=Texture::createTextureFromText(text,color,font,textTexture,outTextWidth,outTextHeight)){
        std::cerr<<"Error, Texture::createTextureFromText() failed for text: "<<text<<std::endl;
        return;
    }

    occupyFreeSlotIndex(outTextId,textTexture);

    return;

}

void TextContainer::reloadText(const char* text, const Color& color, int32_t fontId,
                                    int32_t textId, int32_t& outTextWidth,
                                    int32_t& outTextHeight){
    auto it=_fonts.find(fontId);
    if(_fonts.end()==it){
        std::cerr<<"Error, fontId: "<<fontId<<"could not be found. Will not reload text: "<<text<<std::endl;
        return;
    }

    Texture::freeTexture(_textures[textId]); // better to use "unloadText(textId)" ????

    TTF_Font* font=it->second;
    SDL_Texture* textTexture=nullptr;
    if(EXIT_SUCCESS!=Texture::createTextureFromText(text,color,font,textTexture,outTextWidth,outTextHeight)){
        std::cerr<<"Error, Texture::createTextureFromText() failed for text: "<<text<<std::endl;
        return;
    }
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    _textures[static_cast<size_t>(textId)]=textTexture;
}

void TextContainer::unloadText(int32_t textId){
    if(0>textId || static_cast<int32_t>(_textures.size())<=textId){
        std::cerr<<"Error, trying to unload a non-existing textId: "<<textId<<std::endl;
        return;
    }

    Texture::freeTexture(_textures[textId]);
}

SDL_Texture* TextContainer::getTextTexture(int32_t textId) const {
    if(0>textId || static_cast<int32_t>(_textures.size())<=textId){
        std::cerr<<"Error, trying to get a non-existing textId: "<<textId<<std::endl;
        return nullptr;
    }
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    return _textures[static_cast<size_t>(textId)];
}

void TextContainer::occupyFreeSlotIndex(int32_t& outIdx, SDL_Texture* texture){
    const size_t size = _textures.size();
    for (size_t i=0;i<size;++i){
        if(nullptr==_textures[i]){ // found free index
            _textures[i]=texture;
            // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
            outIdx=static_cast<int32_t>(i);
            return;
        }
    }
    _textures.push_back(texture);
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    outIdx=static_cast<int32_t>(size);
}
