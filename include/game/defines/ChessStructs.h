#ifndef INCLUDE_GAME_DEFINES_CHESSSTRUCTS_H_
#define INCLUDE_GAME_DEFINES_CHESSSTRUCTS_H_

// C system headers
#include <cstdint>
#include <vector>
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardPos.h"
#include "game/defines/ChessDefines.h"
// Forward Declarations

struct TileData{
    TileData(BoardPos inputBoardPos, TileType inputTileType) : boardPos(inputBoardPos), tileType(inputTileType) {} // check if Zhivko has added this ???
    TileData()=default; // check if Zhivko has added this ???
    BoardPos boardPos;
    TileType tileType=TileType::MOVE;
};

using MoveDirection=std::vector<BoardPos>;



#endif // INCLUDE_GAME_DEFINES_CHESSSTRUCTS_H_