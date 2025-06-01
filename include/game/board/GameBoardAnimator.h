#ifndef INCLUDE_GAME_BOARD_GAMEBOARDANIMATOR_H_
#define INCLUDE_GAME_BOARD_GAMEBOARDANIMATOR_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "manager_utils/time/TimerClient.h"
// Forward Declarations
class GameProxy;
class Fbo;

// TODO: write this down in the book...
// Zhivko: Two types of animators exist in general...
// 1) The object that reprsents the animator may have a private member/field of type "Image"...
// ... so the animator creates this "Image" object, keeps it only for itself and modifies it as required
// 2) The second type of animators accepts a pointer to "Image" as a private parameter.
// The Animator can modify the Image object, BUT the image object itself lives outside of...
// ... the Animator class. The Animator is not an owner of the Image object.
// The benefit is that many different animators can exists and can access and modify the Image...
// ... (rotate, flip, etc...)

class GameBoardAnimator : public TimerClient{
public:
    ~GameBoardAnimator();
    int32_t init(GameProxy* gameProxy, Fbo* gameFbo, int32_t gameFboRotTimerId);
    void startAnim(int32_t playerId);
    bool isActive() const;
    void restart();
    int32_t getTargetRotation()const; // this method is NOT added by Zhivko !
private:
    void onTimeout(int32_t timerId) final;
    void processRotAnim();

    GameProxy* _gameProxy=nullptr;
    Fbo* _gameFbo=nullptr;
    int32_t _currRotation{0};
    int32_t _targetRotation{0};
    int32_t _gameFboRotTimerId=-1;
    WidgetFlip _targetFlipType=WidgetFlip::NONE;
    bool _isActive=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_BOARD_GAMEBOARDANIMATOR_H_