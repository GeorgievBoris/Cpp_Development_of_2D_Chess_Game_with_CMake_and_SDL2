#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_IMAGE_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_IMAGE_H_


// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Widget.h"
// Forward Declarations

class Image : public Widget {
public:
    ~Image();
    void create(int32_t rsrcId, const Point& pos=Point::ZERO);
    void destroy();
private:

};



#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_IMAGE_H_