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
#include "game/pieces/types/CapturedPiece.h"
// Forward Declarations
class GameProxy;

// Note: "PieceMoveAnimator.h" is NOT added by Zhivko

class PieceMoveAnimator : public TimerClient{
public:
    int32_t init(GameProxy* gameProxy, int32_t whitePiecesHalvesRsrcId, int32_t blackPiecesHalvesRsrcId,
                int32_t movePieceTimerId, int32_t movePieceHalvesTimerId, int32_t tileSize, int32_t firstTilePosX,
                int32_t firstTilePosY, int32_t& collisionIdx,
                const std::function<bool()>& isKingInCheckCb,
                const std::function<bool(bool)>& isInStalemateCb,
                const std::function<bool()>& isDeadPosition);
                
    void start(std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& pieces, BoardPos& targetBoardPos,
                        std::pair<int32_t,BoardPos>& pair, const int32_t currPlayerId, const int32_t selectedPieceIdx,
                        std::unique_ptr<ChessPiece>*& pawnEnPassantPtr);
    void draw() const;
    BoardPos getTakenPieceBoardPosInv(const BoardPos& takenPieceBoardPos);
    bool isCapturedPieceActive() const;
    int32_t getLastMovedPieceId() const;
private:
    void onTimeout(int32_t timerId) final;
    void movePieceHalves();
    void movePieces();
    void finaliseMove();

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
    bool isTargetPosChangedIfEnPassant();

    int32_t _lastMovedPieceId=INVALID_RSRC_ID;
    int32_t _movePieceTimerId=INVALID_RSRC_ID;
    int32_t _movePieceHalvesTimerId=INVALID_RSRC_ID;
    int32_t _tileSize{};
    int32_t _firstTilePosX{};
    int32_t _firstTilePosY{};
    int32_t _BOARD_SIZE=8;
    int32_t _whitePiecesHalvesRsrcId=INVALID_RSRC_ID;
    int32_t _blackPiecesHalvesRsrcId=INVALID_RSRC_ID;  
    
    std::pair<int32_t,int32_t> _deltaPosXY;

    std::unique_ptr<ChessPiece>* _movedPiecePtr=nullptr;
    std::unique_ptr<ChessPiece>* _takenPiecePtr=nullptr;
    std::unique_ptr<ChessPiece>* _castlingPiecePtr=nullptr;
    std::unique_ptr<CapturedPiece> _capturedPiece=nullptr;
    std::unique_ptr<ChessPiece>* _pawnEnPassantPtr=nullptr;
    BoardPos* _targetBoardPos=nullptr;
    GameProxy* _gameProxy=nullptr;
    int32_t* _collisionIdxPtr=nullptr;
    const ChessPiece::PlayerPieces* _opponentPiecesPtr=nullptr;
    std::pair<int32_t,BoardPos>* _castlingPairPtr=nullptr;
    std::function<bool()> _isKingInCheckCb;
    std::function<bool(bool)> _isInStalemateCb;
    std::function<bool()> _isDeadPositionCb;  
};


#endif // INCLUDE_GAME_PIECES_PIECEMOVEANIMATOR_H_