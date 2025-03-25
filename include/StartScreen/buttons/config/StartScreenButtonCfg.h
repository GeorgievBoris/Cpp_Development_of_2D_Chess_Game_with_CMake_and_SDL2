#ifndef INCLUDE_STARTSCREEN_BUTTONS_CONFIG_STARTSCREENBUTTONCFG_H_
#define INCLUDE_STARTSCREEN_BUTTONS_CONFIG_STARTSCREENBUTTONCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <functional>
// Third-party headers
// Own headers
#include "utils/drawing/Point.h"
#include "StartScreen/defines/StartScreenDefines.h"
// Forward Declarations

// NOTE: "StartScreenButtonCfg.h" is NOT created by Zhivko!

struct StartScreenButtonCfg{
    int32_t btnWidth {};
    int32_t btnHeight {};
    Point btnPos=Point::ZERO;
    StartScreenButtonsType btnType=StartScreenButtonsType::UNKNOWN;
    int32_t startScreenBtnsRsrcId=INVALID_RSRC_ID;
    std::function<void()> btnCallBack;
};

#endif // INCLUDE_STARTSCREEN_BUTTONS_CONFIG_STARTSCREENBUTTONCFG_H_