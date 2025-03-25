#ifndef INCLUDE_GAME_PANELS_BUTTONS_PIECEPROMOTIONBUTTON_H_
#define INCLUDE_GAME_PANELS_BUTTONS_PIECEPROMOTIONBUTTON_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/input/ButtonBase.h"
#include "game/panels/config/PiecePromotionButtonCfg.h"
#include "manager_utils/drawing/Image.h"
// Forward Declarations
class InputEvent;

class PiecePromotionButton : public ButtonBase{
public:
    int32_t init(const PiecePromotionButtonCfg& cfg);
    void draw() const final;
    void handleEvent(const InputEvent& e) final;
    void activate(int32_t activePlayerId);
    void showBtn(); // PiecePromotionButton::showBtn() is NOT added by Zhivko
    void hideBtn(); // PiecePromotionButton::hideBtn() is NOT added by Zhivko
private:
    Image _bgrImg;
    PiecePromotionButtonCfg _cfg;
};



#endif // INCLUDE_GAME_PANELS_BUTTONS_PIECEPROMOTIONBUTTON_H_