#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/MonitorWindow.h"
#include "sdl_utils/InputEvent.h"
#include "game/Game.h"
#include "sdl_utils/Renderer.h"
// Forward Declarations
struct EngineCfg;
struct SDL_Surface;

class Engine {
public:
    int32_t init(const EngineCfg& cfg);
    void deinit();
    void start();
    void draw();

private:

    void mainLoop();
    void drawFrame();
    bool processFrame();
    void handleEvent();

    void limitFPS(int64_t elapsedTimeMicroseconds);
    
    MonitorWindow _window;
    InputEvent _event;
    Game _game;
    Renderer _renderer;
};



#endif // ENGINE_ENGINE_H_