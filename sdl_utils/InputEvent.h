#ifndef SDL_UTILS_INPUTEVENT_H_
#define SDL_UTILS_INPUTEVENT_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/Point.h"
#include "utils/input/EventDefines.h"
// Forward Declarations
union SDL_Event; // Do not expose SDL_Event internals to the system -> so use forward declarations instead of "#include<>"

class InputEvent {
public:
    int32_t init();
    void deinit();
    bool pollEvent();
    bool checkForExitRequest() const;

    Point pos=Point::UNDEFINED;
    int32_t key=Keyboard::KEY_UNKNOWN;
    uint8_t mouseButton=Mouse::UNKNOWN;
    TouchEvent type=TouchEvent::UNKNOWN;
    SDL_Event* _sdlEvent = nullptr;    

private:
    void setEventTypeInternal();
};

#endif // SDL_UTILS_INPUTEVENT_H_