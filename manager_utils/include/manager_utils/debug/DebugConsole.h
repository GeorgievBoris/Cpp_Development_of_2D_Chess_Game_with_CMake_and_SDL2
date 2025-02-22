#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DEBUG_DEBUGCONSOLE_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DEBUG_DEBUGCONSOLE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Text.h"
// Forward Declarations
class InputEvent;

enum DebugTextsIdx{
    FPS_TEXT_IDX,
    ACTIVE_TIMER_IDX,
    ACTIVE_WIDGETS_IDX,

    DEBUG_TEXTS_COUNT
};

class DebugConsole{
public:
    int32_t init(int64_t maxFrames, int32_t fontId);
    void handleEvent(const InputEvent& e);
    void update(int64_t elapsedMicroseconds, size_t activeTimers);
    void updateActiveWidgets(int32_t activeWidgets); // How can this be optimized by using a Frame Buffer Object ?
    void draw() const;
    bool isActive() const;
private:
    Text _debugTexts[DEBUG_TEXTS_COUNT];
    int64_t _maxFrames=0; // used to determine the current FPS;
    int32_t _updateCounter=0; // used for skipping turns - this variable is NOT used at the moment
    bool _isActive=false;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DEBUG_DEBUGCONSOLE_H_