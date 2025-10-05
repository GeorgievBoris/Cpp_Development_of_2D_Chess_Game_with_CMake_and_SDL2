#ifndef GAME_GAME_H_
#define GAME_GAME_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <functional> // added by me !!!!
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
#include "game/animations/WinnerAnimator.h" // NOT added by Zhivko

#include "game/buttons/QuitGameButton.h" // NOT added by Zhivko

// Forward Declarations
class InputEvent;

class Game : public GameProxy {
public:
    int32_t init(const GameCfg& cfg, const std::function<void()>& showStartScreenCallBack);
    void deinit();
    void draw() const;
    void handleEvent(InputEvent& e);
    void show(); // Game::show() is NOT added by Zhivko
    void hide(); // Game::hide() is NOT added by Zhivko
    void start(); // Game::start() is NOT added by Zhivko
    void startPlayersTimer(); // Game::startPlayersTimer() is NOT added by Zhivko
private:
    // IMPORTANT write this down: "onGameTurnFinished()" can be called ONLY by someone who has the proxy...
    //... because the proxy has the method as a PUBLIC member!!!!
    void onGameTurnFinished() final;
    void onPawnPromotion() final;
    void promotePiece(PieceType pieceType) final;
    void onBoardAnimFinished() final;
    void setWidgetFlip(WidgetFlip flipType) final;
    void restart(); // Game::restart() is NOT added by Zhivko

    void regenerateGameFbo();

    void correctInputEvent(InputEvent& e); // Game::correctInputEvent() is NOT added by Zhivko

    void castleTextShow() final; // Game::castleTextShow() is NOT added by Zhivko
    void castleTextHide() final; // Game::castleTextHide() is NOT added by Zhivko
    bool isGameBoardAnimatorActive() const final; // Game::isGameBoardAnimatorActive() method is NOT added by Zhivko
    bool isPromotionActive() const final; // Game::isPromotionActive() method is NOT added by Zhivko
    void setPieceMovementActive(bool isPieceMovementActive) final; // Game::setPieceMovementActive() method is NOT added by Zhivko
    bool isPieceMovementActive() const final; // Game::isPieceMovementActive() method is NOT added by Zhivko
    void setGameEndType(const GameEndType gameEndType) final; // Game::setGameEndType() method is NOT added by Zhivko
    GameEndType getGameEndType() const final; // Game::getGameEndType() method is NOT added by Zhivko

    // NOTE: think about grouping GameBoardAnimator, WinnerAnimator and PieceAnimator into one Animator classs
    GameBoard _gameBoard;
    PieceHandler _pieceHandler;
    GameLogic _gameLogic;
    PiecePromotionPanel _piecePromotionPanel;
    GameBoardAnimator _gameBoardAnimator;
    InputInverter _inputInverter;
    Fbo _gameFbo;

    QuitGameButton _quitGameBtn; // NOT added by Zhivko
    WinnerAnimator _winnerAnimator; // NOT added by Zhivko

    bool _isPromotionActive=false; // a quick fix by Zhivko done in the last lecture 14
    bool _isGameHidden=true; // NOT added by Zhivko
    bool _isPieceMovementActive=false; // NOT added by Zhivko
    GameEndType _gameEndType=GameEndType::NONE; // NOT added by Zhivko

    // NOTE: write down !!!!
    // Code refactoring means : to distribute functionalities amongst different classes...
    // ... so that eventually we do not get source files that contain many, many lines of code
    
};


#endif // GAME_GAME_H_