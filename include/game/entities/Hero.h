#ifndef INCLUDE_GAME_ENTITIES_HERO_H_
#define INCLUDE_GAME_ENTITIES_HERO_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "manager_utils/time/TimerClient.h"
// Forward Declarations
struct InputEvent;

class Hero : public TimerClient {
public:
    int32_t init(int32_t heroRsrcId, int32_t moveTimerId);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);

    void startMoveAnim();
private:
    void onTimeout(int32_t timerId) final;
    void processMoveAnim();
    
    Image _heroImg;
    int32_t _moveTimerId=-1;
    int32_t _moveSteps=50;

};

#endif // INCLUDE_GAME_ENTITIES_HERO_H_