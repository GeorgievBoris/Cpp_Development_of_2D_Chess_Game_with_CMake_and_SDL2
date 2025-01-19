// Corresponding header
#include "manager_utils/drawing/Image.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/RsrcMgr.h"

Image::~Image(){
    if(_isCreated && !_isDestroyed){
        destroy();
    }
}

void Image::create(int32_t rsrcId, const Point& pos){
    if(_isCreated){
        std::cerr<<"Error, image with rsrcId: "<<rsrcId<<" was already created. Will not create twice."<<std::endl;
        return;
    }
    _isCreated=true;
    _isDestroyed=false;
    const Rectangle& rect=gRsrcMgr->getImageFrame(rsrcId);
    _drawParams.rsrcId=rsrcId;
    _drawParams.width=rect.w;
    _drawParams.height=rect.h;
    _drawParams.pos=pos;
    _drawParams.widgetType=WidgetType::IMAGE;
}

void Image::destroy(){
    if(_isCreated){ 
        // _isCreated=false; // added by Zhivko, but probably not needed because Widget::reset() does the same job
        // _isDestroyed=true; // added by Zhivko, but probably not needed because Widget::reset() does the same job
        Widget::reset();
        return;
    }
    std::cerr<<"Error, image was already destroyed."<<std::endl;
}