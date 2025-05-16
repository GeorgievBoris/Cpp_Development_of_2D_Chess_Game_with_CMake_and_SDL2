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
    virtual void onGameFinish()=0; // GameProxy::onKingCheckMate() method NOT added by Zhivko
    virtual void castleTextShow()=0; // GameProxy::castleTextShow() method is NOT added by Zhivko
    virtual void castleTextHide()=0; // GameProxy::castleTextHide() method is NOT added by Zhivko
};


#endif // INCLUDE_GAME_PROXIES_GAMEPROXY_H_