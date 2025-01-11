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

class Game {
public:
    int32_t init(const GameCfg& cfg,const ImageContainer* imageContainerInterface);
    void deinit();
    void draw(std::vector<DrawParams>& outImages);
    void handleEvent(const InputEvent& e);

private:
    DrawParams _pressKeysImg;
    DrawParams _pressKeysImgDuplicate; // for testing purposes - to check if opacity changes for the created SDL_Texture
    DrawParams _layer2Img;
    // TODO remove me!!!
    const ImageContainer* _imageContainer=nullptr;
};


#endif // GAME_GAME_H_