#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_MANAGERHANDLERCFG_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_MANAGERHANDLERCFG_H_

// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/config/DrawMgrCfg.h"
#include "manager_utils/config/RsrcMgrCfg.h"
// Forward Declarations

struct ManagerHandlerCfg{
    DrawMgrCfg drawMgrCfg;
    RsrcMgrCfg rsrcMgrCfg;
};

#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_CONFIG_MANAGERHANDLERCFG_H_