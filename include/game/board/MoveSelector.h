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

    void shiftMoveTilesPos(WidgetFlip flipType,const Point& boardPos, const Point& deltaXY); // MoveSelector::shiftMoveTilesPos() is NOT added by Zhivko
    void onEnPassant(const BoardPos& boardPos); // MoveSelector::onEnPassant() is NOT added by Zhivko
    void onCastling(const BoardPos& boardPos); // MoveSelector::onCastling() is NOT added by Zhivko
    void blinkEnPassant(); // MoveSelector::blinkEnPassant() is NOT added by Zhivko
    void blinkCastling(); // MoveSelector::blinkCastling() is NOT added by Zhivko

    const Image* getCastlingKingTilePtr() const; // MoveSelector::getCastlingKingTilePtr() is NOT added by Zhivko

    enum InternalDefines{
        MAX_ACTIVE_TILES=28
    };
private:
    std::array<Image,MAX_ACTIVE_TILES> _tileImgs;
    size_t _activeTiles{};

    Image* _enPassantTakeTile=nullptr; // NOT added by Zhivko
    Image* _enPassantMoveTile=nullptr; // NOT added by Zhivko
    Image* _castlingKingTile=nullptr; // NOT added by Zhivko
};

#endif // INCLUDE_GAME_BOARD_MOVESELECTOR_H_