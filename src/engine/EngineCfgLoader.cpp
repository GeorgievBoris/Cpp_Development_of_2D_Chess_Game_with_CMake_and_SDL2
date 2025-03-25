// Corresponding headers
#include "engine/EngineCfgLoader.h"
// C system headers
// C++ system headers
#include <iostream>
#include <string>
// Third-party headers
// Own headers
#include "common/CommonDefines.h"


extern const int32_t GAME_X_POS_SHIFT; // NOT added by Zhivko
extern const int32_t GAME_Y_POS_SHIFT; // NOT added by Zhivko

constexpr int32_t GAME_X_POS_SHIFT=0; // NOT added by Zhivko
constexpr int32_t GAME_Y_POS_SHIFT=100; // NOT added by Zhivko

// constants
static constexpr auto WINDOW_WIDTH=900;
static constexpr auto WINDOW_HEIGHT=1000;
static constexpr auto WINDOW_NAME="Hardware_Rendering";

static constexpr auto CHESS_PIECES_FRAMES=6;
static constexpr auto CHESS_PIECES_WIDTH_HEIGHT=96;

static constexpr auto CHESS_BOARD_WIDTH_HEIGHT=900;

static constexpr auto TARGET_IMG_WIDTH_HEIGHT=98;

static constexpr auto MOVE_TILES_FRAMES=3;
static constexpr auto MOVE_TILES_IMG_WIDTH_HEIGHT=98;

static constexpr auto PROMOTION_BTN_FRAMES=2;
static constexpr auto PROMOTION_BTN_WIDTH_HEIGHT=104;

namespace{ // namespace is NOT added by Zhivko
constexpr int32_t START_SCREEN_BUTTONS_FRAMES=2;
constexpr int32_t START_SCREEN_BUTTONS_WIDTH=318;
constexpr int32_t START_SCREEN_BUTTONS_HEIGHT=66;
constexpr int32_t START_SCREEN_BACKGROUND_HEIGHT=WINDOW_HEIGHT; // fix the .png!!!
constexpr int32_t START_SCREEN_BACKGROUND_WIDTH=WINDOW_WIDTH; // fix the .png!!!

constexpr int32_t QUIT_GAME_BUTTON_WIDTH=169;
constexpr int32_t QUIT_GAME_BUTTON_HEIGHT=66;
}

static constexpr auto ANGELINE_VINTAGE_40_FONT_SIZE=40;

static constexpr auto MAX_FRAMERATE=30;

static std::string getFilePath(const std::string& relativePath) {
    // NDEBUG == true -> means that we are in Release mode !!!
    #ifdef RELEASE_BUILD
        return relativePath;
    #else
        return std::string("../")+relativePath;
    #endif
}

static void populateMonitorCfg(MonitorWindowCfg& outCfg){
    outCfg.windowName=WINDOW_NAME;
    // outCfg.windowPos;
    outCfg.windowHeight=WINDOW_HEIGHT;
    outCfg.windowWidth=WINDOW_WIDTH;
    outCfg.windowFlags=WINDOW_SHOWN;
}

static void populateDrawMgrCfg(DrawMgrCfg& cfg){
    populateMonitorCfg(cfg.windowCfg);
    cfg.maxFrameRate=MAX_FRAMERATE;

}

