#ifndef INCLUDE_GAME_LOGIC_GAMELOGIC_H_
#define INCLUDE_GAME_LOGIC_GAMELOGIC_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "manager_utils/time/TimerClient.h" // NOT added by Zhivko
#include "manager_utils/drawing/Text.h" // NOT added by Zhivko
// Forward Declarations
class PieceHandlerProxy;

class GameLogic : public TimerClient { // inheritance is added by me
public:
    GameLogic();
    int32_t init(PieceHandlerProxy* pieceHandlerProxy,int32_t playerTurnCapTimerId, int32_t playerTurnCapTextTimerId, 
                    int32_t blinkTextCastlingTimerId, int32_t fontId, int32_t quitGameButtonRsrcId); // GameLogic::init() method is added by me
    int32_t getActivePlayerId() const;
    void finishTurn();
    void restart(); // GameLogic::restart() method is NOT added by Zhivko
    void startPlayersTimer(); // GameLogic::startPlayerTimer() is NOT added by Zhivko
    void stopPlayersTimer(); // GameLogic::stopPlayerTimer() is NOT added by Zhivko
    void startOnCastleTimer(); // GameLogic::startOnCastleTimer() is NOT added by Zhivko
    void stopOnCastleTimer(); // GameLogic::stopOnCastleTimer() is NOT added by Zhivko
    void draw() const; // GameLogic::draw() method is NOT added by Zhivko
    bool isTimerActive() const; // GameLogic::isTimerOn() method is NOT added by Zhivko
private:
    void onTimeout(int32_t timerId) final; // GameLogic::onTimeout() method is NOT added by Zhivko
    int32_t setInternals() const; // GameLogic::setInternals() method is NOT added by Zhivko

    int32_t _activePlayer;
    int32_t _playerTurnCapTimerId; // variable / object is added by me
    int32_t _playerTurnCapTextTimerId; // variable / object is added by me
    int32_t _blinkTextCastlingTimerId; // NOT added by Zhivko
    int32_t _fontId;  // variable / object is added by me
    mutable int64_t _playerRemainingTime; // variable / object is added by me
    mutable Text _playerTurnText; // variable / object is added by me
    Text _onCastleText; // NOT added by Zhivko
    PieceHandlerProxy* _pieceHandlerProxy; // NOT added by Zhivko
};


#endif // INCLUDE_GAME_LOGIC_GAMELOGIC_H_