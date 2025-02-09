#ifndef INCLUDE_GAME_UTILS_BOARDPOS_H_
#define INCLUDE_GAME_UTILS_BOARDPOS_H_

// C system headers
#include <cstdint>
// C++ system headers
// Third-party headers
// Own headers
// Forward Declarations

struct BoardPos{
    BoardPos(int32_t inputRow, int32_t inputCol) : row(inputRow), col(inputCol) {}
    BoardPos()=default;
    int32_t row{};
    int32_t col{};
};


#endif // INCLUDE_GAME_UTILS_BOARDPOS_H_