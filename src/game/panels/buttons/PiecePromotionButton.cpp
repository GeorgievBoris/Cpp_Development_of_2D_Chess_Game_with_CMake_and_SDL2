// Corresponding header
#include "game/panels/buttons/PiecePromotionButton.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

int32_t PiecePromotionButton::init(const PiecePromotionButtonCfg& cfg){
    _cfg=cfg;
    _bgrImg.create(_cfg.buttonBgrRsrcId,_cfg.bgrPos);

    return EXIT_SUCCESS;
}

void PiecePromotionButton::draw() const {
    _bgrImg.draw();
    ButtonBase::draw();
}

void PiecePromotionButton::handleEvent(const InputEvent& e){
    // // Code Implementation by Zhivko
    // if(TouchEvent::TOUCH_RELEASE==e.type){
    //     _cfg.onBtnClicked(_cfg.pieceType);
    // }
    
    
    // NOTE: In my opinion this should be the right code implementation
    if(TouchEvent::TOUCH_PRESS==e.type){
        _bgrImg.setFrame(CLICKED);
    } else if (TouchEvent::TOUCH_RELEASE==e.type){
        _bgrImg.setFrame(UNCLICKED);
        _cfg.onBtnClicked(_cfg.pieceType);
    }
}

void PiecePromotionButton::activate(int32_t activePlayerId){
    const auto rsrcId=(Defines::WHITE_PLAYER_ID==activePlayerId) ? 
                        _cfg.buttonWhitePieceRsrcId : _cfg.buttonBlackPieceRsrcId;

    const int32_t X_Y_DELTA=(_bgrImg.getWidth()-_cfg.width)/2;
    const Point btnPos=Point(_bgrImg.getPosition().x+X_Y_DELTA,_bgrImg.getPosition().y+X_Y_DELTA);
    if(ButtonBase::isCreated()){
        ButtonBase::destroy();
    }
    ButtonBase::create(rsrcId,btnPos);
    ButtonBase::setFrame(static_cast<int32_t>(_cfg.pieceType));
}