// Corresponding header
#include "game/pieces/types/UnfinishedPiece.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"


int32_t UnfinishedPiece::init(const ChessPieceCfg& cfg){

    if(EXIT_SUCCESS!=ChessPiece::init(cfg)){
        std::cerr<<"ChessPiece::init() failed"<<std::endl;
        return EXIT_FAILURE;
    }
    const std::string text="!";
    const Point pieceAbsPos=BoardUtils::getAbsPos(_boardPos);
    _notReadyText.create(text,cfg.unfinishedPieceFontId,Colors::RED,pieceAbsPos);

    return EXIT_SUCCESS;
}

void UnfinishedPiece::draw() const {
    ChessPiece::draw();
    _notReadyText.draw();
}

std::vector<TileData> UnfinishedPiece::getMoveTiles([[maybe_unused]] const std::array<ChessPiece::PlayerPieces,
                                                            Defines::PLAYERS_COUNT>& activePieces) const{

    return std::vector<TileData>{};

}

void UnfinishedPiece::setBoardPos(const BoardPos& boardPos){
    ChessPiece::setBoardPos(boardPos);
    const Point textPieceAbsPos=BoardUtils::getAbsPos(_boardPos);
    _notReadyText.setPosition(textPieceAbsPos);
}