// Corresponding header
#include "utils/drawing/DrawParams.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers


void DrawParams::reset(){
    pos=Point::UNDEFINED;
    width=0;
    height=0;
    opacity=FULL_OPACITY;
    rsrcId=INVALID_RSRC_ID;
    widgetType=WidgetType::UNKNOWN;
}