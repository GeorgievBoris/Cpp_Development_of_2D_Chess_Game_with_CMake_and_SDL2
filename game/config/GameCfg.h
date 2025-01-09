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

enum GameImages{
    UP,DOWN,LEFT,RIGHT,PRESS_KEYS,LAYER_2,COUNT
};

struct GameCfg{
    std::unordered_map<GameImages,std::string> imgPaths;
};

#endif // GAME_CONFIG_GAMECFG_H_