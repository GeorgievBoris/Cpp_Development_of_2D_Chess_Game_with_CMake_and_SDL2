#ifndef INCLUDE_GAME_PROXIES_GAMEPROXY_H_
#define INCLUDE_GAME_PROXIES_GAMEPROXY_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

class GameProxy{
public:
    virtual ~GameProxy()=default;
    virtual void onButtonPressed(int32_t buttonId)=0;
};


#endif // INCLUDE_GAME_PROXIES_GAMEPROXY_H_