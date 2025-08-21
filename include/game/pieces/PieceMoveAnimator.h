#ifndef INCLUDE_GAME_PIECES_PIECEMOVEANIMATOR_H_
#define INCLUDE_GAME_PIECES_PIECEMOVEANIMATOR_H_

// C system headers
// C++ system headers
#include <array>
#include <functional>
// Third-party headers
// Own headers
#include "manager_utils/time/TimerClient.h"
#include "utils/drawing/Point.h"
#include "game/defines/ChessDefines.h"
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations
class GameProxy;

// Note: "PieceMoveAnimator.h" is NOT added by Zhivko

class PieceMoveAnimator : public TimerClient{
public:
    int32_t init(GameProxy* gameProxy, int32_t movePieceTimerId, int32_t tileSize, int32_t firstTilePosX, int32_t firstTilePosY, int32_t& collisionIdx,
                const std::function<bool()>& isKingInCheckCb, const std::function<void()>& isInStalemateCb);
    void start(std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& pieces, BoardPos& targetBoardPos,
                        std::pair<int32_t,BoardPos>& pair, const int32_t currPlayerId, const int32_t selectedPieceIdx, const int32_t collisionIdx);
    BoardPos getTakenPieceBoardPosInv(const BoardPos& takenPieceBoardPos);
private:
    void onTimeout(int32_t timerId) final;

    Point getPosOfMovedPiece(const Point& currPos, const Point& targetPos, const PieceType pieceType);
    void moveDeltaX(Point& currAbsPos,const Point& targetAbsPos);
    void moveDeltaY(Point& currAbsPos,const Point& targetAbsPos);
    BoardPos getBoardPosForAnim(const Point& absPos);
    Point getAbsPosForAnim(const BoardPos& boardPos);
    BoardPos getInvBoardPosForAnim(const BoardPos& boardPos, WidgetFlip flipType);
    Point getInvAbsPosForAnim(const Point& absPos, WidgetFlip flipType);
    void shiftBoardPosOfMovedPiece(std::unique_ptr<ChessPiece>& piece, const int32_t currPlayerId);
    bool doPiecesPosOverlap(const Point& targetPos, const Point& posLeft);
    Point getAbsPosOfTakenPiece(const ChessPiece::PlayerPieces& pieces);       

    int32_t _movePieceTimerId;
    int32_t _tileSize{};
    int32_t _firstTilePosX{};
    int32_t _firstTilePosY{};
    int32_t _BOARD_SIZE=8;
    std::pair<int32_t,int32_t> _deltaPosXY;

    std::unique_ptr<ChessPiece>* _movedPiecePtr=nullptr;
    std::unique_ptr<ChessPiece>* _takenPiecePtr=nullptr;
    std::unique_ptr<ChessPiece>* _castlingPiecePtr=nullptr;
    BoardPos* _targetBoardPos=nullptr;
    GameProxy* _gameProxy=nullptr;
    int32_t* _collisionIdxPtr=nullptr;
    const ChessPiece::PlayerPieces* _opponentPiecesPtr=nullptr;
    std::pair<int32_t,BoardPos>* _castlingPairPtr=nullptr;
    std::function<bool()> _isKingInCheckCb;
    std::function<void()> _isInStalemateCb;   
};


#endif // INCLUDE_GAME_PIECES_PIECEMOVEANIMATOR_H_