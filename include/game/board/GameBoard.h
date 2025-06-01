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
    int32_t init(int32_t boardRsrcId, int32_t targetRsrcId, int32_t moveTilesRsrcId, int32_t blinkTimerId,
                        int32_t enPassantTimerId, int32_t castlingTimerId); // "int32_t enPassantTimerId" is NOT added by Zhivko
    void drawGameBoardOnFbo(Fbo& fbo) const;
    void drawGameBoardOnly() const;
    void draw() const;
    void restart(); // GameBoard::restart() method is NOT added by Zhivko
    void show(); // GameBoard::show() method is NOT added by Zhivko
    void hide(); // GameBoard::hide() method is NOT added by Zhivko
    void setWidgetFlip(WidgetFlip flipType); // GameBoard::setWidgetFlip() is NOT added by Zhivko
    const Point getChessBoardBoardPos() const;  // GameBoard::getChessBoardBoardPos() is NOT added by Zhivko
private:
    void onPieceGrabbed(const BoardPos& boardPos, const std::vector<TileData>& moveTiles) final;
    void onPieceUngrabbed() final;
    void onTimeout(int32_t timerId) final;
    bool isMoveAllowed(const BoardPos& pos) const final;

    void shiftMoveTilesPos(const BoardPos& boardPos); // GameBoard::shiftMoveTilesPos() is NOT added by Zhivko

    void onEnPassant(const BoardPos& boardPos) final; // GameBoard::onEnPassant() is NOT added by Zhivko
    void onCastling(const BoardPos& boardPos) final; // GameBoard::onCastling() is NOT added by Zhivko

    int32_t _activePlayerId=Defines::WHITE_PLAYER_ID; // this field/member is NOT added by Zhivko

    mutable Image _boardImg; // "mutable" is NOT added by Zhivko
    Image _targetImg;
    int32_t _blinkTimerId;
    int32_t _enPassantTimerId; // NOT added by Zhivko
    int32_t _castlingTimerId; // NOT added by Zhivko
    const TileData* _enPassantEnemyPawn=nullptr; // NOT added by Zhivko
    const TileData* _enPassantMovePos=nullptr; // NOT added by Zhivko
    WidgetFlip _flipType=WidgetFlip::NONE; // NOT added by Zhivko
    MoveSelector _moveSelector;
    std::vector<TileData> _currMoveTiles;
};


#endif // INCLUDE_GAME_BOARD_GAMEBOARD_H_