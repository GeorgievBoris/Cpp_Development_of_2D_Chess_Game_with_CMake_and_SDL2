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
// Forward Declarations
class InputEvent;
struct SDL_Texture;



class Game {
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw(std::vector<SDL_Texture*>& outImages);
    void handleEvent(const InputEvent& e);

private:

    int32_t loadResources(const std::unordered_map<GameImages,std::string>& res);
    SDL_Texture* _currChosenImage=nullptr;
    SDL_Texture* _imageSurfaces[COUNT];
};


#endif // GAME_GAME_H_