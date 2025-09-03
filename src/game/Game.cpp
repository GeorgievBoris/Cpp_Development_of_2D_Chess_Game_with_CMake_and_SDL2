// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "utils/drawing/Color.h"
#include "game/defines/ChessDefines.h"

extern const int32_t GAME_X_POS_SHIFT;
extern const int32_t GAME_Y_POS_SHIFT;

int32_t Game::init(const GameCfg& cfg, const std::function<void()>& showStartScreenCb){
    if(EXIT_SUCCESS!=_gameBoard.init(cfg.chessBoardRsrcId, cfg.targetRsrcId,
                                            cfg.moveTilesRsrcId, cfg.blinkTargetTimerId, cfg.blinkEnPassantTimerId, cfg.blinkTileCastlingTimerId)){
        std::cerr<<"_gameBoard.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_pieceHandler.init(static_cast<GameBoardProxy*>(&_gameBoard), static_cast<GameProxy*>(this),
                                            cfg.whitePiecesRsrcId, cfg.blackPiecesRsrcId,
                                            cfg.whitePiecesHalvesRsrcId,cfg.blackPiecesHalvesRsrcId,
                                            cfg.movePieceTimerId, cfg.movePieceHalvesTimerId)){

        std::cerr<<"_pieceHandler.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_gameLogic.init(static_cast<PieceHandlerProxy*>(&_pieceHandler),cfg.playerTurnCapTimerId, cfg.playerTurnCapTextTimerId, 
                                        cfg.blinkTextCastlingTimerId, cfg.textFontId, cfg.quitGameButtonRsrcId)){

        // this particular if statement is NOT added by Zhivko
        std::cerr<<"_gameLogic.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_piecePromotionPanel.init(cfg.piecePromotionPanelCfg,
                                                                static_cast<GameProxy*>(this))){
        std::cerr<<"_piecePromotionPanel.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_quitGameBtn.init(cfg.quitGameButtonRsrcId,showStartScreenCb,
                                        [&](){_gameLogic.stopPlayersTimer();})){
        // NOT added by Zhivko
        std::cerr<<"_quitGameBtn.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameFbo.create(cfg.piecePromotionPanelCfg.gameBoardWidth, cfg.piecePromotionPanelCfg.gameBoardHeight,
                                    Point::ZERO, Colors::FULL_TRANSPARENT);
    _gameFbo.activateAlphaModulation();
    regenerateGameFbo();

    if(EXIT_SUCCESS!=_gameBoardAnimator.init(static_cast<GameProxy*>(this),&_gameFbo,cfg.gameFboRotTimerId)){
        std::cerr<<"_gameBoardAnimator.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_inputInverter.init(cfg.piecePromotionPanelCfg.gameBoardWidth,
                                            cfg.piecePromotionPanelCfg.gameBoardHeight)){
        std::cerr<<"_inputInverter.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_winnerAnimator.init(static_cast<PieceHandlerProxy*>(&_pieceHandler),showStartScreenCb,
                                                cfg.nextWinnerAnimTimerId, cfg.winnerAnimEndTimerId,cfg.winnerStarRsrcId,cfg.fireworksRsrcId,
                                                cfg.winnerMedalRsrcId, cfg.textFontId,cfg.windowWidth,cfg.windowHeight)){ 
        // NOT added by Zhivko
        std::cerr<<"_winnerAnimator.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }    

    Game::hide(); // NOT added by Zhivko

    return EXIT_SUCCESS;
}

void Game::deinit(){
    // _gameFbo.destroy();
}

void Game::draw() const{

    if(_isGameHidden){ // NOT added by Zhivko
        return; // NOT added by Zhivko
    }
    
    if(_gameBoardAnimator.isActive()){
        _gameBoard.drawGameBoardOnly();
    }
    _gameFbo.draw();
    _gameBoard.draw();
    if(_piecePromotionPanel.isActive()){
        _piecePromotionPanel.draw();
    }
    _gameLogic.draw(); // NOT added by Zhivko

    _quitGameBtn.draw(); // NOT added by Zhivko
    _pieceHandler.draw(); // NOT added by Zhivko
    if(_winnerAnimator.isActive()){ // NOT added by Zhivko
        _winnerAnimator.draw(); // NOT added by Zhivko
    } 
}

void Game::handleEvent(InputEvent& e){    

    if(!_gameLogic.isTimerActive()){ // NOT added by Zhivko
        if(!_piecePromotionPanel.isActive()){ // NOT added by Zhivko
            if(!_isGameFinished){
                return;
            }
        }
    }

    if(!_quitGameBtn.isInputUnlocked()){ // NOT added by Zhivko
        return;
    }

    _quitGameBtn.handleEvent(e); // NOT added by Zhivko

    if(_isGameFinished){ // NOT added by Zhivko
        return;
    }    
    
    if(_piecePromotionPanel.isActive()){
        _piecePromotionPanel.handleEvent(e);
        return;
    }

    correctInputEvent(e); // NOT added by Zhivko

    _inputInverter.invertEvent(e);
    _pieceHandler.handleEvent(e);
}

void Game::show(){ // Game::show() method is NOT added by Zhivko
    _isGameHidden=false;
    _gameFbo.show();
    _gameBoard.show();
    _quitGameBtn.show();
    _piecePromotionPanel.show();
}

void Game::hide(){ // Game::hide() method is NOT added by Zhivko
    _isGameHidden=true;
    _gameFbo.hide();
    _gameBoard.hide();
    _quitGameBtn.hide();
    _piecePromotionPanel.hide();
    _winnerAnimator.deactivate();
}


