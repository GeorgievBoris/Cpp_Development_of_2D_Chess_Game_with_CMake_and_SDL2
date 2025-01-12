#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <vector>
// Third-party headers
// Own headers
#include "game/config/GameCfg.h"
#include "utils/drawing/DrawParams.h"
// Forward Declarations
class InputEvent;
class ImageContainer;
class TextContainer;

class Game {
public:
    int32_t init(const GameCfg& cfg,const ImageContainer* imageContainerInterface,
                                            TextContainer* textContainerInterface);
    void deinit();
    void draw(std::vector<DrawParams>& outImages);
    void handleEvent(const InputEvent& e);

private:
    DrawParams _pressKeysImg;
    DrawParams _layer2Img;
    
    DrawParams _helloText;
    DrawParams _pressText;
    DrawParams _hideText;

    bool isPressTextHidden=false;
    // TODO remove us!!!
    const ImageContainer* _imageContainer=nullptr;
    TextContainer* _textContainer=nullptr;
};


#endif // GAME_GAME_H_