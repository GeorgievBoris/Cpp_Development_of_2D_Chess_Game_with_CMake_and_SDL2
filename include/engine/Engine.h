#ifndef ENGINE_ENGINE_H_
#define ENGINE_ENGINE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/Game.h"
#include "sdl_utils/InputEvent.h"
#include "manager_utils/managers/ManagerHandler.h"
// Forward Declarations
struct EngineCfg;

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
    
    InputEvent _event;
    Game _game;
    ManagerHandler _managerHandler;
};



#endif // ENGINE_ENGINE_H_