#ifndef INCLUDE_GAME_PIECES_TYPES_KNIGHT_H
#define INCLUDE_GAME_PIECES_TYPES_KNIGHT_H

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <array>
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

// Note: "Knight.h" is NOT added by Zhivko

class Knight : public ChessPiece{
public:
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const final;
private:
    bool isDeadPosition(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, int32_t& idx) final;
    std::vector<MoveDirection> getBoardMoves() const;
    void clearVecBoardPos();
    std::vector<BoardPos> _vecBoardPos;
};

#endif // INCLUDE_GAME_PIECES_TYPES_KNIGHT_H