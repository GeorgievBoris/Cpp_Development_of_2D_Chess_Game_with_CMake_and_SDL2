#ifndef INCLUDE_GAME_PIECES_TYPES_QUEEN_H_
#define INCLUDE_GAME_PIECES_TYPES_QUEEN_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <array>
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

// Note: "Queen.h" is NOT added by Zhivko

class Queen : public ChessPiece {
public:
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const final;
private:
    std::vector<MoveDirection> getBoardMoves() const;
};



#endif // INCLUDE_GAME_PIECES_TYPES_QUEEN_H_