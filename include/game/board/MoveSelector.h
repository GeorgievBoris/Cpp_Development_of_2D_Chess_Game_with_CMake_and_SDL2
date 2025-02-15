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

    enum InternalDefines{
        MAX_ACTIVE_TILES=28
    };
private:
    std::array<Image,MAX_ACTIVE_TILES> _tileImgs;
    size_t _activeTiles{};
};

#endif // INCLUDE_GAME_BOARD_MOVESELECTOR_H_