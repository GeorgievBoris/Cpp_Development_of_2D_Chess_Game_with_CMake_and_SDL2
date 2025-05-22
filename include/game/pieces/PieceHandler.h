#ifndef INCLUDE_GAME_PIECES_PIECEHANDLER_H_
#define INCLUDE_GAME_PIECES_PIECEHANDLER_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <array>
#include <functional> // NOT included by Zhivko !!!
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations
class InputEvent;
class GameBoardProxy;
class GameProxy;
class Fbo;

class PieceHandler{
public:
    int32_t init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy,
                        int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t unfinishedPieceFontId);
    void drawOnFbo(Fbo& fbo) const;
    void draw() const;
    void handleEvent(const InputEvent& e);

    void setCurrentPlayerId(int32_t currPlayerId);
    void setWidgetFlip(WidgetFlip flipType);
    void promotePiece(PieceType pieceType);

    // PieceHandler::restart() is NOT added by Zhivko
    int32_t restart(const std::function<void()>& gameRegenerateFboCallBack); // check where #include <functional> is included so that it does not give error here???
    void onPawnPromotion(); // PieceHandler::onPawnPromotion() is NOT added by Zhivko
    void onTurnTimeElapsed(); // PieceHandler::onTurnTimeElapsed() is NOT added by Zhivko
private:
    void handlePieceGrabbedEvent(const InputEvent& e);
    void handlePieceUngrabbedEvent(const InputEvent& e);

    void doMovePiece(const BoardPos& boardPos, int32_t& collisionIdx,
                    std::unique_ptr<ChessPiece>& selectedPiece, const std::pair<bool,std::pair<int32_t, BoardPos>>& pair); // the "collisionIdx", "currPlayerSelectedPiece" and "pair" arguments are NOT added by Zhivko

    void checkPawnsStateForEnPassant(const BoardPos& newBoardPos, const std::unique_ptr<ChessPiece>& selectedPiece); // PieceHandler::checkPawnsForEnPassant() is NOT added by Zhivko
    
    bool isMoveValid(BoardPos& boardPos, bool isCastlingDone, int32_t& collisionIdx); // PieceHandler::isMoveValid() is NOT added by Zhivko
    
    void unmarkPieces(); // PieceHandler::unmarkPieces() is NOT added by Zhivko

    void alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                        PieceType pieceType) const; // PieceHandler::alertGameBoardIfEnPassant() is NOT added by Zhivko
    bool alertGameBoardIfCastling(const std::unique_ptr<ChessPiece>& piece) const; // PieceHandler::alertGameBoardIfCastlling() is NOT added by Zhivko

    bool isOpponentKingInCheck(); // PieceHandler::isOpponentKingInCheck() is NOT added by Zhivko
    bool isOpponentKingInMate(); // PieceHandler::isOpponentKingInMate() is NOT added by Zhivko
    void isOpponentInStalemate(); // PieceHandler::isOpponentInStalemate() is NOT added by Zhivko
    bool isNextMoveCheckForKing(int32_t playerId, int32_t selectedPieceId, int32_t collisionIdx, const BoardPos& boardPos); // PieceHandler::isNextMoveCheckForKing() is NOT added by Zhivko
    bool isPositionTaken(int32_t nonAttackedPlayerId, const BoardPos& attackedKingPos, const BoardPos& nonAttackedKingPos) const; // PieceHandler::isPositionTaken() is NOT added by Zhivko
    int32_t getKingIndex(int32_t playerId) const; // PieceHandler::getKingIndex() is NOT added by Zhivko
    

    GameBoardProxy* _gameBoardProxy=nullptr;
    GameProxy* _gameProxy=nullptr;

    std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    int32_t _selectedPieceId=0;
    int32_t _currPlayerId=0;
    bool _isPieceGrabbed=false;

    bool _isKingInCheck=false; // NOT added by Zhivko
    bool _isPawnPromoted=false; // NOT added by Zhivko
    bool _isCastlingPossible=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_