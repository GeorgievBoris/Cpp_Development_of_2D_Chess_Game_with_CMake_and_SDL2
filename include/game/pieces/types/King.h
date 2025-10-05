#ifndef INCLUDE_GAME_PIECES_TYPES_KING_H
#define INCLUDE_GAME_PIECES_TYPES_KING_H

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
#include <array>
// Third-party headers
// Own headers
#include "game/pieces/types/ChessPiece.h"
// Forward Declarations

// Note: "King.h" is NOT added by Zhivko

/*
Information about the Castling the King and the Rook:

1) Castling is permitted only if neither the king nor the rook has previously moved; 
2) the squares between the king and the rook are vacant; 
3) and the king does not leave, cross over, or finish on a square attacked by an enemy piece. 
4) Castling is the only move in chess in which two pieces are moved at once.[3] 
*/

class King : public ChessPiece{
public:
    King()=default;
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const final;
    bool getIsCastlePossible() const;
    bool isMoved() const;
    bool isInCheck() const;
    void setIsInCheck(bool isInCheck);
    void setWhenFirstMoveIsMade();
private:
    bool isDeadPosition(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, int32_t& idx) final;
    std::vector<MoveDirection> getBoardMoves() const;
    bool isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, const BoardPos& rookBoardPos) const;
    void clearBoardPosVec();
    mutable bool _isCastlePossible=false;
    bool _isMoved=false;
    bool _isInCheck=false;
    std::vector<BoardPos> _boardPosVec;
};

#endif // INCLUDE_GAME_PIECES_TYPES_KING_H


 