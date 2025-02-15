#ifndef INCLUDE_GAME_PIECES_TYPES_PAWN_H_
#define INCLUDE_GAME_PIECES_TYPES_PAWN_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

class Pawn : public ChessPiece {
public:
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                        Defines::PLAYERS_COUNT>& activePieces) const final;

private:
    std::vector<TileData> getWhiteMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                            Defines::PLAYERS_COUNT>& activePieces) const;
    std::vector<TileData> getBlackMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                            Defines::PLAYERS_COUNT>& activePieces) const;

    std::unordered_map<Defines::Directions,MoveDirection> getWhiteBoardMoves() const;
    std::unordered_map<Defines::Directions,MoveDirection> getBlackBoardMoves() const;
};


#endif // INCLUDE_GAME_PIECES_TYPES_PAWN_H_