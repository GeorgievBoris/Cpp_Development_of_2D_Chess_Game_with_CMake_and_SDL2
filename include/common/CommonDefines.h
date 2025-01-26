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
    RUNNING_GIRL,
    WHEEL,
    BLACK_BACKGROUND,
    START_BUTTON,
    STOP_BUTTON,
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
    WHEEL_ROT_ANIM_TIMER_ID,
    WHEEL_SCALE_TIMER_ID,
    GIRL_MOVE_TIMER_ID,
    COUNT
};

} // namespace TimerId



#endif // COMMON_COMMONDEFINES_H_