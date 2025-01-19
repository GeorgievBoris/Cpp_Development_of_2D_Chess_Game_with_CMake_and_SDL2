#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MANAGERHANDLER_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MANAGERHANDLER_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/managers/MgrBase.h"
// Forward Declarations
struct ManagerHandlerCfg;

class ManagerHandler{
public:
    int32_t init(const ManagerHandlerCfg& cfg);
    void deinit();
    void process();

private:
    void nullifyGlobalMgr(int32_t mgrIdx);
    
    MgrBase* _managers[MANAGERS_COUNT];
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_MANAGERS_MANAGER_HANDLER_H_