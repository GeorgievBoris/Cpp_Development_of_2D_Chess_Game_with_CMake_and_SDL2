// Corresponding header
#include "sdl_utils/Texture.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_surface.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_ttf.h>
// Own headers
#include "utils/drawing/Color.h"


// this is a clean and smart way of using a global variable - because it is "static" ...
// ... which means that the variable is NOT visible for any other source files than Texture.cpp ...
// ... and the variable can only be accessed via a setter / getter method
// see how "gRenderer" is set in "Renderer::init()" via the setter method "Texture::setRenderer()"
static SDL_Renderer* gRenderer=nullptr;

int32_t Texture::createSurfaceFromFile(const char* filePath, SDL_Surface*& outSurface){
    outSurface=IMG_Load(filePath);

    if(nullptr==outSurface){
        std::cerr<<"IMG_Load() failed. Reason: "<<IMG_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int32_t Texture::createTextureFromFile(const char* filePath, SDL_Texture*& outTexture){

    // NOTE: it is good to have the function "createSurfaceFromFile", because "SDL_Surface" has MANY USEFUL parameters...
    // ... that we can access...while the implementation of SDL_Texture is hidden from us and...
    // ... hence we do not know what parameters SDL_Texture contains...(hidden from us because it is driver specific)
      
    SDL_Surface* surface=nullptr;
    if(EXIT_SUCCESS!=createSurfaceFromFile(filePath,surface)){
        std::cerr<<"Texture::createSurfaceFromFile() failed for file: "<<filePath<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=createTextureFromSurface(surface,outTexture)){
        std::cerr<<"Texture::createTextureFromSurface() failed for file: "<<filePath<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int32_t Texture::createTextureFromSurface(SDL_Surface*& inOutSurface, SDL_Texture*& outTexture){

    outTexture=SDL_CreateTextureFromSurface(gRenderer,inOutSurface);

    if(nullptr==outTexture){
        std::cerr<<"SDL_CreateTextureFromSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    freeSurface(inOutSurface);

    return EXIT_SUCCESS;
}

int32_t Texture::createTextureFromText(const char* text, const Color& color, TTF_Font* font,
                                            SDL_Texture*& outTexture,
                                            int32_t& outTextWidth, int32_t& outTextHeight){
                                                
    // SDL_Color color={ .r=255,.g=0,.b=0,.a=255 };
    const SDL_Color* sdlColor=reinterpret_cast<const SDL_Color*>(&color.rgba);

    // TTF_RenderText_Blended() -> allows us to perform anti-aliasing on the givent font !!!
    // TTF_RenderText_Solid() -> does NOT allow us to perform anti-aliasing on the givent font !!!    
    SDL_Surface* textSurface=TTF_RenderText_Blended(font,text,*sdlColor);

    if(nullptr==textSurface){
        std::cerr<<"Error, TTF_RenderText_Blended() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    outTextWidth=textSurface->w;
    outTextHeight=textSurface->h;

    if(EXIT_SUCCESS!=createTextureFromSurface(textSurface,outTexture)){
        std::cerr<<"Texture::createTextureFromSurface() failed for text: "<<text<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Texture::freeSurface(SDL_Surface*& outSurface) {

    if(outSurface){
        SDL_FreeSurface(outSurface);
        outSurface=nullptr;
    }
}

void Texture::freeTexture(SDL_Texture*& outTexture){
    if(nullptr!=outTexture){
        SDL_DestroyTexture(outTexture);
        outTexture=nullptr;
    }
}

void Texture::setRenderer(SDL_Renderer* renderer){
    gRenderer=renderer;
}

int32_t Texture::setBlendModeTexture(SDL_Texture* texture, BlendMode blendMode){

    if(EXIT_SUCCESS!=SDL_SetTextureBlendMode(texture,static_cast<SDL_BlendMode>(blendMode))){
        std::cerr<<"SDL_SetTextureBlendMode() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int32_t Texture::setAlphaTexture(SDL_Texture* texture, int32_t alpha){

    if(ZERO_OPACITY>alpha || FULL_OPACITY<alpha){
        std::cerr<<"Error, invalid alpha value: "<<alpha<<" provided."<<std::endl;
        return EXIT_FAILURE;
    }
    if(EXIT_SUCCESS!=SDL_SetTextureAlphaMod(texture,static_cast<uint8_t>(alpha))){
        std::cerr<<"SDL_SetTextureAlphaMod() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}