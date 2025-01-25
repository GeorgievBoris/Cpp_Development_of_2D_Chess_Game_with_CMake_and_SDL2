#ifndef INCLUDE_GAME_ENTITIES_WHEEL_H_
#define INCLUDE_GAME_ENTITIES_WHEEL_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
// Forward Declarations
class InputEvent;

class Wheel{
public:
    int32_t init(int32_t wheelRsrcId);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);
private:
    Image _wheelImg;
};


#endif // INCLUDE_GAME_ENTITIES_WHEEL_H_