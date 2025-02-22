#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
// Third-party headers
// Own headers

#include "game/config/GameCfg.h"
#include "game/board/GameBoard.h"
#include "game/board/GameBoardAnimator.h"
#include "game/pieces/PieceHandler.h"
#include "game/logic/GameLogic.h"
#include "game/proxies/GameProxy.h"
#include "game/panels/PiecePromotionPanel.h"
#include "game/logic/InputInverter.h"
#include "manager_utils/drawing/Image.h"
#include "manager_utils/drawing/Fbo.h"

// Forward Declarations
class InputEvent;

class Game : public GameProxy {
public:
    int32_t init(const GameCfg& cfg);
    void deinit();
    void draw() const;
    void handleEvent(InputEvent& e);
private:
    // IMPORTANT write this down: "onGameTurnFinished()" can be called ONLY by someone who has the proxy...
    //... because the proxy has the method as a PUBLIC member!!!!
    void onGameTurnFinished() final;
    void onPawnPromotion() final;
    void promotePiece(PieceType pieceType) final;
    void onBoardAnimFinished() final;
    void setWidgetFlip(WidgetFlip flipType) final;

    void regenerateGameFbo();

    GameBoard _gameBoard;
    PieceHandler _pieceHandler;
    GameLogic _gameLogic;
    PiecePromotionPanel _piecePromotionPanel;
    GameBoardAnimator _gameBoardAnimator;
    InputInverter _inputInverter;
    Fbo _gameFbo;

    // NOTE: write down !!!!
    // Code refactoring means : to distribute functionalities amongst different classes...
    // ... so that eventually we do not get source files that contain many, many lines of code
};


#endif // GAME_GAME_H_