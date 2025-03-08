// Corresponding header
#include "game/Game.h"
// C system headers
// C++ system headers
#include<iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "utils/drawing/Color.h"
#include "game/defines/ChessDefines.h"

int32_t Game::init(const GameCfg& cfg){
    if(EXIT_SUCCESS!=_gameBoard.init(cfg.chessBoardRsrcId, cfg.targetRsrcId,
                                            cfg.moveTilesRsrcId, cfg.blinkTargetTimerId)){
        std::cerr<<"_gameBoard.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_pieceHandler.init(static_cast<GameBoardProxy*>(&_gameBoard), 
                                        static_cast<GameProxy*>(this),
                                cfg.whitePiecesRsrcId, cfg.blackPiecesRsrcId, cfg.unfinishedPieceFontId)){
        std::cerr<<"_pieceHandler.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_piecePromotionPanel.init(cfg.piecePromotionPanelCfg,
                                                                static_cast<GameProxy*>(this))){
        std::cerr<<"_piecePromotionPanel.init() failed"<<std::endl;
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

    return EXIT_SUCCESS;
}

void Game::deinit(){
    // _gameFbo.destroy();
}

void Game::draw() const{
    _gameBoard.drawGameBoardOnly();
    _gameFbo.draw();
    _gameBoard.draw();
    if(_piecePromotionPanel.isActive()){
        _piecePromotionPanel.draw();
    }
}

void Game::handleEvent(InputEvent& e){
    _inputInverter.invertEvent(e);
    
    if(_piecePromotionPanel.isActive()){
        _piecePromotionPanel.handleEvent(e);
        return;
    }
    _pieceHandler.handleEvent(e);
}

void Game::onGameTurnFinished(){
    _gameBoardAnimator.startAnim(_gameLogic.getActivePlayerId());
}

void Game::onPawnPromotion(){
    _isPromotionActive=true; // a quick fix by Zhivko done in the last lecture 14
    _piecePromotionPanel.activate(_gameLogic.getActivePlayerId());
}

void Game::promotePiece(PieceType pieceType){    
    // NOTE: Remove Game::promotePiece() only for the Game 4/4 lecture !!!!

    // bonus for homework - finish the promotion !!! (listen again to the instructions of Zhivko)
    _pieceHandler.promotePiece(pieceType);
    _isPromotionActive=false;
    onBoardAnimFinished();
}

void Game::onBoardAnimFinished(){
    if(_isPromotionActive){
        return;
    }
    // BONUS: for homework implement text in the upper right corner...
    /// ...to display the currently active player (WHITE or BLACK)    
    _gameLogic.finishTurn();
    _pieceHandler.setCurrentPlayerId(_gameLogic.getActivePlayerId());
}

void Game::setWidgetFlip(WidgetFlip flipType){
    _pieceHandler.setWidgetFlip(flipType);
    _inputInverter.setBoardFlipType(flipType);
    regenerateGameFbo();
}

void Game::regenerateGameFbo() {
    // regenerating the sprite buffer (fbo is also called a sprite buffer)

    _gameFbo.unlock();
    _gameFbo.reset();

    // Zhivko: "Should the _gameBoard be part of the _gameFbo? Both yes and no, but for the moment it should not be."
    if(_gameBoardAnimator.isActive()){
        _gameBoard.drawGameBoardOnFbo(_gameFbo);
    }
    _pieceHandler.drawOnFbo(_gameFbo);
    
    _gameFbo.update();
    _gameFbo.lock();
}