static void populateImageContainerCfg(ImageContainerCfg& cfg){

    ImageCfg imageCfg;

    constexpr int32_t numOfChessPlayers=2;
    const std::string chessPiecesPaths[numOfChessPlayers]={"resources/p/whitePieces.png", "resources/p/blackPieces.png"};
    constexpr TextureId::ResourceId chessResources[numOfChessPlayers]={TextureId::WHITE_PIECES,TextureId::BLACK_PIECES};
    for(int32_t i=0;i<numOfChessPlayers;++i){
        imageCfg.location=getFilePath(chessPiecesPaths[i]);
        for(auto j=0;j<CHESS_PIECES_FRAMES;++j){
            imageCfg.frames.emplace_back(j*CHESS_PIECES_WIDTH_HEIGHT,0,CHESS_PIECES_WIDTH_HEIGHT,CHESS_PIECES_WIDTH_HEIGHT);
        }
        cfg.imageConfigs.emplace(chessResources[i],imageCfg);
        // cfg.imageConfigs.insert({TextureId::RUNNING_GIRL,imageCfg}); // possible - an alternative to std::make_pair
        // cfg.imageConfigs.insert(std::make_pair(TextureId::RUNNING_GIRL,imageCfg));

        imageCfg.frames.clear();
    }


    // the board does not have transparency, so it is ".jpg" -> no point in using ".png", therefore it saves some information
    imageCfg.location=getFilePath("resources/p/chessBoard.jpg");
    imageCfg.frames.emplace_back(0,0,CHESS_BOARD_WIDTH_HEIGHT,CHESS_BOARD_WIDTH_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::CHESS_BOARD,imageCfg);
    imageCfg.frames.clear(); 

    imageCfg.location=getFilePath("resources/p/target.png");
    imageCfg.frames.emplace_back(0,0,TARGET_IMG_WIDTH_HEIGHT,TARGET_IMG_WIDTH_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::TARGET,imageCfg);
    imageCfg.frames.clear();

    imageCfg.location=getFilePath("resources/p/moveTiles.png");
    for(int32_t frameId=0;frameId<MOVE_TILES_FRAMES;++frameId){
        imageCfg.frames.emplace_back(frameId*MOVE_TILES_IMG_WIDTH_HEIGHT,0,
                                    MOVE_TILES_IMG_WIDTH_HEIGHT,MOVE_TILES_IMG_WIDTH_HEIGHT);
    }
    cfg.imageConfigs.emplace(TextureId::MOVE_TILES,imageCfg);
    imageCfg.frames.clear();

    imageCfg.location=getFilePath("resources/p/piecePromoteButtonBgr.png");
    for(int32_t frameId=0;frameId<PROMOTION_BTN_FRAMES;++frameId){
        imageCfg.frames.emplace_back(frameId*PROMOTION_BTN_WIDTH_HEIGHT,0,
                                        PROMOTION_BTN_WIDTH_HEIGHT,PROMOTION_BTN_WIDTH_HEIGHT);
    }
    cfg.imageConfigs.emplace(TextureId::PROMOTION_BUTTON,imageCfg);
    imageCfg.frames.clear();

    imageCfg.location=getFilePath("resources/p/startScreenButtons.png");
    
    for(int32_t frameId=0;frameId<START_SCREEN_BUTTONS_FRAMES;++frameId){
        imageCfg.frames.emplace_back(
            frameId*START_SCREEN_BUTTONS_WIDTH,
            0,
            START_SCREEN_BUTTONS_WIDTH,
            START_SCREEN_BUTTONS_HEIGHT
        );
    }
    cfg.imageConfigs.emplace(TextureId::START_SCREEN_BUTTONS, imageCfg);
    imageCfg.frames.clear();

    imageCfg.location=getFilePath("resources/p/startScreenBackground.png");
    imageCfg.frames.emplace_back(0,0,START_SCREEN_BACKGROUND_WIDTH,START_SCREEN_BACKGROUND_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::START_SCREEN_BACKGROUND,imageCfg);
    imageCfg.frames.clear();


    imageCfg.location=getFilePath("resources/p/quitGameButton.png");
    imageCfg.frames.emplace_back(0,0,QUIT_GAME_BUTTON_WIDTH,QUIT_GAME_BUTTON_HEIGHT);
    cfg.imageConfigs.emplace(TextureId::QUIT_GAME_BUTTON, imageCfg);
    imageCfg.frames.clear();
}

static void populateTextContainerCfg(TextContainerCfg& cfg){
    FontCfg fontCfg;

    fontCfg.location=getFilePath("resources/f/AngelineVintage.ttf");
    fontCfg.fontSize=ANGELINE_VINTAGE_40_FONT_SIZE;

    cfg.fontConfigs.insert(std::make_pair(FontId::ANGELINE_VINTAGE_40,fontCfg));
}

static void populateRsrcMgrCfg(RsrcMgrCfg& cfg){
    populateImageContainerCfg(cfg.imgContainerCfg);
    populateTextContainerCfg(cfg.textContainerCfg);
}

