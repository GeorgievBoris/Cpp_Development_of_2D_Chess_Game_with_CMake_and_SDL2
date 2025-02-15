#ifndef INCLUDE_GAME_PIECES_TYPES_UNFINISHEDPIECE_H_
#define INCLUDE_GAME_PIECES_TYPES_UNFINISHEDPIECE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
#include "manager_utils/drawing/Text.h"
// Forward Declarations

class UnfinishedPiece : public ChessPiece {
public:
    int32_t init(const ChessPieceCfg& cfg) final;
    void draw() const final;
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                    Defines::PLAYERS_COUNT>& activePieces) const final;
    void setBoardPos(const BoardPos& boardPos) final;

private:
    Text _notReadyText;
};


#endif // INCLUDE_GAME_PIECES_TYPES_UNFINISHEDPIECE_H_