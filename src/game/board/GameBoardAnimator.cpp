// Corresponding header
#include "game/board/GameBoardAnimator.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/proxies/GameProxy.h"

int32_t GameBoardAnimator::init(GameProxy* gameProxy, Image* boardImg){
    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameProxy=gameProxy;

    if(nullptr==boardImg){
        std::cerr<<"Error, nullptr provided for boardImg"<<std::endl;
        return EXIT_FAILURE;
    }

    _boardImg=boardImg;

    const Point rotCenter(_boardImg->getWidth()/2,_boardImg->getHeight()/2);
    _boardImg->setRotationCenter(rotCenter);

    return EXIT_SUCCESS;
}

void GameBoardAnimator::startAnim(int32_t playerId){
    // Write this in the book: When we want to achieve mirroring effect - it is much better to perform flipping rather than rotation !!!
    if(Defines::WHITE_PLAYER_ID==playerId){
        // _boardImg->setRotationAngle(FULL_ROTATION/2)
        _targetFlipType=WidgetFlip::HORIZONTAL_AND_VERTICAL;
    } else {
        // _boardImg->setRotationAngle(0);
        _targetFlipType=WidgetFlip::NONE;
    }
    _boardImg->setFlipType(_targetFlipType);
    _gameProxy->setWidgetFlip(_targetFlipType);
    _gameProxy->onBoardAnimFinished();
}
