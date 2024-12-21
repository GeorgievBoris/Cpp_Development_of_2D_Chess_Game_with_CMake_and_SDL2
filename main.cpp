// C system includes
#include <cstdint>
#include <cstdlib>
// C++ system includes
#include <iostream>
// Third-party includes
// Own includes
#include "engine/Engine.h"
#include "sdl_utils/SDLLoader.h"

static int32_t runApplication() {

    Engine engine;

    if(EXIT_SUCCESS!=engine.init()){
        std::cerr<<"engine.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    engine.start();

    engine.deinit();

    return EXIT_SUCCESS;
}

int32_t main([[maybe_unused]]int32_t argc, [[maybe_unused]]char* argv[]){
// int32_t main (int32_t argc, char* argv[]){
//     (void)argc; (void)argv;

    if(EXIT_SUCCESS!=SDLLoader::init()){ // this is an initialization of any dependencies (i.e. the SDL libary)
        std::cerr<<"SDLLoader::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=runApplication()){
        std::cerr<<"runApplication() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    SDLLoader::deinit(); // deinitialization of any initialized dependecies

    return EXIT_SUCCESS;
}