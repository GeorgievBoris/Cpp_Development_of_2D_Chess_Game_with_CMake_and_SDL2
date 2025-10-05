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
    Rook()=default; // NOT added by Zhivko
    int32_t init(const ChessPieceCfg& cfg) final; // Rook::init() method is NOT added by Zhivko
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                            Defines::PLAYERS_COUNT>& activePlayers) const final;                                       
    bool getIsCastlingPossible() const; // Rook::getIsCastlingPossible() is NOT added by Zhivko
    bool isMoved() const; // Rook::isMoved() is NOT added by Zhivko
    void setWhenFirstMoveIsMade(); // Rook::setWhenFirstMoveIsMade() is NOT added by Zhivko
private:
    bool isDeadPosition(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, int32_t& idx) final; // NOT added by Zhivko
    std::vector<MoveDirection> getBoardMoves() const;
    void clearVecBoardPos(); // NOT added by Zhivko
    std::vector<BoardPos> _vecBoardPos; // NOT added by Zhivko
    bool isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, const BoardPos& kingBoardPos) const; // NOT added by Zhivko
    mutable bool _isCastlingPossible=false; // NOT added by Zhivko
    int32_t _initialColumnPosition=INVALID_RSRC_ID; // NOT added by Zhivko
    bool _isMoved=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_TYPES_ROOK_H_