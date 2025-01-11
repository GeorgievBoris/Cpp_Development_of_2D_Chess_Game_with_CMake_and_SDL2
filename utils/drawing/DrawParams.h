#ifndef UTILS_DRAWING_DRAWPARAMS_H_
#define UTILS_DRAWING_DRAWPARAMS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/Point.h"
// Forward Declarations

// "inline" tells us that this variable, even located in a header file...
// ...will be created only at "one place" (the first source file that includes the header file)...
//... and any other place where we use the variable will refer to this "one place" 
// "inline" is handy whenever we DO NOT want to create a separate source file just for defining/initializing a variable
inline constexpr auto INVALID_RSRC_ID=-1;
inline constexpr auto FULL_OPACITY=255;
inline constexpr auto ZERO_OPACITY=0;

enum class BlendMode:uint8_t {
    NONE=0, // value for SDL_BLENDMODE_NONE
    BLEND=1, // value for SDL_BLENDMODE_BLEND
    ADD=2, // value for SDL_BLENDMODE_ADD
    MOD=4 // value for SDL_BLENDMODE_MODE
};


struct DrawParams{
    // Top left position of texture
    Point pos=Point::UNDEFINED;

    // Draw dimensions of the texture
    int32_t width=0;
    int32_t height=0;

    int32_t opacity=FULL_OPACITY;

    // unique identificator of a resource
    int32_t rsrcId=INVALID_RSRC_ID;
};

#endif // UTILS_DRAWING_DRAWPARAMS_H_