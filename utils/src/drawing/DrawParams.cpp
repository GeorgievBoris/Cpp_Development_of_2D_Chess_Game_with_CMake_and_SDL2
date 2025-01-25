// Corresponding header
#include "utils/drawing/DrawParams.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers


void DrawParams::reset(){
    frameRect=Rectangle::ZERO;
    pos=Point::UNDEFINED;
    rotationAngle=0.0;
    rotationCenter=Point::ZERO;
    width=0;
    height=0;
    opacity=FULL_OPACITY;
    rsrcId=INVALID_RSRC_ID;
    widgetType=WidgetType::UNKNOWN;
    flipType=WidgetFlip::NONE;
}