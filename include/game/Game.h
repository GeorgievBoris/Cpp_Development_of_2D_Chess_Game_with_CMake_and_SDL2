#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
#include "game/config/GameCfg.h"
#include "manager_utils/drawing/Image.h"
#include "manager_utils/drawing/Text.h"
#include "game/entities/Hero.h"
#include "game/entities/Wheel.h"
// Forward Declarations
class InputEvent;

class Game {
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);

private:

    void setMousePosText(const Point& mousePos);

    Hero _hero;
    Wheel _wheel;
    Image _blackBackgroundImg; // added by Zhivko at the end of the lecture for demonstration purposes
    Text _mousePosText;
};


#endif // GAME_GAME_H_