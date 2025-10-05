#ifndef INCLUDE_GAME_PIECES_PIECEHANDLER_H_
#define INCLUDE_GAME_PIECES_PIECEHANDLER_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <array>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "game/pieces/types/ChessPiece.h"
#include "game/proxies/PieceHandlerProxy.h" // NOT included by Zhivko
#include "game/pieces/PieceMoveAnimator.h" // NOT included by Zhivko
// #include "manager_utils/time/TimerClient.h"// NOT included by Zhivko
// Forward Declarations
class InputEvent;
class GameBoardProxy;
class GameProxy;
class Fbo;

// class PieceHandler : public PieceHandlerProxy, public TimerClient{
class PieceHandler : public PieceHandlerProxy{
public:
    int32_t init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy,
                 int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId,
                 int32_t whitePiecesHalvesRsrcId, int32_t blackPiecesHalvesRsrcId,
                 int32_t movePieceTimerId, int32_t movePieceHalvesTimerId); // "movePieceTimerId" and "movePieceHalvesTimerId" are NOT added by Zhivko
    void drawOnFbo(Fbo& fbo) const;
    void draw() const;
    void handleEvent(const InputEvent& e);

    void setCurrentPlayerId(int32_t currPlayerId);
    void setWidgetFlip(WidgetFlip flipType);
    void promotePiece(PieceType pieceType);
    int32_t restart(); // PieceHandler::restart() is NOT added by Zhivko
private:
    void handlePieceGrabbedEvent(const InputEvent& e);
    void handlePieceUngrabbedEvent(const InputEvent& e);

    void doMovePiece();

    void checkPawnMoveForEnPassant(); // PieceHandler::checkPawnMoveForEnPassant() is NOT added by Zhivko
    
    bool isMoveValid(); // PieceHandler::isMoveValid() is NOT added by Zhivko

    void alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                        PieceType pieceType) const; // PieceHandler::alertGameBoardIfEnPassant() is NOT added by Zhivko
    bool alertGameBoardIfCastling(const std::unique_ptr<ChessPiece>& piece) const; // PieceHandler::alertGameBoardIfCastlling() is NOT added by Zhivko

    bool isOpponentKingInCheck(); // PieceHandler::isOpponentKingInCheck() is NOT added by Zhivko
    bool isOpponentInStalemate(bool isKingInCheck=false); // PieceHandler::isOpponentInStalemate() is NOT added by Zhivko
    bool isNextMoveCheckForKing(); // PieceHandler::isNextMoveCheckForKing() is NOT added by Zhivko
    bool isPositionTaken(const int32_t nonAttackedPlayerId, const BoardPos& attackedKingPos) const; // PieceHandler::isPositionTaken() is NOT added by Zhivko
    int32_t getKingIndex(int32_t playerId) const; // PieceHandler::getKingIndex() is NOT added by Zhivko

    void rotateWinnerPieces(const bool isNoWinner, const double angle) final; // PieceHandler::rotateWinnerPieces() is NOT added by Zhivko
    void shiftWinnerPiecesPos(const bool isNoWinner) final; // PieceHandler::shiftWinnerPiecesPos() is NOT added by Zhivko
    void onTurnTimeElapsed() final; // PieceHandler::onTurnTimeElapsed() is NOT added by Zhivko
    const ChessPiece::PlayerPieces& getWinnerPieces() final; // PieceHandler::getWinnerPieces() is NOT added by Zhivko
    void changePawnPosIfEnPassant(const std::pair<int32_t,int32_t>& pawnPair, const BoardPos& boardPos) final; // PieceHandler::changePawnPosIfEnPassant() is NOT added by Zhivko
    const BoardPos getBoardPosOfKingAndAttackingPiece(const int32_t playerId) const final; // PieceHandler::getBoardPosOfKingAndAttackingPiece()

    bool isDeadPosition(); // PieceHandler::isDeadPosition() is NOT added by Zhivko

    GameBoardProxy* _gameBoardProxy=nullptr;
    GameProxy* _gameProxy=nullptr;
    PieceMoveAnimator _pieceMoveAnimator; // NOT added by Zhivko
    BoardPos _targetBoardPos; // NOT added by Zhivko

    std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    std::unique_ptr<ChessPiece>* _pawnEnPassantPtr=nullptr; // NOT added by Zhivko
    std::pair<int32_t,BoardPos> pair {INVALID_RSRC_ID,BoardPos{INVALID_RSRC_ID,INVALID_RSRC_ID}}; // NOT added by Zhivko    
    
    int32_t _selectedPieceId=0;
    int32_t _currPlayerId=0;
    int32_t _collisionIdx=INVALID_RSRC_ID;  // NOT added by Zhivko
    size_t _whitePiecesCount=Defines::TOTAL_PIECES_COUNT; // NOT added by Zhivko
    size_t _blackPiecesCount=Defines::TOTAL_PIECES_COUNT; // NOT added by Zhivko
    int32_t _bishopsAndKnightsCount=Defines::BISHOPS_AND_KNIGHTS_COUNT; // NOT added by Zhivko
    bool _isPieceGrabbed=false;
    bool _isCastlingPossible=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_