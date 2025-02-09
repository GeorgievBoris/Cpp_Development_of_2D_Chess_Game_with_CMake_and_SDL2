// Corresponding header
#include "game/utils/BoardUtils.h"
// C system headers
// C++ system headers
#include <cstdlib>
// Third-party headers
// Own headers
#include "utils/drawing/Rectangle.h"

// NOTE: if these four variables DO NOT need to go out of this source file / be visible outside of it...
// ... use them either in a "namespace" or set them as "static"

static constexpr auto BOARD_SIZE=8;
static constexpr auto FIRST_TILE_X_POS=58;
static constexpr auto FIRST_TILE_Y_POS=60;
static constexpr auto TILE_SIZE=98;

BoardPos BoardUtils::getBoardPos(const Point& absPos){
    return {(absPos.y-FIRST_TILE_Y_POS)/TILE_SIZE, (absPos.x-FIRST_TILE_X_POS)/TILE_SIZE};
}

Point BoardUtils::getAbsPos(const BoardPos& boardPos){
    return {(boardPos.col*TILE_SIZE)+FIRST_TILE_X_POS, (boardPos.row*TILE_SIZE)+FIRST_TILE_Y_POS};
}

bool BoardUtils::isInsideBoard(const BoardPos& boardPos){

    const Rectangle bound{0, 0, BOARD_SIZE, BOARD_SIZE};
    const Point pos {boardPos.col, boardPos.row};

    return bound.isPointInside(pos);
}

bool BoardUtils::isInsideBoard(const Point& absPos){

    const Rectangle bound(FIRST_TILE_X_POS, FIRST_TILE_Y_POS, BOARD_SIZE*TILE_SIZE, BOARD_SIZE*TILE_SIZE);

    return bound.isPointInside(absPos);
}