#ifndef UTILS_DRAWING_DRAWPARAMS_H_
#define UTILS_DRAWING_DRAWPARAMS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/Point.h"
#include "utils/drawing/Rectangle.h"
// Forward Declarations

// "inline" tells us that this variable, even located in a header file...
// ...will be created only at "one place" (the first source file that includes the header file)...
//... and any other place where we use the variable will refer to this "one place" 
// "inline" is handy whenever we DO NOT want to create a separate source file just for defining/initializing a variable
inline constexpr auto INVALID_RSRC_ID=-1;
inline constexpr auto FULL_OPACITY=255;
inline constexpr auto ZERO_OPACITY=0;
inline constexpr auto FULL_ROTATION=360.0;

enum class WidgetFlip:uint8_t{
    // the purpose of this enum is to have equivalent values to "SDL_RendererFlip"
    NONE,
    HORIZONTAL,
    VERTICAL,
    HORIZONTAL_AND_VERTICAL // equivalent to HORIZONTAL | VERTICAL -> bitwise OR-ing
};

enum class WidgetType:uint8_t {
    IMAGE,
    TEXT,
    FBO,
    UNKNOWN
};

enum class BlendMode:uint8_t {
    NONE=0, // value for SDL_BLENDMODE_NONE
    BLEND=1, // value for SDL_BLENDMODE_BLEND
    ADD=2, // value for SDL_BLENDMODE_ADD
    MOD=4 // value for SDL_BLENDMODE_MODE
};


struct DrawParams{
    // despite the fact that this is POD (=Plain Old Data), not a problem at all to have one function
    void reset();

    Rectangle frameRect=Rectangle::ZERO;

    // Top left position of texture
    Point pos=Point::UNDEFINED;

    double rotationAngle=0.0;
    Point rotationCenter=Point::ZERO;

    // Draw dimensions of the texture
    int32_t width=0;
    int32_t height=0;

    int32_t opacity=FULL_OPACITY;

    // unique identificator of a resource
    union{
        // with "union" we say that rsrcId and textId together share the same piece of memory...
        // ... BUT there is a place for only one "int32_t" and depending on which we choose lastly...
        // ... the memory iis represented by it
        // Both are mutually interchangeable, have the same meaning and physical piece of memory
        // Without the "union" the structure is forced to carry the two integers in the memory simultaneously 
        int32_t rsrcId=INVALID_RSRC_ID;
        int32_t textId;
        int32_t fboId;
    };

    WidgetType widgetType=WidgetType::UNKNOWN;
    WidgetFlip flipType=WidgetFlip::NONE;
};

#endif // UTILS_DRAWING_DRAWPARAMS_H_