static void populateManagerHandlerCfg(ManagerHandlerCfg& cfg){
    populateDrawMgrCfg(cfg.drawMgrCfg);
    populateRsrcMgrCfg(cfg.rsrcMgrCfg);
}

static void populateGameCfg(GameCfg& cfg){
    cfg.chessBoardRsrcId=TextureId::CHESS_BOARD;
    cfg.whitePiecesRsrcId=TextureId::WHITE_PIECES;
    cfg.blackPiecesRsrcId=TextureId::BLACK_PIECES;
    cfg.targetRsrcId=TextureId::TARGET;
    cfg.moveTilesRsrcId=TextureId::MOVE_TILES;
    cfg.quitGameButtonRsrcId=TextureId::QUIT_GAME_BUTTON; // NOT added by Zhivko

    cfg.blinkTargetTimerId=TimerId::BLINK_TARGET_TIMER_ID;
    cfg.gameFboRotTimerId=TimerId::GAME_FBO_ROT_TIMER_ID;
    cfg.playerTurnCapTimerId=TimerId::PLAYER_TURN_CAP_TIMER_ID; // NOT added by Zhivko
    cfg.playerTurnCapTextTimerId=TimerId::PLAYER_TURN_CAP_TEXT_TIMER_ID; // NOT added by Zhivko
    cfg.blinkEnPassantTimerId=TimerId::BLINK_EN_PASSANT_TIMER_ID; // NOT added by Zhivko
    
    cfg.unfinishedPieceFontId=FontId::ANGELINE_VINTAGE_40;

    auto& panelCfg=cfg.piecePromotionPanelCfg;
    panelCfg.blackPiecesRsrcId=TextureId::BLACK_PIECES;
    panelCfg.whitePiecesRsrcId=TextureId::WHITE_PIECES;
    panelCfg.buttonBgrRsrcId=TextureId::PROMOTION_BUTTON;
    panelCfg.gameBoardWidth=CHESS_BOARD_WIDTH_HEIGHT;
    panelCfg.gameBoardHeight=CHESS_BOARD_WIDTH_HEIGHT;
    panelCfg.buttonHeight=CHESS_PIECES_WIDTH_HEIGHT; // the normal size of 1 tile
    panelCfg.buttonWidth=CHESS_PIECES_WIDTH_HEIGHT; // the normal size of 1 tile
    panelCfg.buttonBgrHeight=PROMOTION_BTN_WIDTH_HEIGHT; // a bit bigger size that surrounds each chess figure/tile
    panelCfg.buttonBgrWidth=PROMOTION_BTN_WIDTH_HEIGHT; // a bit bigger size that surrounds each chess figure/tile
}

// the namespace is NOT added by Zhivko
namespace{
void populateStartScreenCfg(StartScreenCfg& cfg){

    cfg.btnHeight=START_SCREEN_BUTTONS_HEIGHT;
    cfg.btnWidth=START_SCREEN_BUTTONS_WIDTH;
    cfg.gameBoardWidth=CHESS_BOARD_WIDTH_HEIGHT;
    cfg.gameBoardHeight=CHESS_BOARD_WIDTH_HEIGHT;
    cfg.buttonsRsrcId=TextureId::START_SCREEN_BUTTONS;

    cfg.backgroundHeight=START_SCREEN_BACKGROUND_HEIGHT;
    cfg.backgroundWidth=START_SCREEN_BACKGROUND_WIDTH;
    cfg.backgroundRsrcId=TextureId::START_SCREEN_BACKGROUND;
    cfg.timerId=TimerId::START_SCREEN_TIMER_ID;
}

} // end of anonymous namespace


EngineCfg EngineCfgLoader::loadCfg(){
    EngineCfg cfg;
    populateManagerHandlerCfg(cfg.managerHandlerCfg);
    populateGameCfg(cfg.gameCfg);
    cfg.debugConsoleFontId=FontId::ANGELINE_VINTAGE_40;

    // below added by me
    cfg.startScreenButtonsRsrcId=TextureId::START_SCREEN_BUTTONS;
    populateStartScreenCfg(cfg.startScreenCfg);
    return cfg;
}