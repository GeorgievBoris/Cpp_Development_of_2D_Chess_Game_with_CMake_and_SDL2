// Corresponding header
#include "Point.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers

// // example of a "multiple definition" error -> uncomment in Rectangle.cpp, Rectangle.h and Point.h to see it 
// const Point ZERO(0,0);
// const Point UNDEFINED(10000,10000);

const Point Point::ZERO(0,0);
const Point Point::UNDEFINED(10000,10000);


Point::Point(int32_t inputX, int32_t inputY) : x(inputX), y(inputY) {}

bool Point::operator==(const Point& other) const {
    return other.x==x && other.y==y;
}

bool Point::operator!=(const Point& other) const {
    return !operator==(other);
    // return !(*this==other); // another option - dereferencing "this"
}