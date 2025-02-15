#ifndef INCLUDE_DEFINES_CHESSDEFINES_H_
#define INCLUDE_DEFINES_CHESSDEFINES_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

namespace Defines {

enum PlayerId {
    WHITE_PLAYER_ID,
    BLACK_PLAYER_ID,
    PLAYERS_COUNT
};

enum Directions{
    UP_LEFT,
    UP,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    DIRECTION_COUNT
};

enum PawnDefines{
    WHITE_PLAYER_START_PAWN_ROW=6,
    WHITE_PLAYER_START_END_ROW=0,
    BLACK_PLAYER_START_PAWN_ROW=1,
    BLACK_PLAYER_START_END_ROW=7,

    PAWNS_COUNT=8
};

} // end of namespace Defines

// NOTE 1: "PieceType" is NOT an ordinary/standard enum, because it will not be used as an index 
// NOTE 2: by default the type of the enum is "int", but we say it to be "uint8_t" ...
//  ... in order to occupy less memory, because we are not going to save such large amount of information
enum class PieceType : uint8_t {
    KING,
    QUEEN,
    BISHOP,
    KNIGHT,
    ROOK,
    PAWN,
    UNKNOWN
};

enum class TileType : uint8_t{
    MOVE, GUARD, TAKE
};

#endif // INCLUDE_DEFINES_CHESSDEFINES_H_