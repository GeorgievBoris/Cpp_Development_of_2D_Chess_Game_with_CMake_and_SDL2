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
    
    void setFrame(int32_t frameIdx);
    void setNextFrame();
    void setPrevFrame();
    int32_t getFrame() const;
private:
    int32_t _currFrame{0};
    int32_t _maxFrames{0};
};



#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_IMAGE_H_