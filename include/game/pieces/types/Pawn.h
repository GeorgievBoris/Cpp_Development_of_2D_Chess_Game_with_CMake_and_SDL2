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
class GameProxy;

class Pawn : public ChessPiece {
public:
    Pawn(GameProxy* gameProxy);
    void setBoardPos(const BoardPos& boardPos) final;
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                        Defines::PLAYERS_COUNT>& activePieces) const final;
private:
    bool isEnPassantValid(const BoardPos& boardPos, const ChessPiece::PlayerPieces& enemyPieces) const; // Pawn::isEnPassantValid() method is NOT added by Zhivko

    std::vector<TileData> getWhiteMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                            Defines::PLAYERS_COUNT>& activePieces) const;
    std::vector<TileData> getBlackMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                                            Defines::PLAYERS_COUNT>& activePiece) const;

    std::unordered_map<Defines::Directions,MoveDirection> getWhiteBoardMoves() const;
    std::unordered_map<Defines::Directions,MoveDirection> getBlackBoardMoves() const;

    GameProxy* _gameProxy;
};


#endif // INCLUDE_GAME_PIECES_TYPES_PAWN_H_