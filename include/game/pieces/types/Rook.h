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
    bool getIsTaken() const final; // Rook::getIsTaken() is NOT added by Zhivko
    void setWhenFirstMoveIsMade(); // Rook::setWhenFirstMoveIsMade() is NOT added by Zhivko
private:
    std::vector<MoveDirection> getBoardMoves() const;
    bool isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, const BoardPos& kingBoardPos) const; // Rook::checkForCastle() is NOT added by Zhivko
    void setIsTaken(bool isTaken) final; // Rook::setIsTaken() is NOT added by Zhivko
    mutable bool _isCastlingPossible=false; // NOT added by Zhivko
    int32_t _initialColumnPosition=INVALID_RSRC_ID; // NOT added by Zhivko
    bool _isMoved=false; // NOT added by Zhivko
    bool _isTaken=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_TYPES_ROOK_H_