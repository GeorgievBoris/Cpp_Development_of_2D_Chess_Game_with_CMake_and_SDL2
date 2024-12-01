// Corresponding header
#include "Texture.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_surface.h>
#include <SDL_image.h>
// Own headers


int32_t Texture::createSurfaceFromFile(const char* filePath, SDL_Surface*& outSurface){
    outSurface=IMG_Load(filePath);

    if(nullptr==outSurface){
        std::cerr<<"IMG_Load() failed. Reason: "<<IMG_GetError()<<std::endl;
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