#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers
#include "game/config/GameCfg.h"
#include "manager_utils/drawing/Image.h"
#include "manager_utils/drawing/Text.h"
// Forward Declarations
class InputEvent;

class Game {
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw();
    void handleEvent(const InputEvent& e);

private:
    Image _pressKeysImg;
    Image _layer2Img;
    
    Text _helloText;
    Text _pressText;
    Text _hideText;

    void setMousePosText(const Point& mousePos);
    Text _mousePosText;
};


#endif // GAME_GAME_H_