// Corresponding header
#include "game/animator/Animator.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers

int32_t Animator::init(PieceHandlerProxy* pieceHandlerProxy, GameProxy* gameProxy, Fbo& gameFbo,
                                const std::function<void()>& showStartScreenCb,int32_t nextAnimTimerId,
                                int32_t endAnimTimerId, int32_t gameFboRotTimerId, int32_t starRsrcId, int32_t fireworksRsrcId, int32_t medalRsrcId, int32_t targetsRsrcId,
                                int32_t fontId, int32_t windowWidth, int32_t windowHeight){
    
    if(EXIT_SUCCESS!=_gameEndAnimator.init(pieceHandlerProxy,showStartScreenCb,nextAnimTimerId,endAnimTimerId,starRsrcId,fireworksRsrcId,
                                            medalRsrcId,targetsRsrcId,fontId,windowWidth,windowHeight)){
        std::cerr<<"_gameEndAnimator.init() failed"<<std::endl;           
        return EXIT_FAILURE;                                     
    }
    
    if(EXIT_SUCCESS!=_gameBoardAnimator.init(gameProxy,&gameFbo,gameFboRotTimerId)){
        std::cerr<<"_gameBoardAnimator.init() failed"<<std::endl;                                        
        return EXIT_FAILURE;
    }    

    return EXIT_SUCCESS;
}

bool Animator::isGameBoardAnimatorActive() const{
    return _gameBoardAnimator.isActive();
}

void Animator::draw() const{
    _gameEndAnimator.draw();
}

void Animator::deactivate(){
    _gameEndAnimator.deactivate();
}

int32_t Animator::getGameBoardTargetRotation() const{
    return _gameBoardAnimator.getTargetRotation();
}

void Animator::startGameBoardAnim(const int32_t playerId){
    _gameBoardAnimator.startAnim(playerId);
}

void Animator::activateGameEndAnims(int32_t playerId,GameEndType gameEndType,WidgetFlip flipType){
    _gameEndAnimator.activate(playerId,gameEndType,flipType);
}

void Animator::restart(){
    _gameBoardAnimator.restart();
    _gameEndAnimator.restart();
}