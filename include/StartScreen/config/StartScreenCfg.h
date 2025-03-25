#ifndef INCLUDE_STARTSCREEN_CONFIG_STARTSCREENCFG_H_
#define INCLUDE_STARTSCREEN_CONFIG_STARTSCREENCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
// Forward Declarations

// NOTE: "StartScreenCfg.h" is NOT created by Zhivko!

struct StartScreenCfg{
    int32_t btnHeight{};
    int32_t btnWidth{};
    int32_t buttonsRsrcId=INVALID_RSRC_ID;
    int32_t gameBoardWidth{};
    int32_t gameBoardHeight{};
    int32_t backgroundHeight{};
    int32_t backgroundWidth{};
    int32_t backgroundRsrcId=INVALID_RSRC_ID;
    int32_t timerId=INVALID_RSRC_ID; // set to INVALID_RSRC_ID?
    // int32_t buttonsTimerId=INVALID_RSRC_ID; // set to INVALID_RSRC_ID?
    // int32_t backgroundTimerId=INVALID_RSRC_ID; // set to INVALID_RSRC_ID?

    // int32_t startScreenBtnsWidths[startScreenDefines::BUTTONS_COUNT]; // try with this later - if buttons have different Widths and/or Heights 
};

#endif // INCLUDE_STARTSCREEN_CONFIG_STARTSCREENCFG_H_