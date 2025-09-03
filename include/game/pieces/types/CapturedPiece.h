#ifndef INCLUDE_GAME_PIECES_TYPES_CAPTUREDPIECE_H_
#define INCLUDE_GAME_PIECES_TYPES_CAPTUREDPIECE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "game/defines/ChessDefines.h"
// Forward Declarations

// Note: "CapturedPiece.h" is NOT added by Zhivko

class CapturedPiece {
public:
    CapturedPiece()=default;
    void createPieceHalves(int32_t whitePiecesHalvesRsrcId, int32_t blackPiecesHalvesRsrcId, int32_t playerId,
                           PieceType pieceType, const Point& absTopHalfPos);
    void moveHalves();
    void setHalvesOpacity();
    int32_t getHalvesOpacity() const;
    void draw() const;
    bool isActive() const;
private:
    bool _isActive=false;
    Image _topHalf;
    Image _bottomHalf;
};

#endif // GAME_PIECES_TYPES_CAPTUREDPIECE_H_