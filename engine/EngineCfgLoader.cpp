// Corresponding headers
#include "EngineCfgLoader.h"
// C system headers
// C++ system headers
#include <iostream>
#include <string>
// Third-party headers
// Own headers
#include "common/CommonDefines.h"

// constants
static constexpr auto WINDOW_WIDTH=640;
static constexpr auto WINDOW_HEIGHT=480;
static constexpr auto WINDOW_NAME="Hardware_Rendering";

static constexpr auto PRESS_KEYS_WIDTH=640;
static constexpr auto PRESS_KEYS_HEIGHT=480;

static constexpr auto LAYER_2_IMG_WIDTH_HEIGHT=150;

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

static void populateGameCfg(GameCfg& cfg){
    cfg.layer2RsrcId=TextureId::LAYER_2;
    cfg.pressKeysRsrcId=TextureId::PRESS_KEYS;
}


static void populateImageContainerCfg(ImageContainerCfg& cfg){

    ImageCfg imageCfg;
    imageCfg.location=getFilePath("resources/press_keys.png");
    imageCfg.height=PRESS_KEYS_HEIGHT;
    imageCfg.width=PRESS_KEYS_WIDTH;
    // cfg.imageConfigs.insert({TextureId::UP,imageCfg}); // possible - an alternative to std::make_pair
    cfg.imageConfigs.insert(std::make_pair(TextureId::PRESS_KEYS,imageCfg));

    imageCfg.location=getFilePath("resources/layer_2.png");
    imageCfg.height=LAYER_2_IMG_WIDTH_HEIGHT;
    imageCfg.width=LAYER_2_IMG_WIDTH_HEIGHT;
    cfg.imageConfigs.insert(std::make_pair(TextureId::LAYER_2,imageCfg));
}

EngineCfg EngineCfgLoader::loadCfg(){
    EngineCfg cfg;
    populateMonitorCfg(cfg.windowCfg);
    populateImageContainerCfg(cfg.imgContainerCfg);
    populateGameCfg(cfg.gameCfg);

    return cfg;
}