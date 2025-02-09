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
    int32_t chessBoardRsrcId;
    int32_t whitePiecesRsrcId;
    int32_t blackPiecesRsrcId;
    int32_t targetRsrcId;
    
    // int32_t textFontId;

    int32_t blinkTargetTimerId;
};

#endif // GAME_CONFIG_GAMECFG_H_