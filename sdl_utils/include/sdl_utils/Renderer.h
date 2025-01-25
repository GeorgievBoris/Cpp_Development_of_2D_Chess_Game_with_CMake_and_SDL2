#ifndef SDL_UTILS_RENDERER_H_
#define SDL_UTILS_RENDERER_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
// Forward Declarations
struct SDL_Renderer;
struct SDL_Texture; // this is the GPU primitive that is used to upload data on the GPU !!! SDL_Surface is a CPU primitive !!!
struct SDL_Window;

class Renderer{
public:

    Renderer()=default;
    // Forbid copy and move c-tors and assignment operators
    Renderer(const Renderer& other)=delete;
    Renderer& operator=(const Renderer& other)=delete;

    Renderer(Renderer&& other)=delete;
    Renderer& operator=(Renderer&& other)=delete;

    int32_t init(SDL_Window* window);
    void deinit();
    void clearScreen();
    void finishFrame();
    void renderTexture(SDL_Texture* texture, const DrawParams& drawParams);

    void setWidgetBlendMode(SDL_Texture* texture, BlendMode blendMode);
    void setWidgetOpacity(SDL_Texture* texture, int32_t opacity);


private:
    void drawImage(const DrawParams& drawParams, SDL_Texture* texture);
    void drawText(const DrawParams& drawParams, SDL_Texture* texture);
    void drawTextureInternal(const DrawParams& drawParams, SDL_Texture* texture);
    // each window that we create, can have only one "SDL_Renderer" assigned to it !!!
    // an image that is loaded in one Renderer, CANNOT be used in another Renderer !!!
    SDL_Renderer* _sdlRenderer=nullptr;
};



#endif // SDL_UTILS_RENDERER_H_