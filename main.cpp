// C system includes
#include <cstdint>
#include <cstdlib>
// C++ system includes
#include <iostream>
#include <string>
// Third-party includes
#include <SDL.h>
// Own includes
#include "sdl_utils/SDLLoader.h"
#include "sdl_utils/MonitorWindow.h"
#include "sdl_utils/Texture.h"

static void draw(MonitorWindow& window, SDL_Surface* image) {

    SDL_Surface* screenSurface=window.getWindowSurface(); // according to the documentation: This surface will be freed when the window is destroyed.

    if(EXIT_SUCCESS!=SDL_BlitSurface(image,nullptr,screenSurface,nullptr)){
        std::cerr<<"SDL_BlitSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return;
    }

    if(EXIT_SUCCESS!=window.updateWindowSurface()){
        std::cerr<<"MonitorWindow::updateWindowSurface() failed."<<std::endl;
        return;
    }

    constexpr uint32_t timeDelayMilliseconds= 3000;
    SDL_Delay(timeDelayMilliseconds);

    Texture::freeSurface(screenSurface);
}


static int32_t loadResources(SDL_Surface*& outImage){
    // const std::string filePath = "../resources/hello.png"; // the system path is given relative to the "build" directory !!!
    const char* filePath="../resources/hello.png"; // the system path is given relative to the "build" directory !!!

    if(EXIT_SUCCESS!=Texture::createSurfaceFromFile(filePath,outImage)){
        std::cerr<<"Texture::createSurfaceFromFile() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static int32_t init(MonitorWindow& window, SDL_Surface*& outImage){
    MonitorWindowCfg cfg;
    cfg.windowName = "SDL_Runtime";
    cfg.windowWidth = 640;
    cfg.windowHeight = 480;
    cfg.windowFlags = WINDOW_SHOWN; 

    if(EXIT_SUCCESS!=window.init(cfg)){
        std::cerr<<"MonitorWindow::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=loadResources(outImage)){
        std::cerr<<"loadResources() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void deinit(MonitorWindow& window, SDL_Surface*& outImage) {
    // here the deinitialization is performed...
    // ... in a reverse order to the init() function

    Texture::freeSurface(outImage);
    window.deinit();
}

static int32_t runApplication() {

    SDL_Surface* image = nullptr;
    MonitorWindow window;


    if(EXIT_SUCCESS!=init(window,image)){
        std::cerr<<"init() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    draw(window,image);

    deinit(window,image);

    return EXIT_SUCCESS;
}

int32_t main([[maybe_unused]]int32_t argc, [[maybe_unused]]char* argv[]){

    if(EXIT_SUCCESS!=SDLLoader::init()){
        std::cerr<<"SDLLoader::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=runApplication()){
        std::cerr<<"runApplication() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    SDLLoader::deinit();

    return EXIT_SUCCESS;
}