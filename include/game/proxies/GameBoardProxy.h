#ifndef INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_
#define INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
#include "game/defines/ChessStructs.h"
// Forward Declarations
struct BoardPos;

class GameBoardProxy{
public:
    virtual ~GameBoardProxy()=default;
    virtual void onPieceGrabbed(const BoardPos& boardPos, const std::vector<TileData>& moveTiles)=0;
    virtual void onPieceUngrabbed()=0;
    virtual bool isMoveAllowed(const BoardPos& pos)const =0;
    virtual void onEnPassant(const BoardPos& boardPos)=0; // GameBoardProxy::onEnPassant() is NOT added by Zhivko
    virtual void onCastling(const BoardPos& boardPos)=0; // GameBoardProxy::onCastling() is NOT added by Zhivko
};

#endif // INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_