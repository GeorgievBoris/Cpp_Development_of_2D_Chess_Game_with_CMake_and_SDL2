#ifndef INCLUDE_GAME_LOGSCREEN_LOGSCREEN_H_
#define INCLUDE_GAME_LOGSCREEN_LOGSCREEN_H_

#endif // INCLUDE_GAME_LOGSCREEN_LOGSCREEN_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <list>
#include <utility>
// Third-party headers
// Own headers
#include "manager_utils/drawing/Text.h"
#include "manager_utils/drawing/Image.h"
// Forward declarations
class PieceHandlerProxy;
class GameProxy;
class Fbo;
class InputEvent;

// Note: "LogScreen.h" is NOT added by Zhivko 

class LogScreen {
public:
    int32_t init(PieceHandlerProxy* pieceHandlerProxy, GameProxy* gameProxy, const int32_t fontId, const int32_t rsrcId);
    void deinit();
    void draw() const;
    void update(int32_t playerId);
    void drawOnFbo(Fbo& fbo);
    void handleEvent(const InputEvent& e);
private:
    Text _logScreenNote;
    std::pair<Text,Text> _logScreenTitle;
    Image _logScreenBckg;
    std::list<Text> _movedPieces;
    const size_t logSizeLimit=10;
    int32_t _fontId;
    PieceHandlerProxy* _pieceHandlerProxy;
    GameProxy* _gameProxy;
    Text _separator;
    static size_t _counter;
    static size_t _counterMissedWhitesTurns;
    static size_t _counterMissedBlacksTurns;
    bool _isLogScreenVisible=true;
};