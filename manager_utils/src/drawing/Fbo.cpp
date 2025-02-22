// Corresponding header
#include "manager_utils/drawing/Fbo.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/RsrcMgr.h"
#include "manager_utils/managers/DrawMgr.h"


// boost and qt are third - party libraries -> remember them !!!
// listen again around 1:40:00 -> Martin's Question !!!!!!!!

Fbo::~Fbo(){
    if(_isCreated && !_isDestroyed){
        destroy();
    }
}

void Fbo::create(int32_t width, int32_t height, const Point& pos, const Color& clearColor){
    if(_isCreated){
        std::cerr<<"Error, Fbo with fboId: "<<_drawParams.fboId
                 <<" was already created. Will not create twice"<<std::endl;
        return;
    }
    _isCreated=true;
    _isDestroyed=false;
    _clearColor=clearColor;

    _drawParams.width=width;
    _drawParams.height=height;

    _drawParams.frameRect.x=0;
    _drawParams.frameRect.y=0;
    _drawParams.frameRect.w=_drawParams.width;
    _drawParams.frameRect.h=_drawParams.height;

    _drawParams.pos=pos;
    _drawParams.widgetType=WidgetType::FBO;

    gRsrcMgr->createFbo(width,height,_drawParams.fboId);
}

void Fbo::destroy(){
    if(!_isCreated){
        std::cerr<<"Error, Fbo with fboId: "<<_drawParams.fboId
                 <<" was already destroyed"<<std::endl;
        return;
    }

    // _isCreated=false; // Zhivko added them, but no point to do so, because Widget::reset() takes care of updating the values
    // _isDestroyed=true; // Zhivko added them, but no point to do so, because Widget::reset() takes care of updating the values

    _storedItems.clear();
    _clearColor=Colors::BLACK;
    _isLocked=true;

    // // Note: Zhivko did NOT add it... I think that it makes sense to be added?
    // gRsrcMgr->unloadFbo(_drawParams.fboId);

    Widget::reset();
}

void Fbo::unlock(){
    if(_isLocked){
        _isLocked=false;
        if(EXIT_SUCCESS!=gDrawMgr->unlockRenderer()){
            std::cerr<<"gDrawMgr->unlockRenderer() failed"<<std::endl;
        }
        gDrawMgr->setRendererTarget(_drawParams.fboId);
        return;
    }
    std::cerr<<"Error, Fbo was already not locked in the first place"<<std::endl;
}

void Fbo::lock(){
    if(_isLocked){
        std::cerr<<"Error, Fbo was already locked"<<std::endl;
        return;
    }
    _isLocked=true;

    if(EXIT_SUCCESS!=gDrawMgr->lockRenderer()){
        std::cerr<<"gDrawMgr->lockRenderer() failed"<<std::endl;
    }
    gDrawMgr->resetRendererTarget();
}

void Fbo::reset(){
    gDrawMgr->clearCurrentRendererTarget(_clearColor);
    _storedItems.clear(); // listen from 1:37:00 for 2 minues - Lecture Game 4/4
}

void Fbo::addWidget(const Widget& widget){
    if(!widget.isCreated()){
        std::cerr<<"Error, trying to add a non-created widget for FBO with id: "
            <<_drawParams.fboId<<std::endl;
        return;
    }
    _storedItems.push_back(widget.getDrawParams());
}

void Fbo::update() const {

    // listen again to 1:36:00 (Around) part of Lecture Game 4/4 !!!

    for(const DrawParams& drawParam:_storedItems){
        gDrawMgr->addDrawCmd(drawParam);
    }
}


