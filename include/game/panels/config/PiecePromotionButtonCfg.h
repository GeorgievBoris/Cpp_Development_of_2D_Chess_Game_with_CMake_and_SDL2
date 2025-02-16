#ifndef INCLUDE_GAME_PANELS_CONFIG_PIECEPROMOTIONBUTTONCFG_H_
#define INCLUDE_GAME_PANELS_CONFIG_PIECEPROMOTIONBUTTONCFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <functional>
// Third-party headers
// Own headers
#include "game/defines/ChessDefines.h"
#include "utils/drawing/Point.h"
// Forward Declarations

struct PiecePromotionButtonCfg{
    // IMPORTANT !!! write down...
    // std::function<> is the C++ variant of the C (language) callback

    // in std::function -> "void" means the return type of the function that is attached...
    // ... while if this function has any input parameters, we put them in the brackets (...)
    std::function<void(PieceType)> onBtnClicked; // capture the method by lambda
    Point bgrPos;
    int32_t buttonBgrRsrcId;
    int32_t buttonWhitePieceRsrcId;
    int32_t buttonBlackPieceRsrcId;
    int32_t bgrWidth=0;
    int32_t bgrHeight=0;
    int32_t width=0;
    int32_t height=0;
    PieceType pieceType=PieceType::UNKNOWN;
    
};


#endif // INCLUDE_GAME_PANELS_CONFIG_PIECEPROMOTIONBUTTONCFG_H_