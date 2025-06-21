#ifndef INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_
#define INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_

// C system headers
// C++ system headers
#include <vector>
#include <list>
#include <memory>
#include <utility>
#include <functional>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Text.h"
#include "manager_utils/time/TimerClient.h"
#include "manager_utils/drawing/Image.h"
// Forward Declarations
class PieceHandlerProxy;

// NOTE: "WinnerAnimator.h" is NOT created by Zhivko!

class WinnerAnimator : public TimerClient {
public:
    int32_t init(PieceHandlerProxy* pieceHandlerProxy, const std::function<void()>& showStartScreenCb, int32_t nextAnimTimerId, int32_t endAnimTimerId,
                int32_t starRsrcId,int32_t fireworksRsrcId, int32_t medalRsrcId, int32_t fontId, int32_t windowWidth, int32_t windowHeight);
    void draw() const;
    void activate(int32_t playerId);
    void deactivate();
    bool isActive() const;
    void restart();
private:
    class NumGenerator{
    public:
        NumGenerator(){_current=0;}
        ~NumGenerator()=default;
        int32_t operator()(){return (_current+=NumGenerator::_DELTA);}
        static const int32_t _DELTA=10;
    private:
        int32_t _current;
    };

    enum class CurveTypes: uint8_t{
        LEFT_TO_RIGHT_DIAGONAL,
        RIGHT_TO_LEFT_DIAGONAL,
        TOP_TO_BOTTOM,
        // LEFT_TO_RIGHT_CENTER,
        COUNT
    };

    void onTimeout(const int32_t timerId) final;
    void rotate();
    void moveText();
    // void rotateText();
    void createStars();
    void createFireworks();
    void updateFireworks();
    void updateStars();
    Point calcStarPos(const CurveTypes type, const int32_t absX=-1);
    void calcFireworkPos(const Point& pos, std::unique_ptr<Image>& fireworkRef);
    void createMedals();

    PieceHandlerProxy* _pieceHandlerProxy=nullptr;
    std::function<void()> _showStartScreenCb;
    Rectangle _windowFrame=Rectangle::UNDEFINED;
    Text _onWinText;
    
    std::vector<Image> _medals;
    std::list<std::pair<CurveTypes,std::unique_ptr<Image>>> _stars;
    std::list<std::pair<std::pair<bool,Point>,std::unique_ptr<Image>>>_fireworks;    
    std::vector<int32_t> _yRange;
    std::vector<int>::const_iterator _yRangeIter;
    std::list<std::pair<uint32_t,uint32_t>> _fireworksPosCounter;
    
    int32_t _nextAnimTimerId;
    int32_t _endAnimTimerId;
    int32_t _starRsrcId;
    int32_t _fireworksRsrcId;
    int32_t _medalRsrcId;

    double _currRotAngle=0.0;
    double _deltaRotAngle=1.0;
    bool _isClockwiseRotation=true;
    bool _moveUp=false;
    bool _isActive=false;
    // // use below parameters if WinnerAnimator::rotateText() is enabled
    // int32_t _textWidth=0;
    // int32_t _textHeight=0;
    // int32_t _textCounter;
};

#endif // INCLUDE_GAME_ANIMATIONS_WINNERANIMATOR_H_