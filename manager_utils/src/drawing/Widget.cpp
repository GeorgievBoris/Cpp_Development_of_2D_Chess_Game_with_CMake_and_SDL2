// Corresponding header
#include "manager_utils/drawing/Widget.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/DrawMgr.h"

void Widget::draw(){
    if(_isVisible){
        gDrawMgr->addDrawCmd(_drawParams);
    }
}

void Widget::reset(){
    _isCreated=false;
    _isDestroyed=true;
    _isVisible=true;
    _isAlphaModulationEnabled=false;
    _drawParams.reset();
}

void Widget::setWidth(int32_t width){
    _drawParams.width=width;
}

void Widget::setHeight(int32_t height){
    _drawParams.height=height;
}

void Widget::setPosition(const Point& pos){
    _drawParams.pos=pos;
}

void Widget::setPosition(int32_t x, int32_t y){
    _drawParams.pos.x=x;
    _drawParams.pos.y=y;
}

void Widget::setOpacity(int32_t opacity){
    if(_isAlphaModulationEnabled){
        _drawParams.opacity=opacity;
        gDrawMgr->setWidgetOpacity(_drawParams,opacity);
        return;
    }
    std::cerr<<"alpha modulation was not enabled for rsrcId: "<<_drawParams.rsrcId
                <<". Will not change opacity"<<std::endl;
}

int32_t Widget::getWidth() const{
    return _drawParams.width;
}

int32_t Widget::getHeight() const{
    return _drawParams.height;
}

int32_t Widget::getOpacity() const{
    return _drawParams.opacity;
}

Point Widget::getPosition() const{
    return _drawParams.pos;
}

void Widget::activateAlphaModulation(){
    if(_isAlphaModulationEnabled){
        std::cerr<<"alpha modulation was already enabled for rsrcId: "<<_drawParams.rsrcId<<std::endl;
        return;
    }
    _isAlphaModulationEnabled=true;
    gDrawMgr->setWidgetBlendMode(_drawParams,BlendMode::BLEND);
}

void Widget::deactivateAlphaModulation(){
    if(_isAlphaModulationEnabled){
        _isAlphaModulationEnabled=false;
        gDrawMgr->setWidgetBlendMode(_drawParams,BlendMode::NONE);
        return;
    }
    std::cerr<<"alpha modulation was originally not enabled for rsrcId: "<<_drawParams.rsrcId<<std::endl;
}

void Widget::show(){
    _isVisible=true;
}

void Widget::hide(){
    _isVisible=false;
}

void Widget::moveLeft(int32_t delta){
    _drawParams.pos.x-=delta;
}

void Widget::moveRight(int32_t delta){
    _drawParams.pos.x+=delta;
}

void Widget::moveUp(int32_t delta){
    _drawParams.pos.y-=delta;
}

void Widget::moveDown(int32_t delta){
    _drawParams.pos.y+=delta;
}