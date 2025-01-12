#ifndef SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_
#define SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
// Forward Declarations


struct ImageCfg{
    // const char* location;
    std::string location;
    int32_t width=0;
    int32_t height=0;
};


struct ImageContainerCfg{
    std::unordered_map<int32_t,ImageCfg> imageConfigs;
};

#endif // SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_