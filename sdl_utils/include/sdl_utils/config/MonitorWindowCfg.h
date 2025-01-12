#ifndef SDL_UTILS_CONFIG_MONITORWINDOWCFG_H_
#define SDL_UTILS_CONFIG_MONITORWINDOWCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <string>
// Third-party headers
// Own headers
#include "utils/drawing/Point.h"
// Forward Declarations

enum WindowFlags {
    WINDOW_SHOWN=4, //SDL_WINDOW_SHOWN
    WINDOW_FULLSCREEN_DESKTOP=4097 //SDL_WINDOW_FULLSCREEN_DESKTOP
    
    // SDL_WINDOW_HIDDEN SDL_WINDOW_FULLSCREEN_DESKTOP SDL_WINDOW_MAXIMIZED can be implemented as well
};

struct MonitorWindowCfg {
    // char* windowName; // an alternative to std::string
    std::string windowName;
    Point windowPos=Point::UNDEFINED;
    int32_t windowWidth=0;
    int32_t windowHeight=0;
    WindowFlags windowFlags=WINDOW_SHOWN;

};


#endif // SDL_UTILS_CONFIG_MONITORWINDOWCFG_H_