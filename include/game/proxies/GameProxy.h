#ifndef INCLUDE_GAME_PROXIES_GAMEPROXY_H_
#define INCLUDE_GAME_PROXIES_GAMEPROXY_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
// Forward Declarations

class GameProxy{
public:
    virtual ~GameProxy()=default;
    virtual void onGameTurnFinished()=0;
    virtual void onPawnPromotion()=0;
    virtual void promotePiece(PieceType pieceType)=0;
    virtual void onBoardAnimFinished()=0;
    virtual void setWidgetFlip(WidgetFlip flipType)=0;
    virtual void onGameFinish()=0; // GameProxy::onGameFinish() method NOT added by Zhivko
    virtual void castleTextShow()=0; // GameProxy::castleTextShow() method is NOT added by Zhivko
    virtual void castleTextHide()=0; // GameProxy::castleTextHide() method is NOT added by Zhivko
    virtual void setCurrPlayerKingInCheck(bool isCurrPlayerKingInCheck)=0; // Game::setCurrPlayerKingInCheck() is NOT added by Zhivko
    virtual bool isCurrPlayerKingInCheck()=0; // GameProxy::isCurrPlayerKingInCheck() method is NOT added by Zhivko
    virtual void setAutomaticWin(bool isAutomaticWin)=0; // GameProxy::setAutomaticWin() method is NOT added by Zhivko
    virtual bool isAutomaticWin()=0; // GameProxy::isAutomaticWin() method is NOT added by Zhivko
    virtual bool isWinnerAnimatorActive()=0; // GameProxy::isWinnerAnimatorActive() method is NOT added by Zhivko
    virtual bool isPromotionActive()=0; // GameProxy::isPromotionActive() method is NOT added by Zhivko
};


#endif // INCLUDE_GAME_PROXIES_GAMEPROXY_H_