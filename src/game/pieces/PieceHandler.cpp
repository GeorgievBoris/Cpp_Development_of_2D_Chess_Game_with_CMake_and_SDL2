// Corresponding header
#include "game/pieces/PieceHandler.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameBoardProxy.h"
#include "game/pieces/PieceHandlerPopulator.h"

int32_t PieceHandler::init(GameBoardProxy* gameBoardProxy, int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId,
                                                                        int32_t unfinishedPieceFontId){

    if(nullptr==gameBoardProxy){
        std::cerr<<"Error, nullptr provided for gameBoardProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameBoardProxy=gameBoardProxy;

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(whitePiecesRsrcId,blackPiecesRsrcId, 
                                                                unfinishedPieceFontId, _pieces)){
        std::cerr<<"PieceHandlerPopulator::populatePieceHandler() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void PieceHandler::draw() const{
    for(const ChessPiece::PlayerPieces& playerPieces:_pieces){
        for(const std::unique_ptr<ChessPiece>& piece:playerPieces){
            piece->draw();
        }
    }
}

void PieceHandler::handleEvent(const InputEvent& e){
    // NOTE: write this in the book .... this is called тернарен operator 
    _isPieceGrabbed ? handlePieceGrabbedEvent(e) : handlePieceUngrabbedEvent(e);
}

void PieceHandler::handlePieceGrabbedEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    _isPieceGrabbed=false;

    if(!BoardUtils::isInsideBoard(e.pos)){
        _gameBoardProxy->onPieceUngrabbed();
        return;
    }


    const BoardPos boardPos=BoardUtils::getBoardPos(e.pos);
    if(!_gameBoardProxy->isMoveAllowed(boardPos)){
        _gameBoardProxy->onPieceUngrabbed();
        return;
    }

    doMovePiece(boardPos);
}

void PieceHandler::handlePieceUngrabbedEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }

    int32_t currPlayerId=Defines::WHITE_PLAYER_ID;
    for(const ChessPiece::PlayerPieces& playerPieces:_pieces){
        int32_t relativePieceId=0;
        for(const std::unique_ptr<ChessPiece>& piece:playerPieces){
            if(piece->containsEvent(e)){
                _selectedPieceId=relativePieceId;
                _selectedPiecePlayerId=currPlayerId;
                _isPieceGrabbed=true;
                const std::vector<TileData> moveTiles = 
                                        _pieces[_selectedPiecePlayerId][_selectedPieceId]->getMoveTiles(_pieces);
                _gameBoardProxy->onPieceGrabbed(BoardUtils::getBoardPos(e.pos),moveTiles);
                return;
            }
            ++relativePieceId;
        }
        ++currPlayerId;
    }
}

void PieceHandler::doMovePiece(const BoardPos& boardPos){
    _pieces[_selectedPiecePlayerId][_selectedPieceId]->setBoardPos(boardPos);

    const auto opponentId=BoardUtils::getOpponentId(_pieces[_selectedPiecePlayerId][_selectedPieceId]->getPlayerId());
    int32_t collisionIdx=-1;

    if(BoardUtils::doCollideWithPiece(boardPos,_pieces[opponentId],collisionIdx)){
        _pieces[opponentId].erase(_pieces[opponentId].begin()+collisionIdx); // remmeber this - very important !!!
    }
    _gameBoardProxy->onPieceUngrabbed();
}