// Corresponding header
#include "Rectangle.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers

// // example of a "multiple definition" error -> uncomment in Rectangle.h, Point.cpp and Point.h to see it 
// const Rectangle ZERO(0,0,0,0);
// const Rectangle UNDEFINED(10000,10000,10000,10000);


const Rectangle Rectangle::ZERO(0,0,0,0);
const Rectangle Rectangle::UNDEFINED(10000,10000,10000,10000);

Rectangle::Rectangle(int32_t inputX, int32_t inputY, int32_t inputWidth, int32_t inputHeight):
    x(inputX), y(inputY), w(inputWidth), h(inputHeight) {}

bool Rectangle::operator==(const Rectangle& other) const {
    return other.x==x && other.y==y && other.w==w && other.h==h;
}

bool Rectangle::operator!=(const Rectangle& other) const {
    return !operator==(other);
}

bool Rectangle::isPointInside(const Point& point) const {
    const bool isInsideX=point.x>=x && point.x<x+w;
    const bool isInsideY=point.y>=y && point.y<y+h;
    return isInsideX && isInsideY;
}