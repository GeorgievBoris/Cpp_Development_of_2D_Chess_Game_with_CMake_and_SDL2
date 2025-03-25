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
    PROMOTION_BUTTON,
    START_SCREEN_BUTTONS, // NOT added by Zhivko
    START_SCREEN_BACKGROUND, // NOT added by Zhivko
    QUIT_GAME_BUTTON, // NOT added by Zhivko
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
    GAME_FBO_ROT_TIMER_ID,
    START_SCREEN_TIMER_ID,
    PLAYER_TURN_CAP_TIMER_ID, // NOT added by Zhivko
    PLAYER_TURN_CAP_TEXT_TIMER_ID, // NOT added by Zhivko
    BLINK_EN_PASSANT_TIMER_ID, // NOT added by Zhivko
    COUNT
};

} // namespace TimerId



#endif // COMMON_COMMONDEFINES_H_