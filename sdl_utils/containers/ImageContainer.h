#ifndef SDL_UTILS_CONTAINERS_IMAGECONTAINER_H_
#define SDL_UTILS_CONTAINERS_IMAGECONTAINER_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
#include "utils/drawing/Rectangle.h"
#include "sdl_utils/config/ImageContainerCfg.h"
// Forward Declarations
struct SDL_Texture;

class ImageContainer {
public:
    int32_t init(const ImageContainerCfg& cfg);
    void deinit();
    SDL_Texture* getImageTexture(int32_t rsrcId) const;
    Rectangle getImageFrame(int32_t rsrcId) const;
private:
    int32_t loadSingleResource(const ImageCfg& resCfg, int32_t rsrcId);

    // the textures we will be drawing
    std::unordered_map<int32_t, SDL_Texture*> _textures;
    std::unordered_map<int32_t, Rectangle> _textureFrames;


};



#endif // SDL_UTILS_CONTAINERS_IMAGECONTAINER_H_