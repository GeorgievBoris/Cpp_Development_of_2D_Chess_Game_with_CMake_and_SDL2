#ifndef UTILS_DRAWING_COLOR_H_
#define UTILS_DRAWING_COLOR_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declaration

struct RGBA{
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
    uint8_t a{255};
};

class Color{
public:
    Color(const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha=255);
    Color()=delete;
    bool operator==(const Color& other) const;
    bool operator!=(const Color& other) const;

    RGBA rgba;
};

namespace Colors{
    extern const Color RED;
    extern const Color GREEN;
    extern const Color BLUE;
    extern const Color BLACK;
    extern const Color WHITE;
    extern const Color GRAY;
    extern const Color CYAN;
    extern const Color MAGENTA;
    extern const Color YELLOW;
    extern const Color ORANGE;
    extern const Color PURPLE;
    extern const Color FULL_TRANSPARENT;
}


#endif // UTILS_DRAWING_COLOR_H_