void Game::start(){ // Game::start() method is NOT added by Zhivko
    
    Game::show();

    constexpr int32_t initialTargetRotation=0; // degrees
    if(initialTargetRotation!=_gameBoardAnimator.getTargetRotation()){
        Game::restart();
        return;
    }

    if(_quitGameBtn.isInputUnlocked()){
        return;
    }

    _quitGameBtn.restart();
}

void Game::startPlayersTimer(){ // method Game::startPlayersTimer() is NOT added by Zhivko
    _gameLogic.startPlayersTimer();
}

void Game::onGameTurnFinished(){
    if(_isPromotionActive){
        return; // this "if" statement and the "return" are NOT added by Zhivko
    }
    _gameBoardAnimator.startAnim(_gameLogic.getActivePlayerId());
}

void Game::onPawnPromotion(){
    _isPromotionActive=true; // a quick fix by Zhivko done in the last lecture 14
    _piecePromotionPanel.activate(_gameLogic.getActivePlayerId());
}

void Game::promotePiece(PieceType pieceType){    
    _pieceHandler.promotePiece(pieceType);
    _isPromotionActive=false; // a quick fix by Zhivko done in the last lecture 14
    _gameBoardAnimator.startAnim(_gameLogic.getActivePlayerId()); // NOT added by Zhivko
}

void Game::onBoardAnimFinished(){
    if(_isPromotionActive){
        // a quick fix by Zhivko done in the last lecture 14
        return;
    }

    if(_isGameFinished){ // NOT added by Zhivko
        if(_isAutomaticWin){
            _gameLogic.finishTurn();
            _pieceHandler.setCurrentPlayerId(_gameLogic.getActivePlayerId());
        }
        _winnerAnimator.activate(_gameLogic.getActivePlayerId());
        regenerateGameFbo();
        return;
    }        
    _gameLogic.finishTurn();
    _pieceHandler.setCurrentPlayerId(_gameLogic.getActivePlayerId());
    _gameLogic.startPlayersTimer(); // NOT added by Zhivko
}

void Game::setWidgetFlip(WidgetFlip flipType){
    _pieceHandler.setWidgetFlip(flipType);
    _inputInverter.setBoardFlipType(flipType);
    _gameBoard.setWidgetFlip(flipType); // NOT added by Zhivko
    regenerateGameFbo();
}

void Game::restart(){ // Game::restart() method is NOT added by Zhivko
    if(EXIT_SUCCESS!=_pieceHandler.restart()){
        std::cerr<<"PieceHandler::restart() failed. The game cannot be restarted."<<std::endl;
        return;
    }
    regenerateGameFbo();
    _piecePromotionPanel.restart(); 
    _gameBoardAnimator.restart();
    _inputInverter.restart(); 
    _gameLogic.restart();
    _quitGameBtn.restart();
    _gameBoard.restart();
    _winnerAnimator.restart();
    _gameBoard.setWidgetFlip(WidgetFlip::NONE);    
    _isPromotionActive=false;
    _isGameFinished=false;
    _isAutomaticWin=false;
    _isPieceMovementActive=false;
}

void Game::regenerateGameFbo() {
    // regenerating the sprite buffer (fbo is also called a sprite buffer)

    _gameFbo.unlock();
    _gameFbo.reset();

    // Zhivko: "Should the _gameBoard be part of the _gameFbo? Both yes and no, but for the moment it should not be."
    _gameBoard.drawGameBoardOnFbo(_gameFbo);
    // if(_gameBoardAnimator.isActive()){
    //     _gameBoard.drawGameBoardOnFbo(_gameFbo);
    // }
    _pieceHandler.drawOnFbo(_gameFbo);
    
    _gameFbo.update();
    _gameFbo.lock();
}

void Game::correctInputEvent(InputEvent& e){ // Game::correctInputEvent() method is NOT added by Zhivko
    // Game::correctInputEvent() is called only just before the _pieceHandler.handleEvent() method is called
    if(TouchEvent::KEYBOARD_PRESS==e.type){
        return;
    }

    if(TouchEvent::KEYBOARD_RELEASE==e.type){
        return;
    }

    e.pos.x-=GAME_X_POS_SHIFT;
    e.pos.y-=GAME_Y_POS_SHIFT;
}

void Game::onGameFinish() { // Game::onGameFinish() is NOT added by Zhivko
    _isGameFinished=true;
}

void Game::castleTextShow() { // Game::castleTextShow() is NOT added by Zhivko
    _gameLogic.startOnCastleTimer();
}

void Game::castleTextHide(){ // Game::castleTextHide() is NOT added by Zhivko
    _gameLogic.stopOnCastleTimer();
}

void Game::setAutomaticWin(bool isAutomaticWin){ // GameProxy::setAutomaticWin() method is NOT added by Zhivko
    _isAutomaticWin=isAutomaticWin;
} 

bool Game::isAutomaticWin(){ // GameProxy::isAutomaticWin() method is NOT added by Zhivko
    return _isAutomaticWin;
}

bool Game::isWinnerAnimatorActive() { // Game::isWinnerAnimatorActive() method is NOT added by Zhivko
    return _winnerAnimator.isActive();
}; 

bool Game::isPromotionActive() { // Game::isPromotionActive() method is NOT added by Zhivko
    return _isPromotionActive;
}

void Game::setPieceMovementActive(bool isPieceMovementActive){ // Game::setPieceAnimationActive() method is NOT added by Zhivko
    _isPieceMovementActive=isPieceMovementActive;
    if(_isPieceMovementActive){
        _gameLogic.stopPlayersTimer(); 
        Game::regenerateGameFbo();
    }   
}

bool Game::isPieceMovementActive(){ // Game::isPieceMovementActive() method is NOT added by Zhivko
    return _isPieceMovementActive;
}