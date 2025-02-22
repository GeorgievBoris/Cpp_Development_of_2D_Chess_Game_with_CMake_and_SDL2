#ifndef INCLUDE_GAME_BOARD_GAMEBOARD_H_
#define INCLUDE_GAME_BOARD_GAMEBOARD_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "manager_utils/time/TimerClient.h"
#include "game/board/MoveSelector.h"
#include "game/proxies/GameBoardProxy.h"
// Forward Declarations
class Fbo;

class GameBoard : public GameBoardProxy , public TimerClient {
public:
    ~GameBoard();
    int32_t init(int32_t boardRsrcId, int32_t targetRsrcId, int32_t moveTilesRsrcId, int32_t blinkTimerId);
    void drawGameBoardOnFbo(Fbo& fbo) const;
    void drawGameBoardOnly() const;
    void draw() const;
private:
    void onPieceGrabbed(const BoardPos& boardPos, const std::vector<TileData>& moveTiles) final;
    void onPieceUngrabbed() final;
    void onTimeout(int32_t timerId) final;
    bool isMoveAllowed(const BoardPos& pos) const final;

    int32_t _activePlayerId=Defines::WHITE_PLAYER_ID; // this field/member is added by me - NOT by Zhivko !!!

    Image _boardImg;
    Image _targetImg;
    int32_t _blinkTimerId;

    MoveSelector _moveSelector;
    std::vector<TileData> _currMoveTiles;
};


#endif // INCLUDE_GAME_BOARD_GAMEBOARD_H_