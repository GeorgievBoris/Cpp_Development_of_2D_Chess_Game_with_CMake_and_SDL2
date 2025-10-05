#ifndef INCLUDE_GAME_ANIMATOR_ANIMATOR_H_
#define INCLUDE_GAME_ANIMATOR_ANIMATOR_H_

// C system headers
// C++ system headers
#include <functional>
// Third-party headers
// Own headers
#include "game/animator/types/GameBoardAnimator.h"
// #include "game/animator/types/GameDrawAnimator.h"
// #include "game/animator/types/GameWinAnimator.h"
#include "game/animator/types/GameEndAnimator.h"
// Forward Declarations
class GameProxy;
class PieceHandlerProxy;
class Fbo;

// Note: "Animator.h" is NOT added by Zhivko

class Animator{
public:
    int32_t init(PieceHandlerProxy* pieceHandlerProxy, GameProxy* gameProxy, Fbo& gameFbo,
                const std::function<void()>& showStartScreenCb, int32_t nextAnimTimerId, int32_t endAnimTimerId, int32_t gameFboRotTimerId,
                int32_t starRsrcId,int32_t fireworksRsrcId, int32_t medalRsrcId, int32_t targetsRsrcId, 
                int32_t fontId, int32_t windowWidth, int32_t windowHeight);    

    bool isGameBoardAnimatorActive() const;
    void draw() const;

    void deactivate();
    int32_t getGameBoardTargetRotation() const;
    void startGameBoardAnim(const int32_t playerId);
    void activateGameEndAnims(int32_t playerId,GameEndType gameEndType,WidgetFlip flipType);
    void restart();

private:
    GameBoardAnimator _gameBoardAnimator;
    GameEndAnimator _gameEndAnimator;
    
    int32_t _nextAnimTimerId=INVALID_RSRC_ID;
    int32_t _endAnimTimerId=INVALID_RSRC_ID;  
};



#endif // INCLUDE_GAME_ANIMATOR_ANIMATOR_H_