// Corresponding header
#include "manager_utils/managers/DrawMgr.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/RsrcMgr.h"
#include "manager_utils/config/DrawMgrCfg.h"

DrawMgr* gDrawMgr=nullptr;

int32_t DrawMgr::init(const DrawMgrCfg& cfg){

    if(EXIT_SUCCESS!=_window.init(cfg.windowCfg)){
        std::cerr<<"_window.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_renderer.init(_window.getWindow())){
        std::cerr<<"_renderer.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _maxFrames=cfg.maxFrameRate;

    return EXIT_SUCCESS;
}


void DrawMgr::deinit(){
    _renderer.deinit();
    _window.deinit();
}

void DrawMgr::process(){

// emptry
}

void DrawMgr::clearScreen(){
    _renderer.clearScreen();
}

void DrawMgr::finishFrame(){
    _renderer.finishFrame();
}

void DrawMgr::addDrawCmd(const DrawParams& drawParams) const{
    SDL_Texture* texture=getTextureInternal(drawParams);
    _renderer.renderTexture(texture,drawParams);
}

void DrawMgr::setWidgetBlendMode(const DrawParams& drawParams, BlendMode blendMode){
    SDL_Texture* texture=getTextureInternal(drawParams);
    _renderer.setWidgetBlendMode(texture,blendMode);
}

void DrawMgr::setWidgetOpacity(const DrawParams& drawParams, int32_t opacity){
    if(WidgetType::IMAGE==drawParams.widgetType){
        return;
    }
    SDL_Texture* texture=getTextureInternal(drawParams);
    _renderer.setWidgetOpacity(texture,opacity);
}

int64_t DrawMgr::getMaxFrameRate() const {
    return _maxFrames;
}

int32_t DrawMgr::getActiveWidgets() const{
    return _renderer.getActiveWidgets();
}

int32_t DrawMgr::clearCurrentRendererTarget(const Color& color){
    return _renderer.clearCurrentRendererTarget(color);
}

int32_t DrawMgr::setRendererTarget(int32_t fboId){
    SDL_Texture* fboTexture=gRsrcMgr->getFboTexture(fboId);
    return _renderer.setRendererTarget(fboTexture);
}

int32_t DrawMgr::resetRendererTarget(){
    return _renderer.resetRendererTarget();
}

int32_t DrawMgr::lockRenderer(){
    return _renderer.lockRenderer();
}

int32_t DrawMgr::unlockRenderer(){
    return _renderer.unlockRenderer();
}

SDL_Texture* DrawMgr::getTextureInternal(const DrawParams& drawParams) const {
    if(WidgetType::IMAGE==drawParams.widgetType){
        return gRsrcMgr->getImageTexture(drawParams.rsrcId);
    } else if (WidgetType::TEXT==drawParams.widgetType){
        return gRsrcMgr->getTextTexture(drawParams.textId);
    } else if(WidgetType::FBO==drawParams.widgetType){
        return gRsrcMgr->getFboTexture(drawParams.fboId);
    } else {
        std::cerr<<"Error, received unsupported WidgetType: "
        <<static_cast<int32_t>(drawParams.widgetType)<<" for rsrcId: "
        <<drawParams.rsrcId<<std::endl;      
    }
    return nullptr;
}