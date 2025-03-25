#ifndef INCLUDE_STARTSCREEN_DEFINES_STARTSCREENDEFINES_H_
#define INCLUDE_STARTSCREEN_DEFINES_STARTSCREENDEFINES_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

// NOTE: "StartScreenDefines.h" is NOT created by Zhivko!

namespace startScreenDefines{

enum InternalDefines{
    EXIT_BUTTON,
    START_GAME_BUTTON,
    BUTTONS_COUNT
};

} // end of startScreenDefines

enum class StartScreenButtonsType{
    EXIT,
    START_CHESS,
    UNKNOWN
};

#endif // INCLUDE_STARTSCREEN_DEFINES_STARTSCREENDEFINES_H_