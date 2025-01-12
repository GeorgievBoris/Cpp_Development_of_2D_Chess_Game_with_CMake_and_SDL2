// Corresponding header
#include "sdl_utils/SDLLoader.h"
// C system includes
// C++ system includes
#include <cstdlib>
#include <iostream>
// Third-party includes
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
// Own includes


int32_t SDLLoader::init(){

    if(EXIT_SUCCESS!=TTF_Init()){
        std::cerr<<"TTF_Init() failed. Reason: "<<TTF_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO)){
        std::cerr<<"SDL_Init() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    const int32_t imgFlags = IMG_INIT_PNG; // Zhivko said that this includes .JPEG inside it by default
    // function description says that OR is used instead of AND !!!
    if(!(IMG_Init(imgFlags) & imgFlags)){
        std::cerr<<"IMG_Init() failed. Reason: "<<IMG_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    constexpr int32_t soundFrequency = 44100;
    constexpr int32_t numOfChannels = 2;
    constexpr int32_t chunkSize = 2048;
    if(0 > Mix_OpenAudio(soundFrequency,MIX_DEFAULT_FORMAT,numOfChannels,chunkSize)){
        std::cerr<<"Mix_OpenAudio() failed. Reason: "<<Mix_GetError()<<std::endl;
        return EXIT_FAILURE;
    }
    

    return EXIT_SUCCESS;
}


void SDLLoader::deinit(){
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
    TTF_Quit();
}