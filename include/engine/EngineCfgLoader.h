#ifndef ENGINE_ENGINECFGLOADER_H_
#define ENGINE_ENGINECFGLOADER_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "engine/config/EngineCfg.h"
// Forward Declarations

class EngineCfgLoader{
public:
    EngineCfgLoader()=delete;
    ~EngineCfgLoader()=default;

    static EngineCfg loadCfg();
private:
};

#endif // ENGINE_ENGINECFGLOADER_H_