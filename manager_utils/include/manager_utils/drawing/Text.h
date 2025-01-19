#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_TEXT_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_TEXT_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <string>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Widget.h"
#include "utils/drawing/Color.h"
// Forward Declarations

class Text : public Widget {
public:
    ~Text();
    void create(const std::string& text, int32_t fontId, const Color& color, const Point& pos=Point::ZERO);
    void destroy();
    void setText(const std::string& text);
    void setColor(const Color& color);
    std::string getTextContent() const;
private:
    std::string _textContent;
    Color _color=Colors::BLACK;
    int32_t _fontId=INVALID_RSRC_ID;
};



#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_TEXT_H_