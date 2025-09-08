#ifndef GAME_CONFIG_GAMECFG_H_
#define GAME_CONFIG_GAMECFG_H_

// C system headers
#include <cstdint>
// C++ system headers
#include <unordered_map>
#include <string>
// Third-party headers
// Own headers
#include "game/panels/config/PiecePromotionPanelCfg.h"
// Foward Declarations

struct GameCfg{
    PiecePromotionPanelCfg piecePromotionPanelCfg;

    int32_t chessBoardRsrcId;
    int32_t whitePiecesRsrcId;
    int32_t blackPiecesRsrcId;
    // int32_t targetRsrcId;

    int32_t moveTilesRsrcId;
    
    int32_t textFontId;

    int32_t quitGameButtonRsrcId; // NOT added by Zhivko
    int32_t winnerStarRsrcId; // NOT added by Zhivko
    int32_t fireworksRsrcId; // NOT added by Zhivko
    int32_t winnerMedalRsrcId; // NOT added by Zhivko
    int32_t whitePiecesHalvesRsrcId;// NOT added by Zhivko
    int32_t blackPiecesHalvesRsrcId;// NOT added by Zhivko

    int32_t blinkTargetTimerId;
    int32_t gameFboRotTimerId;
    int32_t playerTurnCapTimerId; // NOT added by Zhivko
    int32_t playerTurnCapTextTimerId; // NOT added by Zhivko
    int32_t blinkEnPassantTimerId; // NOT added by Zhivko
    int32_t blinkTileCastlingTimerId; // NOT added by Zhivko
    int32_t blinkTextCastlingTimerId; // NOT added by Zhivko
    int32_t nextWinnerAnimTimerId; // NOT added by Zhivko
    int32_t winnerAnimEndTimerId; // NOT added by Zhivko
    int32_t movePieceTimerId; // NOT added by Zhivko
    int32_t movePieceHalvesTimerId; // NOT added by Zhivko
    int32_t targetsRsrcId; // NOT added by Zhivko

    int32_t windowWidth; // NOT added by Zhivko
    int32_t windowHeight; // NOT added by Zhivko
};

#endif // GAME_CONFIG_GAMECFG_H_