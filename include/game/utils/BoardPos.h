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
    bool operator==(const BoardPos& other) const{
        return row==other.row && col==other.col;
    }
    bool operator!=(const BoardPos& other) const{
        return !BoardPos::operator==(other);
    }
    int32_t row{};
    int32_t col{};
};


#endif // INCLUDE_GAME_UTILS_BOARDPOS_H_