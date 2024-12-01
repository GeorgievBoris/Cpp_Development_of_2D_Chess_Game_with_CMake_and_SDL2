// C system includes
#include <cstdint>
#include <cstdlib>
// C++ system includes
#include <iostream>
#include <string>
// Third-party includes
#include <SDL.h>
// Own includes


static void draw(SDL_Window* window,SDL_Surface* screenSurface, SDL_Surface* image) {
    if(EXIT_SUCCESS!=SDL_BlitSurface(image,nullptr,screenSurface,nullptr)){
        std::cerr<<"SDL_BlitSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return;
    }

    if(EXIT_SUCCESS!=SDL_UpdateWindowSurface(window)){
        std::cerr<<"SDL_UpdateWindowSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return;
    }

    constexpr uint32_t timeDelayMilliseconds= 3000;
    SDL_Delay(timeDelayMilliseconds);
}


static int32_t loadResources(SDL_Surface*& outImage){
    const std::string filePath = "../resources/hello.bmp";
    outImage=SDL_LoadBMP(filePath.c_str());
    if (nullptr == outImage){
        std::cerr<<"SDL_LoadBMP() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int32_t init(SDL_Window*& outWindow, SDL_Surface*& outScreenSurface, SDL_Surface*& outImage){

    const std::string windowName = "Hello, World!";
    const int32_t windowX = SDL_WINDOWPOS_UNDEFINED; // SDL_WINDOWPOS_CENTERED;
    const int32_t windowY = SDL_WINDOWPOS_UNDEFINED; // SDL_WINDOWPOS_CENTERED;
    const int32_t windowWidth = 640;
    const int32_t windowHeight = 480;
    const uint32_t windowFlag = SDL_WINDOW_SHOWN; // SDL_WINDOW_HIDDEN SDL_WINDOW_FULLSCREEN_DESKTOP SDL_WINDOW_MAXIMIZED

    outWindow = SDL_CreateWindow(windowName.c_str(),windowX,windowY,windowWidth,windowHeight,windowFlag);
    if(nullptr==outWindow){
        std::cerr<<"SDL_CreateWindow() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // according to the documentation: This surface will be freed when the window is destroyed.
    outScreenSurface = SDL_GetWindowSurface(outWindow);
    if(nullptr==outScreenSurface){
        std::cerr<<"SDL_GetWindowSurface() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS != loadResources(outImage)){
        std::cerr<<"loadResources() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

static void deinit(SDL_Window*& outWindow, [[maybe_unused]] SDL_Surface*& outScreenSurface, SDL_Surface*& outImage) {

    // here the deinitialization is performed...
    // ... in a reverse order to the init() function

    if(nullptr!=outImage){
        SDL_FreeSurface(outImage);
        outImage=nullptr;
    }
    // // according to the documentation: This surface will be freed when the window is destroyed.
    // if(nullptr!=outScreenSurface){
    //     SDL_FreeSurface(outScreenSurface);
    //     outScreenSurface = nullptr;
    // }
    
    if(nullptr!=outWindow){
        SDL_DestroyWindow(outWindow);
        outWindow = nullptr;
    }
}


static int32_t runApplication() {

    SDL_Window* window=nullptr;
    SDL_Surface* screenSurface=nullptr;
    SDL_Surface* image=nullptr;

    if(EXIT_SUCCESS!=init(window,screenSurface,image)){
        std::cerr<<"init() failed."<<std::endl;
        return EXIT_FAILURE;
    }

    draw(window,screenSurface,image);

    deinit(window,screenSurface,image);

    return EXIT_SUCCESS;
}

int32_t main([[maybe_unused]]int32_t argc, [[maybe_unused]]char* argv[]){

    // In this case the VIDEO system of SDL is initialized, BUT can initialize many others as well
    if(EXIT_SUCCESS!=SDL_Init(SDL_INIT_VIDEO)){
        std::cerr<<"SDL_Init() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    runApplication();

    SDL_Quit();

    return EXIT_SUCCESS;
}