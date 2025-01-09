// Corresponding header
#include "Texture.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_surface.h>
#include <SDL_image.h>
#include <SDL_render.h>
// Own headers


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