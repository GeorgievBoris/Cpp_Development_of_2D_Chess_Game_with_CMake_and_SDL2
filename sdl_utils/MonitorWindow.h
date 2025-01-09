#ifndef SDL_UTILS_MONITORWINDOW_H_
#define SDL_UTILS_MONITORWINDOW_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <string>
// Third-party headers
// Own headers
#include "config/MonitorWindowCfg.h"
// Forward declarations
struct SDL_Window; // do NOT do the "#include<SDL_video.h>" here, because it will break the idea of the wrapper class MonitorWindow...instead use Forward Declarations
struct SDL_Surface;


class MonitorWindow {
public:

    MonitorWindow()=default;
    ~MonitorWindow();

    MonitorWindow(const MonitorWindow& other)=delete; // copy Ctor
    MonitorWindow& operator=(const MonitorWindow& other)=delete; // copy assignment operator

    MonitorWindow(MonitorWindow&& other)=delete; // move Ctor
    MonitorWindow& operator=(MonitorWindow&& other)=delete; // move assignment operator

    int32_t init(const MonitorWindowCfg& cfg);
    void deinit();
    void destroyWindow();

    SDL_Window* getWindow() const;

private:
    SDL_Window* _window=nullptr;
};

#endif // SDL_UTILS_MONITORWINDOW_H_