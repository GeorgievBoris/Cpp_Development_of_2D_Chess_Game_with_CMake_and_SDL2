// Corresponding header
#include "game/pieces/types/ChessPiece.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "sdl_utils/InputEvent.h"

int32_t ChessPiece::init(const ChessPieceCfg& cfg){

    // NOTE: write this down ... in a real application must make a class or a sequence of classes...
    // ... for instance having the name "LOGGER", in which we can log and the user can ...
    // ... automatically set the option if "LOGGER" writes in a file, or with what is error (i.e. MINOR, MAJOR, DEBUG, INFO, WARNING, etc)...
    // ... and subsequently can filter our log depending on the level/depth of the error


    if(PieceType::UNKNOWN==cfg.pieceType){
        std::cerr<<"Error, invalid PieceType provided: "<<static_cast<int32_t>(cfg.pieceType)<<std::endl;
        return EXIT_FAILURE;
    }

    if(INVALID_RSRC_ID==cfg.rsrcId){
        std::cerr<<"Error, invalid rsrcId provided"<<std::endl;
        return EXIT_FAILURE;
    }
    _boardPos=cfg.boardPos;
    _playerId=cfg.playerId;
    _pieceType=cfg.pieceType;

    _pieceImg.create(cfg.rsrcId,BoardUtils::getAbsPos(_boardPos));
    _pieceImg.setFrame(static_cast<int32_t>(_pieceType));
    
    return EXIT_SUCCESS;
}

void ChessPiece::draw() const{
    _pieceImg.draw();
}

bool ChessPiece::containsEvent(const InputEvent& e) const{
    return _pieceImg.containsPoint(e.pos);
}

void ChessPiece::setBoardPos(const BoardPos& boardPos){
    _boardPos=boardPos;
    _pieceImg.setPosition(BoardUtils::getAbsPos(_boardPos));
}

BoardPos ChessPiece::getBoardPos() const{
    return _boardPos;
}

int32_t ChessPiece::getPlayerId() const{
    return _playerId;
}