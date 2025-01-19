// Corresponding header
#include "manager_utils/drawing/Text.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "manager_utils/managers/RsrcMgr.h"

Text::~Text(){
    if(_isCreated && !_isDestroyed){
        destroy();
    }
}

void Text::create(const std::string& text, int32_t fontId, const Color& color, const Point& pos){
    if(_isCreated){
        std::cerr<<"Error, text with fontId: "<<fontId <<" was already created. Will not create twice."<<std::endl;
        return;
    }
    _isCreated=true;
    _isDestroyed=false;
    _textContent=text;
    _color=color;
    _fontId=fontId;
    gRsrcMgr->createText(_textContent.c_str(),_color,_fontId,_drawParams.textId,_drawParams.width,_drawParams.height);
    _drawParams.pos=pos;
    _drawParams.widgetType=WidgetType::TEXT;
}

void Text::destroy(){
    if(_isCreated){
        // _isCreated=false; // added by Zhivko, but probably not needed because Widget::reset() does the same job
        // _isDestroyed=true; // added by Zhivko, but probably not needed because Widget::reset() does the same job

        if(gRsrcMgr){ // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
            gRsrcMgr->unloadText(_drawParams.textId); // added by Zhivko as a fix in the beginning of Lecture 11 Game 2/4
        }
        
        Widget::reset();
        return;
    }
    std::cerr<<"Error, text was already destroyed"<<std::endl;
}

void Text::setText(const std::string& text){
    if(text==_textContent){
        return;
    }
    _textContent=text;
    gRsrcMgr->reloadText(_textContent.c_str(),_color,_fontId,_drawParams.textId,_drawParams.width,_drawParams.height);
}

void Text::setColor(const Color& color){
    if(color==_color){
        return;
    }
    _color=color;
    gRsrcMgr->reloadText(_textContent.c_str(),_color,_fontId,_drawParams.textId,_drawParams.width,_drawParams.height);
}

std::string Text::getTextContent() const{
    return _textContent;
}