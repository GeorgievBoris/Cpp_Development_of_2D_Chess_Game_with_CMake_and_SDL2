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

}

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

#endif // INCLUDE_DEFINES_CHESSDEFINES_H_