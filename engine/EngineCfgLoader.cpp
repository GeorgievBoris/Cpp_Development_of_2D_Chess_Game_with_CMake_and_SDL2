// Corresponding headers
#include "EngineCfgLoader.h"
// C system headers
// C++ system headers
#include <iostream>
#include <string>
// Third-party headers
// Own headers

// constants
static constexpr auto WINDOW_WIDTH=640;
static constexpr auto WINDOW_HEIGHT=480;
static constexpr auto WINDOW_NAME="Hardware_Rendering";

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

    cfg.imgPaths[UP]=getFilePath("resources/up.png");
    cfg.imgPaths[DOWN]=getFilePath("resources/down.png");
    cfg.imgPaths[LEFT]=getFilePath("resources/left.png");
    cfg.imgPaths[RIGHT]=getFilePath("resources/right.png");
    cfg.imgPaths[PRESS_KEYS]=getFilePath("resources/press_keys.png");
    cfg.imgPaths[LAYER_2]=getFilePath("resources/layer_2.png");

}

EngineCfg EngineCfgLoader::loadCfg(){
    EngineCfg cfg;
    populateMonitorCfg(cfg.windowCfg);
    populateGameCfg(cfg.gameCfg);

    return cfg;
}