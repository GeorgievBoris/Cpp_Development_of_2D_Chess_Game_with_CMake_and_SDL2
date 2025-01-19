#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_DRAWMGRCFG_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_DRAWMGRCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/config/MonitorWindowCfg.h"
// Forward Declarations

struct DrawMgrCfg{
    MonitorWindowCfg windowCfg;
    int64_t maxFrameRate;
};

#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_DRAWMGRCFG_H_