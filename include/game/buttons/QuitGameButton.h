#ifndef INCLUDE_GAME_BUTTONS_QUITGAMEBUTTON_H_
#define INCLUDE_GAME_BUTTONS_QUITGAMEBUTTON_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <functional>
// Third-party headers
// Own headers
#include "manager_utils/input/ButtonBase.h"
// Forward Declarations
class Fbo;
// class function;

// "QuitGameButton.h" is NOT added by Zhivko !

class QuitGameButton : public ButtonBase {
public:
    int32_t init(int32_t rsrcId, const std::function<void()>& showStartScreenCallBack,
                                 const std::function<void()>& gameLogicAndWinnerAnimatorCallBack);
    void handleEvent(const InputEvent& e) final;

    void restart();
    // void drawOnFbo(Fbo& fbo) const;
private:
    std::function<void()> _showStartScreenCallBack;
    std::function<void()> _gameLogicAndWinnerAnimatorCallBack;
};


#endif // ! INCLUDE_GAME_BUTTONS_QUITGAMEBUTTON_H_