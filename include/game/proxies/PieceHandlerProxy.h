#ifndef INCLUDE_GAME_PROXIES_PIECEHANDLERPROXY_H_
#define INCLUDE_GAME_PROXIES_PIECEHANDLERPROXY_H_

// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

// NOTE: "PieceHandlerProxy.h" is NOT created by Zhivko!

class PieceHandlerProxy{
public:
    virtual ~PieceHandlerProxy()=default;
    virtual void rotateWinnerPieces(double angle)=0;
    virtual void onTurnTimeElapsed()=0;
    virtual const ChessPiece::PlayerPieces& getWinnerPieces()=0;
    virtual void shiftWinnerPiecesPos()=0;
};


#endif // INCLUDE_GAME_PROXIES_PIECEHANDLERPROXY_H_