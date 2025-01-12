#ifndef SDL_UTILS_CONTAINERS_TEXTCONTAINER_H_
#define SDL_UTILS_CONTAINERS_TEXTCONTAINER_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <unordered_map>
// Third-party headers
// Own headers
#include "sdl_utils/config/TextContainerCfg.h"
// Forward Declarations
typedef struct _TTF_Font TTF_Font;
struct SDL_Texture;
class Color;

class TextContainer{
public:
    int32_t init(const TextContainerCfg& cfg);
    void deinit();

    void createText(const char* text, const Color& color, int32_t fontId,
                            int32_t& outTextId, int32_t& outTextWidth,
                            int32_t& outTextHeight);
    
    void reloadText(const char* text, const Color& color, int32_t fontId,
                            int32_t textId, int32_t& outTextWidth,
                            int32_t& outTextHeight);
    
    void unloadText(int32_t textId);
    SDL_Texture* getTextTexture(int32_t textId) const;
private:
    void occupyFreeSlotIndex(int32_t& outIdx, SDL_Texture* texture);
    // the textures we will be drawing
    std::vector<SDL_Texture*> _textures;
    std::unordered_map<int32_t, TTF_Font*> _fonts;
};


#endif // SDL_UTILS_CONTAINERS_TEXTCONTAINER_H_