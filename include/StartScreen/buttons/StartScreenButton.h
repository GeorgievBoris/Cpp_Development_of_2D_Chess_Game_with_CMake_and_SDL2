#ifndef INCLUDE_STARTSCREEN_BUTTONS_STARTSCREENBUTTON_H_
#define INCLUDE_STARTSCREEN_BUTTONS_STARTSCREENBUTTON_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/input/ButtonBase.h"
#include "StartScreen/buttons/config/StartScreenButtonCfg.h"
// Forward Declarations

// NOTE: "StartScreenButton.h" is NOT created by Zhivko!

class StartScreenButton : public ButtonBase{
public:
    int32_t init(const StartScreenButtonCfg& cfg);
    void deinit();
    void handleEvent(const InputEvent& e) final;
    void draw() const;
    void activate();
    void deactivate();
private:
    StartScreenButtonCfg _cfg;
};


#endif // INCLUDE_STARTSCREEN_BUTTONS_CONFIG_STARTSCREENBUTTON_H_