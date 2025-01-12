#ifndef UTILS_DRAWING_POINT_h_
#define UTILS_DRAWING_POINT_h_

// C system headers
#include <cstdint>
// C++ system headers
// Third party headers
// Own headers
// Foward Declarations

struct Point {
    Point()=default;
    Point(int32_t inputX, int32_t inputY); 

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;

    int32_t x=0;
    int32_t y=0;

    static const Point ZERO;
    static const Point UNDEFINED;
};

// // example of a "multiple definition" error -> uncomment in Rectangle.cpp, Rectangle.h and Point.cpp to see it 
// extern const Point ZERO;
// extern const Point UNDEFINED;



#endif // UTILS_DRAWING_POINT_h_