// Corresponding header
#include "sdl_utils/Renderer.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_render.h>
// Own headers
#include "sdl_utils/Texture.h"

int32_t Renderer::init(SDL_Window* window){

    // SDL_RENDERER_SOFTWARE // allows to use the CPU for rendering - added for legacy reasons

    // SDL_RENDERER_PRESENTVSYNC // enables VSYNC -> read the notes (Zhivko's explanation) in the notebook
    // VSYNC - we do not sleep in our application, but the speed of the frame / loop obeys the refresh rate of the monitor
    
    // SDL_RENDERER_TARGETTEXTURE // we can create runtime images via this option...
    // load four normal images and unite them in one big image - can do all of this runtime...
    // this one big image that we get can be manipulated, moved, rotated, animated, do whatever we want with it runtime !!!

    const auto unspecifiedDriverId=-1; // -1 means that SDL decides for itself which back-end graphical library to use

    _sdlRenderer=SDL_CreateRenderer(window,unspecifiedDriverId,SDL_RENDERER_ACCELERATED);
    if(nullptr==_sdlRenderer){
        std::cerr<<"SDL_CreateRenderer() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // when we say "clear" to the buffer, the buffer just "paints" or "smudges" all the pixels with some information...
    // ... and this information is actually a particular color...so set the color via SDL_SetRenderDrawColor()
    // Later in the course / lectures we will create a "class Color" to have easy access to different colors
    if(EXIT_SUCCESS!=SDL_SetRenderDrawColor(_sdlRenderer,0,0,255,SDL_ALPHA_OPAQUE)){
        std::cerr<<"SDL_SetRenderDrawColor() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // a global variable in "Texture.cpp" is set, BUT this is done in a smart and clean way !!!
    Texture::setRenderer(_sdlRenderer);

    return EXIT_SUCCESS;
}

void Renderer::deinit(){
    if(nullptr!=_sdlRenderer){
        SDL_DestroyRenderer(_sdlRenderer);
        _sdlRenderer=nullptr;
    }
}

void Renderer::clearScreen(){
    if(EXIT_SUCCESS!=SDL_RenderClear(_sdlRenderer)){
        std::cerr<<"SDL_RenderClear() failed. Reason: "<<SDL_GetError()<<std::endl;
    }

}

void Renderer::finishFrame(){
    // SDL_RenderPresent() does two things: 1) says swap to the pointers and 2) updates the screen
    SDL_RenderPresent(_sdlRenderer);
}

void Renderer::renderTexture(SDL_Texture* texture){
    // first "nullptr" means take the entire image and second "nullptr" means to draw it on the entire window
    const int32_t err=SDL_RenderCopy(_sdlRenderer,texture,nullptr,nullptr);
    if(EXIT_SUCCESS!=err){
        std::cerr<<"SDL_RenderCopy() failed. Reason: "<<SDL_GetError()<<std::endl;
    }
}