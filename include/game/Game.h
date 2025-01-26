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
#include "game/entities/Hero.h"
#include "game/entities/Wheel.h"
#include "game/proxies/GameProxy.h"
#include "game/buttons/WheelButton.h"
// Forward Declarations
class InputEvent;

class Game : public GameProxy{
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);
private:

    // the beauty is that we can inherit "onButtonPressed()" as private, so it cannot be called via the API of the "Game" class
    // ... BUT via an interface, it can be used/called because it is public in the base class "GameProxy" !!!
    void onButtonPressed(int32_t buttonId) final;

    enum Internals{
        WHEEL_START_BTN_IDX,
        WHEEL_STOP_BTN_IDX,
        WHEEL_BTNS_COUNT
    };

    Hero _hero;
    Wheel _wheel;
    Image _blackBackgroundImg; // added by Zhivko at the end of the lecture for demonstration purposes
    WheelButton _wheelBtns[WHEEL_BTNS_COUNT]; // NOTE: a zero-initializer can be used here
};


#endif // GAME_GAME_H_