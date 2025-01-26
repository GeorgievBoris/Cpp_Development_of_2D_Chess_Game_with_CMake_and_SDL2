#ifndef INCLUDE_GAME_ENTITIES_WHEEL_H_
#define INCLUDE_GAME_ENTITIES_WHEEL_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "manager_utils/time/TimerClient.h"
// Forward Declarations
class InputEvent;

class Wheel : public TimerClient {
public:
    ~Wheel();
    int32_t init(int32_t wheelRsrcId, int32_t rotAnimTimerId, int32_t scaleAnimTimerId);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);
    void startAnimation();
    void stopAnimation();
private:
    void processRotAnim();
    void processScaleAnim();
    void onTimeout(int32_t timerId) final;

    bool _isAnimActive=false;
    Image _wheelImg;
    int32_t _rotAnimTimerId=-1;
    int32_t _scaleAnimTimerId=-1;
    int32_t _scaleSteps=50;
    bool _isShrinking=true;
};


#endif // INCLUDE_GAME_ENTITIES_WHEEL_H_