// Corresponding header
#include "sdl_utils/Renderer.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
#include <SDL_render.h>
#include <SDL_hints.h>
// Own headers
#include "sdl_utils/Texture.h"

int32_t Renderer::init(SDL_Window* window){

    // implement renderer hints to enable linear interpolation
    // renderer hints must be enabled before the renderer is actually created !!!

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){ // read the description in "SDL_hints.h"
        std::cerr<<"Warning: Linear texture filtering not enabled!"
        "SDL_SetHint() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // SDL_RENDERER_SOFTWARE // allows to use the CPU for rendering - added for legacy reasons

    // SDL_RENDERER_PRESENTVSYNC // enables VSYNC -> read the notes (Zhivko's explanation) in the notebook
    // VSYNC - we do not sleep in our application, but the speed of the frame / loop obeys the refresh rate of the monitor
    
    // SDL_RENDERER_TARGETTEXTURE // we can create runtime images via this option...
    // load four normal images and unite them in one big image - can do all of this runtime...
    // this one big image that we get can be manipulated, moved, rotated, animated, do whatever we want with it runtime !!!

    const auto unspecifiedDriverId=-1; // -1 means that SDL decides for itself which back-end graphical library to use

    _sdlRenderer=SDL_CreateRenderer(window,unspecifiedDriverId,
                        SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    if(nullptr==_sdlRenderer){
        std::cerr<<"SDL_CreateRenderer() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // when we say "clear" to the buffer, the buffer just "paints" or "smudges" all the pixels with some information...
    // ... and this information is actually a particular color...so set the color via SDL_SetRenderDrawColor()
    // Later in the course / lectures we will create a "class Color" to have easy access to different colors
    if(EXIT_SUCCESS!=SDL_SetRenderDrawColor(_sdlRenderer,
                        _clearColor.rgba.r, _clearColor.rgba.g,
                        _clearColor.rgba.b, _clearColor.rgba.a)){
        std::cerr<<"SDL_SetRenderDrawColor() failed. Reason: "<<SDL_GetError()<<std::endl;
        return EXIT_FAILURE;
    }

    // a global variable in "Texture.cpp" is set, BUT this is done in a smart and clean way !!!
    Texture::setRenderer(_sdlRenderer);

    return EXIT_SUCCESS;
}

void Renderer::deinit(){
    if(nullptr!=_sdlRenderer){
        SDL_DestroyRenderer(_sdlRenderer);
        _sdlRenderer=nullptr;
    }
}

void Renderer::clearScreen(){
    if(EXIT_SUCCESS!=Texture::clearCurrentRendererTarget(_clearColor)){
        std::cerr<<"Texture::clearCurrentRendererTarget() failed"<<std::endl;
    }
}

void Renderer::finishFrame(){
    // SDL_RenderPresent() does two things: 1) says swap to the pointers and 2) updates the screen
    
    if(!_isRendererLocked){
        std::cerr<<"WARNING, Renderer was left unlocked. Self-locking renderer."
                    "Some FBO will be in a broken state "<<std::endl;
        _isRendererLocked=true;
        resetRendererTarget();
    }
    SDL_RenderPresent(_sdlRenderer);
    _activeWidgets=0;

}

void Renderer::renderTexture(SDL_Texture* texture, const DrawParams& drawParams) const{
    if(WidgetType::IMAGE==drawParams.widgetType){
        drawImage(drawParams,texture);
    } else if(WidgetType::TEXT==drawParams.widgetType ||
              WidgetType::FBO==drawParams.widgetType){
        drawTextureInternal(drawParams,texture);
    } else {
        std::cerr<<"Error, received unsupported WidgetType: "
                <<static_cast<int32_t>(drawParams.widgetType)
                <<" for rsrcId: "<<drawParams.rsrcId<<std::endl;
        --_activeWidgets;
    }
    ++_activeWidgets;
}

void Renderer::setWidgetBlendMode(SDL_Texture* texture, BlendMode blendMode){
    if(EXIT_SUCCESS!=Texture::setBlendModeTexture(texture,blendMode)){
        std::cerr<<"Texture::setBlendModeTexture() failed"<<std::endl;
    }
}

void Renderer::setWidgetOpacity(SDL_Texture* texture, int32_t opacity){
    if(EXIT_SUCCESS!=Texture::setAlphaTexture(texture,opacity)){
        std::cerr<<"Texture::setAlphaTexture() failed"<<std::endl;
    }
}

int32_t Renderer::getActiveWidgets() const {
    return _activeWidgets;
}

int32_t Renderer::clearCurrentRendererTarget(const Color& color){
    return Texture::clearCurrentRendererTarget(color);
}

int32_t Renderer::setRendererTarget(SDL_Texture* target){
    return Texture::setRendererTarget(target);
}

int32_t Renderer::resetRendererTarget(){
    return Texture::resetRendererTarget();
}

int32_t Renderer::lockRenderer(){

    if(_isRendererLocked){
        std::cerr<<"Error, renderer was already locked"<<std::endl;
        return EXIT_FAILURE;
    }
    _isRendererLocked=true;
    return EXIT_SUCCESS;
}

int32_t Renderer::unlockRenderer(){
    if(_isRendererLocked){
        _isRendererLocked=false;
        return EXIT_SUCCESS;
    }
    std::cerr<<"Error, renderer was not locked in the first place"<<std::endl;
    return EXIT_FAILURE;
}

void Renderer::drawImage(const DrawParams& drawParams, SDL_Texture* texture) const{
    if(FULL_OPACITY==drawParams.opacity){
        drawTextureInternal(drawParams,texture);
    } else {
        if(EXIT_SUCCESS!=Texture::setAlphaTexture(texture,drawParams.opacity)){
            std::cerr<<"Texture::setAlphaTexture() failed for rsrcId: "<<drawParams.rsrcId<<std::endl;
        }
        drawTextureInternal(drawParams,texture);

        if(EXIT_SUCCESS!=Texture::setAlphaTexture(texture,FULL_OPACITY)){
            std::cerr<<"Texture::setAlphaTexture() failed for rsrcId: "<<drawParams.rsrcId<<std::endl;
        }
    }  
}

void Renderer::drawTextureInternal(const DrawParams& drawParams, SDL_Texture* texture) const{
    // first "nullptr" means take the entire image and second "nullptr" means to draw it on the entire window
    // const int32_t err=SDL_RenderCopy(_sdlRenderer,texture,nullptr,nullptr);
       
    const SDL_Rect destRect={.x=drawParams.pos.x, .y=drawParams.pos.y, .w=drawParams.width, .h=drawParams.height};
    // reinterpret_cast<>() is equivalent to C-style cast !!!
    const SDL_Rect* sourceRect=reinterpret_cast<const SDL_Rect*>(&drawParams.frameRect);

    const SDL_RendererFlip sdlRenderFlip=static_cast<const SDL_RendererFlip>(drawParams.flipType);
    const SDL_Point* sdlCenterPoint=reinterpret_cast<const SDL_Point*>(&drawParams.rotationCenter);
    const int32_t err=SDL_RenderCopyEx(_sdlRenderer,texture,sourceRect,&destRect,
                                                drawParams.rotationAngle,sdlCenterPoint,sdlRenderFlip);

    if(EXIT_SUCCESS!=err){
        std::cerr<<"SDL_RenderCopy() failed for rsrcId: "<<drawParams.rsrcId<<". Reason: "<<SDL_GetError()<<std::endl;
    }
}