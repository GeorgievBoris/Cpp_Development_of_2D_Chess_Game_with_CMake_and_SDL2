#ifndef SDL_UTILS_INCLUDE_SDL_UTILS_CONTAINERS_FBOCONTAINER_H_
#define SDL_UTILS_INCLUDE_SDL_UTILS_CONTAINERS_FBOCONTAINER_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
// Forward Declarations
struct SDL_Texture;

class FboContainer{
public:
    void createFbo(int32_t fboWidth, int32_t fboHeight, int32_t& outFboId);
    void unloadFbo(int32_t fboId);
    SDL_Texture* getFboTexture(int32_t fboId) const;

protected:
    int32_t init();
    void deinit();

private:
    void occupyFreeSlotIndex(int32_t& outIdx, SDL_Texture* texture);
    void freeSlotIndex(int32_t index);
    
    std::vector<SDL_Texture*> _textures;

};


#endif // SDL_UTILS_INCLUDE_SDL_UTILS_CONTAINERS_FBOCONTAINER_H_