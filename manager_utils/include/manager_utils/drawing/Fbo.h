#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_FBO_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_FBO_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <vector>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Widget.h"
#include "utils/drawing/Color.h"
// Forward Declarations

// NOTE: write this down...
// The piece of advice : Apply Frame Buffer Object(s) only when the whole application is...
// ... ready/implemented. When the application is ready apply FBO as an optimization.
// The other possibility is to apply FBO where we want to achieve advanced animations.

// a Frame Buffer Object is also called a sprite buffer !!!
class Fbo : public Widget {
public:
    ~Fbo();
    void create(int32_t width, int32_t height, const Point& pos=Point::ZERO,
                const Color& clearColor=Colors::BLACK);
    void destroy();
    void unlock();
    void lock();
    void reset();
    void addWidget(const Widget& widget);
    void update() const;
private:
    std::vector<DrawParams> _storedItems;
    Color _clearColor=Colors::BLACK;
    bool _isLocked=true;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_FBO_H_