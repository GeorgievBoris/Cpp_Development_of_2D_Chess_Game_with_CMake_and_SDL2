#ifndef INCLUDE_GAME_SAVER_GAMESAVER_H_
#define INCLUDE_GAME_SAVER_GAMESAVER_H_


// C system headers
#include <cstdint>
// C++ system headers
#include <functional>
#include <iosfwd>
// Third-party headers
// Own headers
#include "manager_utils/input/ButtonBase.h"
// Forward Declarations
class InputEvent;
class PieceHandlerProxy;

// "SaveGameButton.h" is NOT added by Zhivko !

class SaveGameButton : public ButtonBase{
public:
    int32_t init(int32_t rsrcId, PieceHandlerProxy* pieceHandlerProxy,const std::function<void(std::ofstream&)>& funcCallback);
    void handleEvent(const InputEvent& e) final;
    void restart();
private:
    PieceHandlerProxy* _pieceHandlerProxy=nullptr;
    std::function<void(std::ofstream&)> _funcCallback;
};



#endif // INCLUDE_GAME_SAVER_GAMESAVER_H_