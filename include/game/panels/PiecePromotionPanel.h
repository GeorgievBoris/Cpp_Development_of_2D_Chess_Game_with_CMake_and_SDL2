#ifndef INCLUDE_GAME_PANELS_PIECEPROMOTIONPANEL_H_
#define INCLUDE_GAME_PANELS_PIECEPROMOTIONPANEL_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <array>
// Third-party headers
// Own headers
#include "game/panels/buttons/PiecePromotionButton.h"
#include "game/panels/config/PiecePromotionPanelCfg.h"
// Forward Declarations
class GameProxy;
class InputEvent;


class PiecePromotionPanel{
public:
    int32_t init(const PiecePromotionPanelCfg& cfg, GameProxy* gameProxy);
    void handleEvent(const InputEvent& e);
    void draw() const;
    void activate(int32_t playerId);
    bool isActive() const;

private:
    void onButtonClicked(PieceType pieceType);

    enum InternalDefines{
        BUTTONS_COUNT=4
    };
    std::array<PiecePromotionButton,BUTTONS_COUNT> _promotionBtns;
    GameProxy* _gameProxy=nullptr;
    bool _isActive=false;

};


#endif // INCLUDE_GAME_PANELS_PIECEPROMOTIONPANEL_H_