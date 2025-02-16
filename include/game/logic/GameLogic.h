#ifndef INCLUDE_GAME_LOGIC_GAMELOGIC_H_
#define INCLUDE_GAME_LOGIC_GAMELOGIC_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

class GameLogic{
public:
    GameLogic();
    int32_t getActivePlayerId() const;
    void finishTurn();
private:
    int32_t _activePlayer;
};


#endif // INCLUDE_GAME_LOGIC_GAMELOGIC_H_