#ifndef GAME_CONFIG_GAMECFG_H_
#define GAME_CONFIG_GAMECFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <string>
// Third-party headers
// Own headers
// Foward Declarations

struct GameCfg{
    int32_t runningGirlRsrcId;
    int32_t wheelRsrcId;
    int32_t blackBackgroundRsrcId;
    int32_t startButtonRsrcId;
    int32_t stopButtonRsrcId;
    
    int32_t textFontId;

    int32_t wheelRotAnimTimerId;
    int32_t wheelScaleAnimTimerId;
    int32_t girlMoveTimerId;
};

#endif // GAME_CONFIG_GAMECFG_H_