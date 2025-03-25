#ifndef INCLUDE_GAME_LOGIC_INPUTINVERTER_H_
#define INCLUDE_GAME_LOGIC_INPUTINVERTER_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
// Forward Declarations
class InputEvent;

// NOTE: to be 100% correct this class must be in the "sdl_utils" library because it is related to the input

class InputInverter{
public:
    int32_t init(int32_t boardWidth, int32_t boardHeight);
    void setBoardFlipType(WidgetFlip flipType);
    void invertEvent(InputEvent& inputEvent);
    void restart(); // added by me
private:
    int32_t _boardWidth{0};
    int32_t _boardHeight{0};
    WidgetFlip _flipType=WidgetFlip::NONE;

};


#endif // INCLUDE_GAME_LOGIC_INPUTINVERTER_H_