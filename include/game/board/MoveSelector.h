#ifndef INCLUDE_GAME_BOARD_MOVESELECTOR_H_
#define INCLUDE_GAME_BOARD_MOVESELECTOR_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <array>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
#include "game/defines/ChessStructs.h"
// Forward Declarations

class MoveSelector{
public:
    int32_t init(int32_t rsrcId);
    void draw() const;
    void markTiles(const std::vector<TileData>& markedTiles);
    void unmarkTiles();

    void shiftMoveTilesPos(const Point& posShift); // MoveSelector::shiftMoveTilesPos() method is NOT added by Zhivko
    void onEnPassant(const BoardPos& boardPos); // MoveSelector::onEnPassant() method is NOT added by Zhivko
    void blinkEnPassant(); // MoveSelector::indicateEnPassant() method is NOT added by Zhivko

    enum InternalDefines{
        MAX_ACTIVE_TILES=28
    };

    
private:
    std::array<Image,MAX_ACTIVE_TILES> _tileImgs;
    size_t _activeTiles{};

    Image* _enPassantTakeTile=nullptr;
    Image* _enPassantMoveTile=nullptr;
};

#endif // INCLUDE_GAME_BOARD_MOVESELECTOR_H_