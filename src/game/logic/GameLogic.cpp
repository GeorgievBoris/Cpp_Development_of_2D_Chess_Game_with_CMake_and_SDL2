// Corresponding header
#include "game/logic/GameLogic.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"

GameLogic::GameLogic() : _activePlayer(Defines::WHITE_PLAYER_ID){

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