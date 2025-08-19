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
    int32_t init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy, int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t movePieceTimerId); // "movePieceTimerId" is NOT added by Zhivko
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

    // void checkPawnsStateForEnPassant(); // PieceHandler::checkPawnsForEnPassant() is NOT added by Zhivko
    void checkPawnMoveForEnPassant(); // PieceHandler::checkPawnMoveForEnPassant() is NOT added by Zhivko
    
    bool isMoveValid(); // PieceHandler::isMoveValid() is NOT added by Zhivko

    void alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                        PieceType pieceType) const; // PieceHandler::alertGameBoardIfEnPassant() is NOT added by Zhivko
    bool alertGameBoardIfCastling(const std::unique_ptr<ChessPiece>& piece) const; // PieceHandler::alertGameBoardIfCastlling() is NOT added by Zhivko

    bool isOpponentKingInCheck(); // PieceHandler::isOpponentKingInCheck() is NOT added by Zhivko
    bool isOpponentKingInMate(bool isKingInCheck=false); // PieceHandler::isOpponentKingInMate() is NOT added by Zhivko
    void isOpponentInStalemate(); // PieceHandler::isOpponentInStalemate() is NOT added by Zhivko
    bool isNextMoveCheckForKing(); // PieceHandler::isNextMoveCheckForKing() is NOT added by Zhivko
    bool isPositionTaken(const int32_t nonAttackedPlayerId, const BoardPos& attackedKingPos) const; // PieceHandler::isPositionTaken() is NOT added by Zhivko
    int32_t getKingIndex(int32_t playerId) const; // PieceHandler::getKingIndex() is NOT added by Zhivko

    void rotateWinnerPieces(double angle) final; // PieceHandler::rotateWinnerPieces() is NOT added by Zhivko
    void shiftWinnerPiecesPos() final; // PieceHandler::shiftWinnerPiecesPos() is NOT added by Zhivko
    void onTurnTimeElapsed() final; // PieceHandler::onTurnTimeElapsed() is NOT added by Zhivko
    const ChessPiece::PlayerPieces& getWinnerPieces() final; // PieceHandler::getWinnerPieces() is NOT added by Zhivko
    void changePawnPosIfEnPassant(const std::pair<int32_t,int32_t>& pawnPair, const BoardPos& boardPos) final; // PieceHandler::changePawnPosIfEnPassant() is NOT added by Zhivko
    
    GameBoardProxy* _gameBoardProxy=nullptr;
    GameProxy* _gameProxy=nullptr;

    std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    int32_t _selectedPieceId=0;
    int32_t _currPlayerId=0;
    bool _isPieceGrabbed=false;
    
    PieceMoveAnimator _pieceMoveAnimator; // NOT added by Zhivko
    bool _isCastlingPossible=false; // NOT added by Zhivko
    BoardPos _targetBoardPos; // NOT added by Zhivko
    int32_t _collisionIdx=INVALID_RSRC_ID;  // NOT added by Zhivko
    std::unique_ptr<ChessPiece>* _enPassantPawnId=nullptr; // NOT added by Zhivko
    std::pair<int32_t,BoardPos> pair {INVALID_RSRC_ID,BoardPos{INVALID_RSRC_ID,INVALID_RSRC_ID}}; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_