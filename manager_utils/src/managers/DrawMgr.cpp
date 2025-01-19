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

    _maxFrames=cfg.maxFrameRate; // probably Zhivko forgot to perform this assignment in his code

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

void DrawMgr::addDrawCmd(const DrawParams& drawParams){
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

SDL_Texture* DrawMgr::getTextureInternal(const DrawParams& drawParams) const {
    if(WidgetType::IMAGE==drawParams.widgetType){
        return gRsrcMgr->getImageTexture(drawParams.rsrcId);
    } else if (WidgetType::TEXT==drawParams.widgetType){
        return gRsrcMgr->getTextTexture(drawParams.textId);
    } else {
        std::cerr<<"Error, received unsupported WidgetType: "
        <<static_cast<int32_t>(drawParams.widgetType)<<" for rsrcId: "
        <<drawParams.rsrcId<<std::endl;      
    }
    return nullptr;
}