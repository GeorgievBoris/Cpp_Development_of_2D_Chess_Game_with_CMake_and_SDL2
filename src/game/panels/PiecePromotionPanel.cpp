// Corresponding header
#include "game/panels/PiecePromotionPanel.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "game/proxies/GameProxy.h"
#include "sdl_utils/InputEvent.h"

extern const int32_t GAME_X_POS_SHIFT; // added by me
extern const int32_t GAME_Y_POS_SHIFT; // added by me

int32_t PiecePromotionPanel::init(const PiecePromotionPanelCfg& cfg, GameProxy* gameProxy){
    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
        return EXIT_FAILURE;
    }
    _gameProxy=gameProxy;

    PiecePromotionButtonCfg btnCfg;

    // lambda function is used instead of std::bind -> both of course are possible, but lambda is simpler
    btnCfg.onBtnClicked=[this](PieceType pieceType){onButtonClicked(pieceType);};

    btnCfg.buttonWhitePieceRsrcId=cfg.whitePiecesRsrcId;
    btnCfg.buttonBlackPieceRsrcId=cfg.blackPiecesRsrcId;
    btnCfg.buttonBgrRsrcId=cfg.buttonBgrRsrcId;    
    btnCfg.bgrHeight=cfg.buttonBgrHeight;
    btnCfg.bgrWidth=cfg.buttonBgrWidth;
    btnCfg.width=cfg.buttonWidth;
    btnCfg.height=cfg.buttonHeight;

    // TODO: write this in the book...std::array<> has a constexpr Ctor !!!
    constexpr std::array<PieceType,BUTTONS_COUNT> pieceTypes {
        PieceType::QUEEN, PieceType::ROOK, PieceType::BISHOP, PieceType::KNIGHT
    };

    constexpr auto btnOffset=50;
    // const auto startX=(cfg.gameBoardWidth-(BUTTONS_COUNT*(cfg.buttonWidth+btnOffset)))/2; // this is Zhivko's code - I think it has a little error!
    const auto startX=GAME_X_POS_SHIFT+((cfg.gameBoardWidth-((BUTTONS_COUNT*(cfg.buttonWidth+btnOffset))-btnOffset))/2); // added by me - this should be more accurte 
    btnCfg.bgrPos.y=GAME_Y_POS_SHIFT+((cfg.gameBoardHeight-cfg.buttonHeight)/2);

    for(int32_t i=0;i<BUTTONS_COUNT;++i){
        btnCfg.bgrPos.x=(i*(cfg.buttonWidth+btnOffset))+startX;
        btnCfg.pieceType=pieceTypes[i];

        if(EXIT_SUCCESS!=_promotionBtns[i].init(btnCfg)){
            std::cerr<<"_promotionBtns["<<i<<"].init() failed"<<std::endl;

            // TODO: write this in the book...
            // It is good to return EXIT_FAILURE/EXIT_SUCCESS, because we are still at the "init()" step...
            // ... In general, if something fails it will happen while the program initializes ->...
            // .... -> i.e. the init() step. During the runtime, we do not want to be so...
            // ... strict / pendantic, hence can omit EXIT_FAILURE/EXIT_SUCCESS
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

void PiecePromotionPanel::handleEvent(const InputEvent& e){
    for(PiecePromotionButton& btn:_promotionBtns){
        if(btn.isInputUnlocked() && btn.containsEvent(e)){
            btn.handleEvent(e);
            break;
        }
    }
}

void PiecePromotionPanel::draw() const {
    for(const PiecePromotionButton& btn:_promotionBtns){
        btn.draw();
    }
}

void PiecePromotionPanel::activate(int32_t playerId){
    _isActive=true;
    for(PiecePromotionButton& btn:_promotionBtns){
        btn.activate(playerId);
    }
}

bool PiecePromotionPanel::isActive() const{
    return _isActive;
}

void PiecePromotionPanel::restart(){
    _isActive=false;
}

void PiecePromotionPanel::show(){ //PiecePromotionPanel::show() is NOT added by Zhivko
    if(!_isActive){
        return;
    }

    for(PiecePromotionButton& btn:_promotionBtns){
        btn.showBtn();
    }
}

void PiecePromotionPanel::hide(){ //PiecePromotionPanel::hide() is NOT added by Zhivko
    if(!_isActive){
        return;
    }

    for(PiecePromotionButton& btn:_promotionBtns){
        btn.hideBtn();
    }
}

void PiecePromotionPanel::onButtonClicked(PieceType pieceType) {
    _isActive=false;
    _gameProxy->promotePiece(pieceType);

}