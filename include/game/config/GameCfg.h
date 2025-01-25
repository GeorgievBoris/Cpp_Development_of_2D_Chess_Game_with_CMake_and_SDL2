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

    int32_t textFontId;
};

#endif // GAME_CONFIG_GAMECFG_H_