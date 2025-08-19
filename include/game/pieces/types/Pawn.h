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
class PieceHandlerProxy;

class Pawn : public ChessPiece {
public:
    Pawn(GameProxy* gameProxy, PieceHandlerProxy* pieceHandlerProxy);
    std::vector<TileData> getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                    Defines::PLAYERS_COUNT>& activePieces) const final;
    void checkMoveForEnPassant(const BoardPos& boardPos); // Pawn::checkMoveForEnPassant() is NOT added by Zhivko
    bool isPawnTargetedForEnPassant() const; // Pawn::isPawnTargetedForEnPassant() is NOT added by Zhivko
    void checkForPawnPromotion(); // Pawn::checkForPawnPromotion() is NOT added by Zhivko
    void setIsPawnTargetedForEnPassant(bool isPawnTargetedForEnPassant); // Pawn::setIsPawnTargetedForEnPassant() is NOT added by Zhivko
private:
    bool isEnPassantPossible(const BoardPos& boardPos, const ChessPiece::PlayerPieces& enemyPieces, int32_t& enemyPawnIndx) const; // Pawn::isEnPassantValid() method is NOT added by Zhivko
    bool areEnPassantTilesValid(const BoardPos& boardPos,const BoardPos& kingBoardPos,const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, const int32_t enemyPawnIndx) const; // Pawn::areEnPassantTilesValid() is NOT added by Zhivko
    std::vector<TileData> getWhiteMoveTiles(const std::array<ChessPiece::PlayerPieces, Defines::PLAYERS_COUNT>& activePieces, 
                                            const BoardPos& kingBoardPos, const bool isAnotherPieceGetMoveTilesCalled) const; // "BoardPos& kingBoardPos" is NOT added by Zhivko
    std::vector<TileData> getBlackMoveTiles(const std::array<ChessPiece::PlayerPieces, Defines::PLAYERS_COUNT>& activePieces,
                                            const BoardPos& kingBoardPos, const bool isAnotherPieceGetMoveTilesCalled) const; // "BoardPos& kingBoardPos" is NOT added by Zhivko
    std::unordered_map<Defines::Directions,MoveDirection> getWhiteBoardMoves() const;
    std::unordered_map<Defines::Directions,MoveDirection> getBlackBoardMoves() const;
    GameProxy* _gameProxy=nullptr;;
    PieceHandlerProxy* _pieceHandlerProxy=nullptr;; // NOT added by Zhivko;
    bool _isPawnTargetedForEnPassant=false; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_PIECES_TYPES_PAWN_H_