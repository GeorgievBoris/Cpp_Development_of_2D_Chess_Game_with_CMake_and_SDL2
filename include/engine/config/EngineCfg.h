#ifndef ENGINE_CONFIG_ENGINECFG_H_
#define ENGINE_CONFIG_ENGINECFG_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/config/MonitorWindowCfg.h"
#include "sdl_utils/config/ImageContainerCfg.h"
#include "sdl_utils/config/TextContainerCfg.h"
#include "game/config/GameCfg.h"
// Forward Declarations

struct EngineCfg {
    MonitorWindowCfg windowCfg;
    ImageContainerCfg imgContainerCfg;
    TextContainerCfg textContainerCfg;
    GameCfg gameCfg;
};


#endif // ENGINE_CONFIG_ENGINECFG_H_