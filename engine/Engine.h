#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/Game.h"
#include "sdl_utils/MonitorWindow.h"
#include "sdl_utils/InputEvent.h"
#include "sdl_utils/Renderer.h"
#include "sdl_utils/containers/ImageContainer.h"
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
    Renderer _renderer;
    ImageContainer _imgContainer;
    Game _game;
};



#endif // ENGINE_ENGINE_H_