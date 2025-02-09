#ifndef INCLUDE_GAME_UTILS_BOARDUTILS_H_
#define INCLUDE_GAME_UTILS_BOARDUTILS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardPos.h"
#include "utils/drawing/Point.h"
// Forward Declarations

class BoardUtils{
public:
    BoardUtils()=delete; // probably Zhivko forgot to add it, but it must be here for a class that contains only static members
    ~BoardUtils()=default; // probably Zhivko forgot to add it, but it must be here for a class that contains only static members

    // map monitor/screen coordinates to coordinates on the chess board AND vice versa
    static BoardPos getBoardPos(const Point& absPos);
    static Point getAbsPos(const BoardPos& boardPos);
    static bool isInsideBoard(const BoardPos& boardPos);
    static bool isInsideBoard(const Point& absPos);
};

#endif // INCLUDE_GAME_UTILS_BOARDUTILS_H_