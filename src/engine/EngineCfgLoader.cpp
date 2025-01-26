// Corresponding headers
#include "engine/EngineCfgLoader.h"
// C system headers
// C++ system headers
#include <iostream>
#include <string>
// Third-party headers
// Own headers
#include "common/CommonDefines.h"

// constants
static constexpr auto WINDOW_WIDTH=1024;
static constexpr auto WINDOW_HEIGHT=800;
static constexpr auto WINDOW_NAME="Hardware_Rendering";

static constexpr auto RUNNING_GIRL_FRAMES=6;
static constexpr auto RUNNING_GIRL_IMG_WIDTH=256;
static constexpr auto RUNNING_GIRL_IMG_HEIGHT=220;

static constexpr auto BUTTON_FRAMES=3;
static constexpr auto BUTTON_WIDTH=150;
static constexpr auto BUTTON_HEIGHT=50;

static constexpr auto BLACK_BACKGROUND_IMG_WIDTH=1024;
static constexpr auto BLACK_BACKGROUND_IMG_HEIGHT=800;

static constexpr auto WHEEL_IMG_WIDTH_HEIGHT=695;

static constexpr auto ANGELINE_VINTAGE_40_FONT_SIZE=80;

static constexpr auto MAX_FRAMERATE=30;

static std::string getFilePath(const std::string& relativePath) {
    
    // NDEBUG == true -> means that we are in Release mode !!!
    #ifdef RELEASE_BUILD
        return relativePath;
    #else
        return std::string("../")+relativePath;
    #endif
}

static void populateMonitorCfg(MonitorWindowCfg& outCfg){
    outCfg.windowName=WINDOW_NAME;
    // outCfg.windowPos;
    outCfg.windowHeight=WINDOW_HEIGHT;
    outCfg.windowWidth=WINDOW_WIDTH;
    outCfg.windowFlags=WINDOW_SHOWN;
}

static void populateDrawMgrCfg(DrawMgrCfg& cfg){
    populateMonitorCfg(cfg.windowCfg);
    cfg.maxFrameRate=MAX_FRAMERATE;

}

static void populateImageContainerCfg(ImageContainerCfg& cfg){

    ImageCfg imageCfg;
    imageCfg.location=getFilePath("resources/p/sprites/running_girl_small.png");
    for(auto i=0;i<RUNNING_GIRL_FRAMES;++i){
        imageCfg.frames.emplace_back(i*RUNNING_GIRL_IMG_WIDTH,0,256,220); // x, y, w, h
    }
    // cfg.imageConfigs.insert({TextureId::RUNNING_GIRL,imageCfg}); // possible - an alternative to std::make_pair
    // cfg.imageConfigs.insert(std::make_pair(TextureId::RUNNING_GIRL,imageCfg));
    cfg.imageConfigs.emplace(TextureId::RUNNING_GIRL,imageCfg);
    imageCfg.frames.clear();

    imageCfg.location=getFilePath("resources/p/wheel.png");
    imageCfg.frames.emplace_back(0,0,WHEEL_IMG_WIDTH_HEIGHT,WHEEL_IMG_WIDTH_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::WHEEL,imageCfg);
    imageCfg.frames.clear(); 

    imageCfg.location=getFilePath("resources/p/black_background.png");
    imageCfg.frames.emplace_back(0,0,BLACK_BACKGROUND_IMG_WIDTH,BLACK_BACKGROUND_IMG_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::BLACK_BACKGROUND,imageCfg);
    imageCfg.frames.clear();

    constexpr int32_t buttonsCnt=2;
    const std::string buttonPaths[buttonsCnt]={"resources/p/buttons/button_start.png", "resources/p/buttons/button_stop.png"};
    constexpr TextureId::ResourceId buttonRsrcIds[buttonsCnt]={TextureId::START_BUTTON, TextureId::STOP_BUTTON};
    for(int32_t i=0;i<buttonsCnt;++i){
        imageCfg.location=getFilePath(buttonPaths[i]);
        for(int32_t frameId=0;frameId<BUTTON_FRAMES;++frameId){
            imageCfg.frames.emplace_back(frameId*BUTTON_WIDTH,0,BUTTON_WIDTH,BUTTON_HEIGHT);
        }
        cfg.imageConfigs.emplace(buttonRsrcIds[i],imageCfg);
        imageCfg.frames.clear(); // important-clean it for the next element of the "unordered_map" that is going to use it!!!
    }
}

static void populateTextContainerCfg(TextContainerCfg& cfg){
    FontCfg fontCfg;

    fontCfg.location=getFilePath("resources/f/AngelineVintage.ttf");
    fontCfg.fontSize=ANGELINE_VINTAGE_40_FONT_SIZE;

    cfg.fontConfigs.insert(std::make_pair(FontId::ANGELINE_VINTAGE_40,fontCfg));
}

static void populateRsrcMgrCfg(RsrcMgrCfg& cfg){
    populateImageContainerCfg(cfg.imgContainerCfg);
    populateTextContainerCfg(cfg.textContainerCfg);
}

static void populateManagerHandlerCfg(ManagerHandlerCfg& cfg){
    populateDrawMgrCfg(cfg.drawMgrCfg);
    populateRsrcMgrCfg(cfg.rsrcMgrCfg);
}

static void populateGameCfg(GameCfg& cfg){
    cfg.runningGirlRsrcId=TextureId::RUNNING_GIRL;
    cfg.wheelRsrcId=TextureId::WHEEL;
    cfg.blackBackgroundRsrcId=TextureId::BLACK_BACKGROUND;
    cfg.startButtonRsrcId=TextureId::START_BUTTON;
    cfg.stopButtonRsrcId=TextureId::STOP_BUTTON;

    cfg.textFontId=FontId::ANGELINE_VINTAGE_40;

    cfg.wheelRotAnimTimerId=TimerId::WHEEL_ROT_ANIM_TIMER_ID;
    cfg.wheelScaleAnimTimerId=TimerId::WHEEL_SCALE_TIMER_ID;
    cfg.girlMoveTimerId=TimerId::GIRL_MOVE_TIMER_ID;
}

EngineCfg EngineCfgLoader::loadCfg(){
    EngineCfg cfg;
    populateManagerHandlerCfg(cfg.managerHandlerCfg);
    populateGameCfg(cfg.gameCfg);

    return cfg;
}