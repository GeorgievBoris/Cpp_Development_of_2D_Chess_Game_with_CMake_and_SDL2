#ifndef ENGINE_CONFIG_ENGINECFG_H_
#define ENGINE_CONFIG_ENGINECFG_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/config/ManagerHandlerCfg.h"
#include "game/config/GameCfg.h"
#include "StartScreen/config/StartScreenCfg.h"
// Forward Declarations

struct EngineCfg {
    ManagerHandlerCfg managerHandlerCfg;
    GameCfg gameCfg;

    int32_t debugConsoleFontId;

    StartScreenCfg startScreenCfg; // NOT added by Zhivko
    int32_t startScreenButtonsRsrcId; // NOT added by Zhivko
};


#endif // ENGINE_CONFIG_ENGINECFG_H_