#ifndef INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_
#define INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations
struct BoardPos;

class GameBoardProxy{
public:
    virtual ~GameBoardProxy()=default;
    virtual void onPieceGrabbed(const BoardPos& boardPos)=0;
    virtual void onPieceUngrabbed()=0;
};

#endif // INCLUDE_GAME_PROXIES_GAMEBOARDPROXY_H_