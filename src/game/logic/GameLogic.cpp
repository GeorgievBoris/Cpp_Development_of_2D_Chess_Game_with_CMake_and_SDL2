// Corresponding header
#include "game/logic/GameLogic.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "manager_utils/managers/TimerMgr.h" // NOT added by Zhivko
#include "manager_utils/managers/RsrcMgr.h"// NOT added by Zhivko
#include "game/proxies/PieceHandlerProxy.h" // NOT added by Zhivko

GameLogic::GameLogic() : _activePlayer(Defines::WHITE_PLAYER_ID){

}

int32_t GameLogic::init(PieceHandlerProxy* pieceHandlerProxy,int32_t playerTurnCapTimerId,int32_t playerTurnCapTextTimerId, 
                        int32_t blinkTextCastlingTimerId, int32_t fontId, int32_t quitGameButtonRsrcId){ // GameLogic::init() method is added by me

    if(nullptr==pieceHandlerProxy){
        std::cerr<<"Error, received nullptr for pieceHandlerProxy"<<std::endl;
        return EXIT_FAILURE;
    }
    _pieceHandlerProxy=pieceHandlerProxy;
    _playerTurnCapTimerId=playerTurnCapTimerId;
    _playerTurnCapTextTimerId=playerTurnCapTextTimerId;
    _blinkTextCastlingTimerId=blinkTextCastlingTimerId;

    _fontId=fontId;

    const Frames& frame=gRsrcMgr->getImageFrame(quitGameButtonRsrcId); // obtain the position, width, height of the Game Quit Button

    constexpr auto offset=100;
    const Point textPos=Point(offset+frame.front().x+frame.front().w,0);

    _playerTurnText.create("0",_fontId,Colors::WHITE,textPos);
    _playerTurnText.hide();

    _onCastleText.create("Castling possible",_fontId,Colors::WHITE,Point(textPos.x+offset*7,textPos.y));
    _onCastleText.hide();

    return EXIT_SUCCESS;
}

int32_t GameLogic::getActivePlayerId() const {
    return _activePlayer;
}

void GameLogic::finishTurn(){
    if(Defines::WHITE_PLAYER_ID==_activePlayer){
        _activePlayer=Defines::BLACK_PLAYER_ID;
        return;
    }
    _activePlayer=Defines::WHITE_PLAYER_ID;
}

void GameLogic::restart(){ // GameLogic::restart() method is NOT added by Zhivko
    _activePlayer=Defines::WHITE_PLAYER_ID;
    _playerTurnText.setText("0");
    GameLogic::stopPlayersTimer();
}

void GameLogic::startPlayersTimer(){ // GameLogic::startPlayerTimer() is NOT added by Zhivko
    _playerTurnText.show();

    if(TimerClient::isActiveTimerId(_playerTurnCapTimerId)){
        return;
    }
    TimerClient::startTimer(60000,_playerTurnCapTimerId,TimerType::ONESHOT);

    if(TimerClient::isActiveTimerId(_playerTurnCapTextTimerId)){
        return;
    }
    TimerClient::startTimer(500,_playerTurnCapTextTimerId,TimerType::PULSE);

    GameLogic::setInternals();
}

void GameLogic::stopPlayersTimer(){ // GameLogic::stopPlayerTimer() is NOT added by Zhivko
    _playerTurnText.hide();
    if(TimerClient::isActiveTimerId(_playerTurnCapTextTimerId)){
        TimerClient::stopTimer(_playerTurnCapTextTimerId);
    }

    if(TimerClient::isActiveTimerId(_playerTurnCapTimerId)){
        TimerClient::stopTimer(_playerTurnCapTimerId);
    }

    if(TimerClient::isActiveTimerId(_blinkTextCastlingTimerId)){
        TimerClient::stopTimer(_blinkTextCastlingTimerId);
    }
}

void GameLogic::startOnCastleTimer(){ // GameLogic::startOnCastleTimer() is NOT added by Zhivko
    
    if(TimerClient::isActiveTimerId(_blinkTextCastlingTimerId)){
        return;
    }
    _onCastleText.show();
    startTimer(500,_blinkTextCastlingTimerId,TimerType::PULSE);
}

void GameLogic::stopOnCastleTimer(){ // GameLogic::stopOnCastleTimer() is NOT added by Zhivko
    
    if(TimerClient::isActiveTimerId(_blinkTextCastlingTimerId)){
        _onCastleText.hide();
        stopTimer(_blinkTextCastlingTimerId);
    }
}

void GameLogic::draw() const{ // GameLogic::draw() method added by me
    _playerTurnText.draw();
    _onCastleText.draw();
}

bool GameLogic::isTimerActive() const{ // GameLogic::isTimerOn() method added by me
    return TimerClient::isActiveTimerId(_playerTurnCapTimerId);
}

void GameLogic::onTimeout(int32_t timerId){ // GameLogic::onTimeout() method is added by me

    if(timerId==_playerTurnCapTimerId){
        _pieceHandlerProxy->onTurnTimeElapsed();
        GameLogic::stopPlayersTimer();
        return;
    }    

    if(timerId==_playerTurnCapTextTimerId){

        GameLogic::setInternals();
        if(30<_playerRemainingTime){
            return;
        }
        _playerTurnText.isVisible() ? _playerTurnText.hide() : _playerTurnText.show();
        return;
    }

    if(timerId==_blinkTextCastlingTimerId){
        _onCastleText.isVisible() ? _onCastleText.hide() : _onCastleText.show();
        return;
    }
    std::cerr<<"Error, invalid timerId is provieded for GameLogic"<<std::endl;
}

int32_t GameLogic::setInternals() const{

    gTimerMgr->calculateTimerRemainingTime(_playerTurnCapTimerId,_playerRemainingTime);
    if(0>_playerRemainingTime){
        std::cerr<<"Error, TimerMgr::getTimerRemainingTime() failed!"<<std::endl;
        Defines::WHITE_PLAYER_ID==_activePlayer ? std::cerr<<"Player with white pieces " :  std::cerr<<"Player with black pieces ";
        std::cerr<<"has negative remaining turn time!"<<std::endl;
        return EXIT_FAILURE;
    }

    constexpr auto millisecondsPerSecond=1000;

    _playerRemainingTime/=millisecondsPerSecond; // convert from milliseconds to seconds

    std::string text="Move ";
    Defines::WHITE_PLAYER_ID==_activePlayer ? text.append("White") : text.append("Black");
    text.append(" Pieces     Time Left: ")
        .append(std::to_string(_playerRemainingTime))
        .append("sec");

    _playerTurnText.setText(text);

    return EXIT_SUCCESS;

}