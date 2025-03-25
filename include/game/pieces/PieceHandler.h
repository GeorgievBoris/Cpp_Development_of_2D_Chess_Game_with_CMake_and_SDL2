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

    int32_t restart(const std::function<void()>& gameRegenerateFboCallBack); // NOT added by Zhivko ; check where #include <functional> is included so that it does not give error here???
    void setIsPieceGrabbed(); //  NOT added by Zhivko
private:
    void handlePieceGrabbedEvent(const InputEvent& e);
    void handlePieceUngrabbedEvent(const InputEvent& e);

    void doMovePiece(BoardPos& boardPos); // "(const BoardPos& boardPos)" originally added by Zhivko
    void unmarkPieces(); // NOT added by Zhivko

    void alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                        PieceType pieceType) const ; // PieceHandler::alertGameBoardIfEnPassant() is NOT added by Zhivko
    
    GameBoardProxy* _gameBoardProxy=nullptr;
    GameProxy* _gameProxy=nullptr;

    std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    int32_t _selectedPieceId=0;
    int32_t _currPlayerId=0;
    bool _isPieceGrabbed=false;
};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_