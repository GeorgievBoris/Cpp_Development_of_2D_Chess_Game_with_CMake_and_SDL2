#ifndef INCLUDE_GAME_PIECES_PIECEHANDLERPOPULATOR_H_
#define INCLUDE_GAME_PIECES_PIECEHANDLERPOPULATOR_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <array>
#include <vector>
#include <memory>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations
class GameProxy;

class PieceHandlerPopulator{
public:
    PieceHandlerPopulator()=delete;
    // dtor is left here just in case if somebody somehow creates an object -> this object has a way of destroying
    ~PieceHandlerPopulator()=default; 

    static int32_t populatePieceHandler(GameProxy* gameProxy, int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t unfinishedPieceFontId,
                                        std::array<ChessPiece::PlayerPieces, Defines::PLAYERS_COUNT>& outPieces);
    static std::unique_ptr<ChessPiece> createPiece(PieceType pieceType, GameProxy* gameProxy);
};


#endif // INCLUDE_GAME_PIECES_PIECEHANDLERPOPULATOR_H_