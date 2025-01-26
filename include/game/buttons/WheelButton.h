#ifndef INCLUDE_GAME_BUTTONS_WHEELBUTTON_H_
#define INCLUDE_GAME_BUTTONS_WHEELBUTTON_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/input/ButtonBase.h"
// Forward Declarations
class GameProxy;

class WheelButton : public ButtonBase{
public:
    void handleEvent(const InputEvent& e) final;
    int32_t init(GameProxy* gameProxy, int32_t buttonId);
private:
    GameProxy* _gameProxy=nullptr;
    int32_t _buttonId{0};
};

#endif // INCLUDE_GAME_BUTTONS_WHEELBUTTON_H_