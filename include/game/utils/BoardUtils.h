#ifndef INCLUDE_GAME_UTILS_BOARDUTILS_H_
#define INCLUDE_GAME_UTILS_BOARDUTILS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardPos.h"
#include "utils/drawing/Point.h"
#include "game/pieces/types/ChessPiece.h"
#include "game/defines/ChessDefines.h"
// Forward Declarations

class BoardUtils{
public:
    BoardUtils()=delete; // probably Zhivko forgot to add it, but it must be here for a class that contains only static members
    ~BoardUtils()=default; // probably Zhivko forgot to add it, but it must be here for a class that contains only static members

    // map monitor/screen coordinates to coordinates on the chess board AND vice versa
    static BoardPos getBoardPos(const Point& absPos);
    static Point getAbsPos(const BoardPos& boardPos);
    static BoardPos getInvertedBoardPos(const BoardPos& boardPos, WidgetFlip flipType); // NOT added by Zhivko
    static Point getCenterPos(const int32_t& width, const int32_t& height); // NOT added by Zhivko
    static bool isInsideBoard(const BoardPos& boardPos);
    static bool isInsideBoard(const Point& absPos);

    static int32_t getOpponentId(int32_t activePlayerId);
    static BoardPos getAdjacentPos(Defines::Directions dir, const BoardPos& currPos);
    static bool doCollideWithPiece(const BoardPos& selectedPos,
                                        const ChessPiece::PlayerPieces& pieces, int32_t& outCollisionRelativeId);
    static TileType getTileType(const BoardPos& boardPos, const ChessPiece::PlayerPieces& playerPieces,
                                                            const ChessPiece::PlayerPieces& enemyPieces);

    static BoardPos shiftBoardPositions(const BoardPos& boardPos); // BoardUtils::shiftBoardPositions() is NOT added by Zhivko
        
    static void getBoardPosIfEnPassant(const std::unique_ptr<ChessPiece>& selectedPiece, const ChessPiece::PlayerPieces& enemyPieces,
                                        BoardPos& boardPos, int32_t& outCollisionRelativeId); // BoardUtils::getBoardPosIfEnPassant() method is NOT added by Zhivko    
    
    static void getBoardPosIfCastling(const ChessPiece::PlayerPieces& pieces, const std::unique_ptr<ChessPiece>& piece, BoardPos& newBoardPos,
                                                std::pair<int32_t,BoardPos>& pair); // BoardUtils::getBoardPosIfCastling() is NOT added by Zhivko
    static BoardPos getKingBoardPos(const ChessPiece::PlayerPieces& pieces); // BoardUtils::getBoardPosIfCastling() is NOT added by Zhivko
    static std::pair<int32_t,int32_t> getTakenPieceMoveDirections(const BoardPos& targetBoardPos, const BoardPos& boardPos); // BoardUtils::getTakenPieceMoveDirection() is NOT added by Zhivko
};

#endif // INCLUDE_GAME_UTILS_BOARDUTILS_H_