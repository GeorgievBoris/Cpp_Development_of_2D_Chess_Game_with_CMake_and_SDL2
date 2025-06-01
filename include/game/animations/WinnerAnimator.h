#ifndef INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_
#define INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_

// C system headers
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Text.h"
#include "manager_utils/time/TimerClient.h"
// Forward Declarations
class PieceHandlerProxy;

// NOTE: "WinnerAnimator.h" is NOT created by Zhivko!

class WinnerAnimator : public TimerClient {
public:
    int32_t init(PieceHandlerProxy* pieceHandlerProxy, int32_t timerId,
                int32_t rsrcId, int32_t fontId);
    void draw() const;
    void activate(int32_t playerId);
    void deactivate();
    bool isActive() const;
    void restart();
private:
    class Generator{
    public:
        Generator(){_current=0;}
        ~Generator()=default;
        int operator()(){return (_current+=10);}
    private:
        int _current;
    };

    void onTimeout(int32_t timerId) final;
    void setRotation();
    void setWinText();

    int32_t _blinkWinnerTextTimerId;
    int32_t _winnerStarRsrcId;
    Text _onWinText;
    PieceHandlerProxy* _pieceHandlerProxy=nullptr;
    std::vector<int> yRange;
    std::vector<int>::iterator _yRangeIter;

    double _piecesCurrRotAngle=0.0;
    double _deltaRotAngle=0.5;
    bool _isClockwiseRotation=true;
    bool _isIteratorAtEnd=false;
    bool _isActive=false;
};


#endif // INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_