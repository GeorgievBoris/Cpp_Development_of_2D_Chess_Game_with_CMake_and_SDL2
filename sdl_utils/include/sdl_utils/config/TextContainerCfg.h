#ifndef SDL_UTILS_CONFIG_TEXTCONTAINERCFG_H_
#define SDL_UTILS_CONFIG_TEXTCONTAINERCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <string>
// Third-party headers
// Own headers
// Forward Declarations

struct FontCfg{
    std::string location;
    int32_t fontSize=0;
};

struct TextContainerCfg{
    std::unordered_map<int32_t, FontCfg> fontConfigs; 
};

#endif // SDL_UTILS_CONFIG_TEXTCONTAINERCFG_H_