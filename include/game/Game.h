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
#include "game/board/GameBoard.h"
#include "game/pieces/PieceHandler.h"
// Forward Declarations
class InputEvent;

class Game{
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw() const;
    void handleEvent(const InputEvent& e);
private:
    GameBoard _gameBoard;
    PieceHandler _pieceHandler;
};


#endif // GAME_GAME_H_