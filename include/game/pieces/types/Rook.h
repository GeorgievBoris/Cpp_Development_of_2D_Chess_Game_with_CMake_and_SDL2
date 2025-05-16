#ifndef INCLUDE_GAME_PIECES_TYPES_ROOK_H_
#define INCLUDE_GAME_PIECES_TYPES_ROOK_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

class Rook : public ChessPiece{
public:
    int32_t init(const ChessPieceCfg& cfg) final; // Rook::init() method is NOT added by Zhivko
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                            Defines::PLAYERS_COUNT>& activePlayers) const final;
    bool getIsCastlePossible() const; // Rook::getIsCastlePossible() is NOT added by Zhivko
    bool isMoved() const; // Rook::isMoved() is NOT added by Zhivko
private:
    std::vector<MoveDirection> getBoardMoves() const;
    bool isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, const BoardPos& kingBoardPos) const; // Rook::checkForCastle() is NOT added by Zhivko

    mutable bool _isCastlePossible=false; // NOT added by Zhivko
    int32_t _initialColumnPosition=INVALID_RSRC_ID; // NOT added by Zhivko

};


#endif // INCLUDE_GAME_PIECES_TYPES_ROOK_H_