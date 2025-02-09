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

class PieceHandler{
public:
    int32_t init(GameBoardProxy* gameBoardProxy, int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId);
    void draw() const;
    void handleEvent(const InputEvent& e);


private:
    void handlePieceGrabbedEvent(const InputEvent& e);
    void handlePieceUngrabbedEvent(const InputEvent& e);

    int32_t populateWhitePieces(int32_t rsrcId);
    int32_t populateBlackPieces(int32_t rsrcId);
// since we will very often access the chess figures, this access must happen fast...
// ... therefore use random_access_iterator -> std::vector...Furthermore,...
// ... we use std::vector<> when the number/count of the elements inside it is dynamic (i.e. the count reduces/increases)
    using PlayerPieces=std::vector<ChessPiece>;

    GameBoardProxy* _gameBoardProxy=nullptr;

    std::array<PlayerPieces,Defines::PLAYERS_COUNT> _pieces;
    int32_t _selectedPieceId=0;
    int32_t _selectedPiecePlayerId=0;
    bool _isPieceGrabbed=false;

};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLER_H_