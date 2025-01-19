#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_RSRCMGRCFG_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_RSRCMGRCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/config/ImageContainerCfg.h"
#include "sdl_utils/config/TextContainerCfg.h"
// Forward Declarations

struct RsrcMgrCfg {
    ImageContainerCfg imgContainerCfg;
    TextContainerCfg textContainerCfg;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_RSRCMGRCFG_H_