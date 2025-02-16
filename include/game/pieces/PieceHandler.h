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
// Forward Declarations
class InputEvent;
class GameBoardProxy;
class GameProxy;

class PieceHandler{
public:
    int32_t init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy,
                        int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t unfinishedPieceFontId);
    void draw() const;
    void handleEvent(const InputEvent& e);

    void setCurrentPlayerId(int32_t currPlayerId);
    void setWidgetFlip(WidgetFlip flipType);
private:
    void handlePieceGrabbedEvent(const InputEvent& e);
    void handlePieceUngrabbedEvent(const InputEvent& e);

    void doMovePiece(const BoardPos& boardPos);
    

    GameBoardProxy* _gameBoardProxy=nullptr;
    GameProxy* _gameProxy=nullptr;

    std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    int32_t _selectedPieceId=0;
    int32_t _currPlayerId=0;
    bool _isPieceGrabbed=false;

};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_