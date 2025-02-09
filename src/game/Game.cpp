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
    if(EXIT_SUCCESS!=_gameBoard.init(cfg.chessBoardRsrcId, cfg.targetRsrcId, cfg.blinkTargetTimerId)){
        std::cerr<<"_gameBoard.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    if(EXIT_SUCCESS!=_pieceHandler.init(static_cast<GameBoardProxy*>(&_gameBoard),cfg.whitePiecesRsrcId, 
                                                    cfg.blackPiecesRsrcId)){
        std::cerr<<"_pieceHandler.init() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void Game::deinit(){

}

void Game::draw() const{
    _gameBoard.draw();
    _pieceHandler.draw();
}

void Game::handleEvent(const InputEvent& e){
    _pieceHandler.handleEvent(e);
}