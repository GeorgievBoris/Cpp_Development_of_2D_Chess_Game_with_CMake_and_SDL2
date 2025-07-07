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
        
    static void checkForEnPassant(const std::unique_ptr<ChessPiece>& selectedPiece, const ChessPiece::PlayerPieces& enemyPieces,
                                        BoardPos& boardPos, int32_t& outCollisionRelativeId); // BoardUtils::checkForEnPassant() method is NOT added by Zhivko    
    
    static void checkForCastling(const ChessPiece::PlayerPieces& pieces, const std::unique_ptr<ChessPiece>& piece, BoardPos& newBoardPos,
                                                std::pair<bool,std::pair<int32_t,BoardPos>>& pair); // BoardUtils::checkForCastling() is NOT added by Zhivko
    static Point getPosOfMovedPiece(const Point& currPos, const Point& targetPos, const PieceType pieceType); // BoardUtils::getPosOfMovedPiece() is NOT added by Zhivko
    static void moveDeltaX(Point& currAbsPos,const Point& targetAbsPos); // BoardUtils::moveDeltaX() is NOT added by Zhivko
    static void moveDeltaY(Point& currAbsPos,const Point& targetAbsPos); // BoardUtils::moveDeltaY() is NOT added by Zhivko                                            
    static BoardPos getBoardPosForAnim(const Point& absPos); // BoardUtils::getBoardPosForAnim() is NOT added by Zhivko
    static Point getAbsPosForAnim(const BoardPos& boardPos); // BoardUtils::getAbsPosForAnim() is NOT added by Zhivko
    static BoardPos getInvBoardPosForAnim(const BoardPos& boardPos, WidgetFlip flipType); // BoardUtils::getInvBoardPosForAnim() is NOT added by Zhivko

    static void shiftBoardPosOfMovedPiece(std::unique_ptr<ChessPiece>& piece, const int32_t currPlayerId, BoardPos& targetBoardPos); // BoardUtils::shiftBoardPosOfMovedPiece() is NOT added by Zhivko
    static bool doPiecesPosOverlap(const Point& targetPos, const Point& posLeft); // BoardUtils::doPiecesPosOverlap() is NOT added by Zhivko
    static Point getAbsPosOfTakenPiece(const ChessPiece::PlayerPieces& pieces); // BoardUtils::getAbsPosOfTakenPiece() is NOT added by Zhivko
};

#endif // INCLUDE_GAME_UTILS_BOARDUTILS_H_