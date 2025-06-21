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
    // TODO: an optimization will be to have a "const" pointer to "frames" and in this way do not call each time "gRsrcMgr" 
    const Frames& frames=gRsrcMgr->getImageFrame(rsrcId);
    _maxFrames=static_cast<int32_t>(frames.size());
    const Rectangle& firstFrame=frames.front();
    _drawParams.frameRect=firstFrame;
    _drawParams.rsrcId=rsrcId;
    _drawParams.width=firstFrame.w;
    _drawParams.height=firstFrame.h;
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

void Image::setFrame(int32_t frameIdx){
    if(0>frameIdx || _maxFrames<=frameIdx){
        std::cerr<<"Error, trying to set invalid frameIdx: "<<frameIdx
                    <<" for Image with rsrcId: "<<_drawParams.rsrcId <<std::endl;
        return;
    }
    _currFrame=frameIdx;
    const Frames& frames=gRsrcMgr->getImageFrame(_drawParams.rsrcId);
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    _drawParams.frameRect=frames[static_cast<size_t>(_currFrame)];


    _drawParams.width=_drawParams.frameRect.w; // NOT added by Zhivko
    _drawParams.height=_drawParams.frameRect.h; // NOT added by Zhivko   
}

void Image::setNextFrame(){
    ++_currFrame;
    if(_maxFrames==_currFrame){
        _currFrame=0;
    }
    const Frames& frames=gRsrcMgr->getImageFrame(_drawParams.rsrcId);
    // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    _drawParams.frameRect=frames[static_cast<size_t>(_currFrame)];

    _drawParams.width=_drawParams.frameRect.w; // NOT added by Zhivko
    _drawParams.height=_drawParams.frameRect.h; // NOT added by Zhivko  

}

void Image::setPrevFrame(){
    --_currFrame;
    if(-1==_currFrame){
        _currFrame=_maxFrames-1;
    }
     const Frames& frames=gRsrcMgr->getImageFrame(_drawParams.rsrcId);
     // the casting is done here, because if the clang++ compiler is used instead of g++, it will throw a warning/error
    _drawParams.frameRect=frames[static_cast<size_t>(_currFrame)];

    _drawParams.width=_drawParams.frameRect.w; // NOT added by Zhivko
    _drawParams.height=_drawParams.frameRect.h; // NOT added by Zhivko  

}

int32_t Image::getFrame()const{
    return _currFrame;
}

int32_t Image::getMaxFrame() const{ // NOT added by Zhivko
    return _maxFrames;
}