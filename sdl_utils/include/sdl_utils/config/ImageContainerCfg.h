#ifndef SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_
#define SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <vector>
// Third-party headers
// Own headers
#include "utils/drawing/Rectangle.h"
// Forward Declarations


struct ImageCfg{
    // const char* location;
    std::string location;
    std::vector<Rectangle> frames;
};


struct ImageContainerCfg{
    std::unordered_map<int32_t,ImageCfg> imageConfigs;
};

#endif // SDL_UTILS_CONFIG_IMAGECONTAINERCFG_H_