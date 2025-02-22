#ifndef SDL_UTILS_RENDERER_H_
#define SDL_UTILS_RENDERER_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
#include "utils/drawing/Color.h"
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
    void renderTexture(SDL_Texture* texture, const DrawParams& drawParams) const;

    void setWidgetBlendMode(SDL_Texture* texture, BlendMode blendMode);
    void setWidgetOpacity(SDL_Texture* texture, int32_t opacity);

    int32_t getActiveWidgets() const;

    int32_t clearCurrentRendererTarget(const Color& color);
    int32_t setRendererTarget(SDL_Texture* target);
    int32_t resetRendererTarget();

    int32_t lockRenderer();
    int32_t unlockRenderer();
private:
    void drawImage(const DrawParams& drawParams, SDL_Texture* texture) const;
    void drawTextureInternal(const DrawParams& drawParams, SDL_Texture* texture) const;
    // each window that we create, can have only one "SDL_Renderer" assigned to it !!!
    // an image that is loaded in one Renderer, CANNOT be used in another Renderer !!!
    SDL_Renderer* _sdlRenderer=nullptr;
    mutable int32_t _activeWidgets=0; // NOTE: write this down in the book !!!
    Color _clearColor=Colors::BLUE;
    // allows or forbids changing to different renderer target
    // if "true" this means that the default target points at the back buffer...
    // if "false" this means that we have exposed its functionality AND temporarily...
    // ... have given the renderer target to some Frame Buffer Object...so it draws, draws...
    // ... and finally when we are ready we must lock it
    bool _isRendererLocked=true; 
};



#endif // SDL_UTILS_RENDERER_H_