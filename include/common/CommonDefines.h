#ifndef COMMON_COMMONDEFINES_H_
#define COMMON_COMMONDEFINES_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

namespace TextureId{

enum ResourceId{
    CHESS_BOARD,
    TARGET,
    WHITE_PIECES,
    BLACK_PIECES,
    MOVE_TILES,
    COUNT
};

} // namespace TextureId

namespace FontId{

enum FontIdKeys{
    ANGELINE_VINTAGE_40
};

} // namespace FontId

namespace TimerId {
    
enum Keys{
    BLINK_TARGET_TIMER_ID,
    COUNT
};

} // namespace TimerId



#endif // COMMON_COMMONDEFINES_H_