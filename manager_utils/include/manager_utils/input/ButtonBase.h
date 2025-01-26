#ifndef MANAGER_UTILS_INCLUDE_MANAGER_UTILS_INPUT_BUTTONBASE_H_
#define MANAGER_UTILS_INCLUDE_MANAGER_UTILS_INPUT_BUTTONBASE_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/drawing/Image.h"
// Forward Declarations
class InputEvent;

enum ButtonStates{
    UNCLICKED, CLICKED, DISABLED
};
// Variants 1 and 2 exist:
// Variant 1: have the "Image" as a member of the class. This variant is cleaner because...
// ... it provides us with additional functionalities. Drawback is that we could possibly need to write again...
// ... the same setters and getter that we have for "Widget". So to save time, apply Variant 2
// Variant 2: inherit the "Image"
class ButtonBase : public Image {
public:
    virtual ~ButtonBase()=default;
    virtual void handleEvent(const InputEvent& e) =0; // a pure-virtual method
    // void create(int32_t rsrcId, const Point& pos=Point::ZERO); // must be used if Variant 1 is used
    virtual void draw();
    void lockInput();
    void unlockInput();

    bool isInputUnlocked() const;
    bool containsEvent(const InputEvent& e);

private:
    // Image _buttonTexture; // this is related to Variant 1 as described above
    // Rectangle _boundRect=Rectangle::ZERO; // this variable could be used to cearly define the real borders/limits of our button -> for scenarious when "graphically / visually" our button is not the entire file (.png, .jpg, etc...) that is loaded.
    bool _isInputUnlocked=true;
};


#endif // MANAGER_UTILS_INCLUDE_MANAGER_UTILS_INPUT_BUTTONBASE_H_