#ifndef INCLUDE_GAME_BOARD_GAMEBOARD_H_
#define INCLUDE_GAME_BOARD_GAMEBOARD_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations
#include "manager_utils/drawing/Image.h"
#include "game/proxies/GameBoardProxy.h"
#include "manager_utils/time/TimerClient.h"
class GameBoard : public GameBoardProxy , public TimerClient {
public:
    ~GameBoard();
    int32_t init(int32_t boardRsrcId, int32_t targetRsrcId, int32_t blinkTimerId);
    void draw() const;
private:
    void onPieceGrabbed(const BoardPos& boardPos) final;
    void onPieceUngrabbed() final;
    void onTimeout(int32_t timerId) final;

    Image _boardImg;
    Image _targetImg;
    int32_t _blinkTimerId;
};


#endif // INCLUDE_GAME_BOARD_GAMEBOARD_H_