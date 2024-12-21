#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/MonitorWindow.h"
#include "sdl_utils/InputEvent.h"
// Forward Declarations
struct SDL_Surface;

enum Images {
    UP,DOWN,LEFT,RIGHT,ALL_KEYS,COUNT
};


class Engine {
public:
    int32_t init();
    void deinit();
    void start();
    void draw();

private:

    void mainLoop();
    void drawFrame();
    bool processFrame();
    void handleEvent();

    void limitFPS(int64_t elapsedTimeMicroseconds);
    int32_t loadResources();
        

    MonitorWindow _window;
    InputEvent _event;
    SDL_Surface* _screenSurface=nullptr;
    // game specific logic - in theory this has no place inside the Engine
    SDL_Surface* _currChosenImage=nullptr;
    SDL_Surface* _imageSurfaces[COUNT]{};
};



#endif // ENGINE_ENGINE_H_