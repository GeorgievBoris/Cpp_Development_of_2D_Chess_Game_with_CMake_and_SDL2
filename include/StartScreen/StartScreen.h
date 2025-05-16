#ifndef INCLUDE_STARTSCREEN_STARTSCREEN_H_
#define INCLUDE_STARTSCREEN_STARTSCREEN_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "StartScreen/buttons/StartScreenButton.h"
#include "StartScreen/defines/StartScreenDefines.h"
#include "manager_utils/drawing/Image.h"
#include "manager_utils/time/TimerClient.h"
// Forward Declarations
struct StartScreenCfg;
class InputEvent;

// NOTE: "StartScreen.h" is NOT created by Zhivko!

class StartScreen : public TimerClient{
public:
    int32_t init(const StartScreenCfg& cfg, const std::function<void()>& hideGameCallBack,
                                            const std::function<void()>& startGameCallBack,
                                            const std::function<void()>& startPlayersTimerCallBack);
    void deinit();
    void draw() const;
    void handleEvent(const InputEvent& e);
  
    bool shouldExit() const;
    
    void show();
    void hide();
private:
    void activate();
    void deactivate();  
    void setShouldExit(bool shouldExit=true);
    void onTimeout(int32_t timerId) final; // put it private here, because it is PUBLIC in the BaseClass
    void changeOpacity(int32_t opacity);
    // void getOpacity();

    enum InternalDefines{
        BUTTONS_COUNT=2,
        BACKGROUND_COUNT=1,
        TOTAL_ENTITIES_COUNT=BUTTONS_COUNT+BACKGROUND_COUNT
    };

    std::function<void()> _hideGameCallBack;
    std::function<void()> _startGameCallBack;
    std::function<void()> _startPlayersTimerCallBack;

    bool _shouldExit=false;

    Image _background;
    StartScreenButton _screenBtns[BUTTONS_COUNT];

    int32_t _backgroundRsrcId=INVALID_RSRC_ID;
    int32_t _timerId=INVALID_RSRC_ID; // set also to INVALID_RSRC_ID?
    int32_t _deltaOpacity{};
    int32_t _currTotalOpacity=TOTAL_ENTITIES_COUNT*FULL_OPACITY;
    const int32_t _maxTotalOpacity=FULL_OPACITY*TOTAL_ENTITIES_COUNT;
};

#endif // INCLUDE_STARTSCREEN_STARTSCREEN_H_