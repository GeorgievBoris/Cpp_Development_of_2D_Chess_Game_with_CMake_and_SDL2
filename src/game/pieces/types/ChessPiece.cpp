// Corresponding header
#include "game/pieces/types/ChessPiece.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "sdl_utils/InputEvent.h"
#include "manager_utils/drawing/Fbo.h"

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
    
    const Point rotCenter=Point(_pieceImg.getWidth()/2,_pieceImg.getHeight()/2); // NOT added by Zhivko
    _pieceImg.setRotationCenter(rotCenter); // NOT added by Zhivko

    return EXIT_SUCCESS;
}

void ChessPiece::drawOnFbo(Fbo& fbo) const{
    fbo.addWidget(_pieceImg);
}

void ChessPiece::draw() const{
    _pieceImg.draw();
}

bool ChessPiece::containsEvent(const InputEvent& e) const{
    return _pieceImg.containsPoint(e.pos);
}

void ChessPiece::setBoardPos(const BoardPos& boardPos){
    _boardPos=boardPos;
    // _pieceImg.setPosition(BoardUtils::getAbsPos(_boardPos)); // added by Zhivko
    const Point absPosTemporary=BoardUtils::getAbsPos(_boardPos);
    const Point absPos(absPosTemporary.x+_boardPos.remCol,absPosTemporary.y+_boardPos.remRow);
    _pieceImg.setPosition(absPos); // NOT added by Zhivko
}

void ChessPiece::setIsTaken(bool isTaken){
    if(PieceType::KING==_pieceType){
        return;
    }
    _isTaken=isTaken;
}

bool ChessPiece::getIsTaken() const {
    return _isTaken;
} 

BoardPos ChessPiece::getBoardPos() const{
    return _boardPos;
}

int32_t ChessPiece::getPlayerId() const{
    return _playerId;
}

int32_t ChessPiece::getRsrcId() const{
    return _pieceImg.getRsrcId();
}

void ChessPiece::setWidgetFlip(WidgetFlip flipType){
    _pieceImg.setFlipType(flipType);
}

PieceType ChessPiece::getPieceType() const{ // ChessPiece::getPieceType() is NOT added by Zhivko
    return _pieceType;
}

void ChessPiece::setRotationAngle(double angle) { // ChessPiece::setRotationAngle() is NOT added by Zhivko
    _pieceImg.setRotationAngle(angle);
}

double ChessPiece::getRotationAngle() const{ // ChessPiece::getRotationAngle() is NOT added by Zhivko
    return _pieceImg.getRotationAngle();
}

void ChessPiece::activateAlphaModulation(){
    _pieceImg.activateAlphaModulation();
}

void ChessPiece::setOpacity(int32_t opacity){
    if(0>opacity){
        return;
    }
    _pieceImg.setOpacity(opacity);
}

int32_t ChessPiece::getOpacity() const{
    return _pieceImg.getOpacity();
}

void ChessPiece::deactivateAlphaModulation(){
    _pieceImg.deactivateAlphaModulation();
}

bool ChessPiece::isGetMoveTilesCalled(const std::array<PlayerPieces, Defines::PLAYERS_COUNT>& activePlayers) const{
    for(const PlayerPieces& player:activePlayers){
        for(const std::unique_ptr<ChessPiece>& piece:player){
            if(piece->_isFncGetMoveTilesCalled){
                return true;
            }
        }
    }
    return false;
}

bool ChessPiece::isMoveTileValid(const BoardPos& boardPos, const BoardPos& kingBoardPos, const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const {
    int32_t opponentId=BoardUtils::getOpponentId(_playerId);
    const ChessPiece::PlayerPieces& opponentPieces=activePlayers[opponentId];
    const BoardPos initialBoardPos=_boardPos;
    _boardPos=boardPos;

    if(PieceType::KING!=_pieceType){
        for(const std::unique_ptr<ChessPiece>& piece:opponentPieces){
            const std::vector<TileData> moveTiles=piece->getMoveTiles(activePlayers);
            for(const TileData& tileData:moveTiles){
                if(TileType::TAKE!=tileData.tileType){
                    continue;
                }
                if(kingBoardPos!=tileData.boardPos){
                    continue;
                }
                _boardPos=initialBoardPos;
                return false;            
            }        
        }
        _boardPos=initialBoardPos;
        return true;        
    }

    for(const std::unique_ptr<ChessPiece>& piece:opponentPieces){
        const std::vector<TileData> moveTiles=piece->getMoveTiles(activePlayers);
        for(const TileData& tileData:moveTiles){
            if(TileType::TAKE!=tileData.tileType){
                continue;
            }
            if(boardPos!=tileData.boardPos){
                continue;
            }
            _boardPos=initialBoardPos;
            return false;
        }
    }
    _boardPos=initialBoardPos;
    return true;
}

bool ChessPiece::isTakeTileValid(const BoardPos& boardPos, const BoardPos& kingBoardPos, const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const {
    int32_t opponentId=BoardUtils::getOpponentId(_playerId);
    const ChessPiece::PlayerPieces& opponentPieces=activePlayers[opponentId];
    
    if(PieceType::KING!=_pieceType){
        const BoardPos initialBoardPos=_boardPos;
        _boardPos=boardPos;
        for(const std::unique_ptr<ChessPiece>& piece:opponentPieces){
            if(boardPos==piece->getBoardPos()){
                continue;
            }
            
            const std::vector<TileData> moveTiles=piece->getMoveTiles(activePlayers);
            for(const TileData& tileData:moveTiles){
                if(TileType::TAKE!=tileData.tileType){
                    continue;
                }
                if(kingBoardPos!=tileData.boardPos){
                    continue;
                }
                _boardPos=initialBoardPos;
                return false;            
            }        
        }
        _boardPos=initialBoardPos;
        return true;
    }


    for(const std::unique_ptr<ChessPiece>& piece:opponentPieces){
        if(boardPos==piece->getBoardPos()){
            continue;
        }

        const std::vector<TileData> moveTiles=piece->getMoveTiles(activePlayers);
        for(const TileData& tileData:moveTiles){
            if(TileType::GUARD!=tileData.tileType){
                continue;
            }
            if(boardPos!=tileData.boardPos){
                continue;
            }
            return false;
        }
    }
    return true;
}