// Corresponding header
#include "manager_utils/input/ButtonBase.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"

void ButtonBase::draw() const{
    Image::draw();
    // draw(); // NOTE: just "draw();" will cause an infinite loop !!!
    
}

void ButtonBase::lockInput(){
    _isInputUnlocked=false;
    // Image::setFrame(DISABLED); // Original code used by Zhivko!
    Image::setFrame(UNCLICKED); // added by me, NOT by Zhivko !
}

void ButtonBase::unlockInput(){
    _isInputUnlocked=true;
    // Image::setFrame(UNCLICKED); // Original code used by Zhivko!
    Image::setFrame(UNCLICKED); // added by me, NOT by Zhivko !
}

void ButtonBase::destroy(){
    _isInputUnlocked=true;
    Image::destroy();
}

bool ButtonBase::isInputUnlocked() const{
    return _isInputUnlocked;
}

bool ButtonBase::containsEvent(const InputEvent& e) const{
    return Image::containsPoint(e.pos);
}