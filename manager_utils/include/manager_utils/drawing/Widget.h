#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_WIDGET_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_WIDGET_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "utils/drawing/DrawParams.h"
// Forward Declarations

class Widget{
public:
    void draw() const;

    void reset();
    void setFlipType(WidgetFlip flipType);
    void setWidth(int32_t width);
    void setHeight(int32_t height);
    void setRotationCenter(const Point& rotCenter);
    void setRotationAngle(double angle);
    void setPosition(const Point& pos);
    void setPosition(int32_t x, int32_t y);
    void setOpacity(int32_t opacity);
    
    int32_t getWidth() const;
    int32_t getHeight() const;
    double getRotationAngle() const;
    int32_t getOpacity() const;
    Point getPosition() const;
    bool isVisible() const;

    void activateAlphaModulation();
    void deactivateAlphaModulation();

    void show();
    void hide();

    void moveRight(int32_t delta);
    void moveLeft(int32_t delta);
    void moveUp(int32_t delta);
    void moveDown(int32_t delta);

    void rotateRight(double delta);
    void rotateLeft(double delta);

    bool containsPoint(const Point& pos) const;
    bool isCreated() const;

    DrawParams getDrawParams() const;

protected:
    DrawParams _drawParams;
    bool _isCreated=false;
    bool _isDestroyed=true;
    bool _isVisible=true;
    bool _isAlphaModulationEnabled=false;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_DRAWING_WIDGET_H_