// Corresponding header
#include "game/pieces/PieceHandler.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "sdl_utils/InputEvent.h"
#include "manager_utils/drawing/Fbo.h"
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameBoardProxy.h"
#include "game/proxies/GameProxy.h"
#include "game/pieces/PieceHandlerPopulator.h"

int32_t PieceHandler::init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy, 
                                int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t unfinishedPieceFontId){

    if(nullptr==gameBoardProxy){
        std::cerr<<"Error, nullptr provided for gameBoardProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for gameProxy"<<std::endl;
        return EXIT_FAILURE;
    }

    _gameBoardProxy=gameBoardProxy;
    _gameProxy=gameProxy;

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(_gameProxy, whitePiecesRsrcId,blackPiecesRsrcId, 
                                                                unfinishedPieceFontId, _pieces)){
        std::cerr<<"PieceHandlerPopulator::populatePieceHandler() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void PieceHandler::drawOnFbo(Fbo& fbo) const {

    for(const ChessPiece::PlayerPieces& playerPieces:_pieces){
        for(const std::unique_ptr<ChessPiece>& piece:playerPieces){
            piece->drawOnFbo(fbo);
        }
    }
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

void PieceHandler::setCurrentPlayerId(int32_t currPlayerId){
    _currPlayerId=currPlayerId;
}

void PieceHandler::setWidgetFlip(WidgetFlip flipType){
    for(ChessPiece::PlayerPieces& playerPieces : _pieces){
        for(std::unique_ptr<ChessPiece>& piece : playerPieces){
            piece->setWidgetFlip(flipType);
        }
    }
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

    int32_t relativePieceId=0;
    for(const std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        if(piece->containsEvent(e)){
            _selectedPieceId=relativePieceId;
            _isPieceGrabbed=true;
            const std::vector<TileData> moveTiles = 
                                    _pieces[_currPlayerId][_selectedPieceId]->getMoveTiles(_pieces);
            _gameBoardProxy->onPieceGrabbed(BoardUtils::getBoardPos(e.pos),moveTiles);
            return;
        }
        ++relativePieceId;
    }
}

void PieceHandler::doMovePiece(const BoardPos& boardPos){
    _pieces[_currPlayerId][_selectedPieceId]->setBoardPos(boardPos);

    const auto opponentId=BoardUtils::getOpponentId(_pieces[_currPlayerId][_selectedPieceId]->getPlayerId());
    int32_t collisionIdx=-1;

    if(BoardUtils::doCollideWithPiece(boardPos,_pieces[opponentId],collisionIdx)){
        _pieces[opponentId].erase(_pieces[opponentId].begin()+collisionIdx); // remmeber this - very important !!!
    }
    _gameBoardProxy->onPieceUngrabbed();

    _gameProxy->onGameTurnFinished();
}

void PieceHandler::promotePiece(PieceType pieceType){
    const auto boardPos=_pieces[_currPlayerId][_selectedPieceId]->getBoardPos();
    const auto piecePlayerId=_pieces[_currPlayerId][_selectedPieceId]->getPlayerId();
    const auto pieceRsrcId=_pieces[_currPlayerId][_selectedPieceId]->getRsrcId();

    _pieces[_currPlayerId][_selectedPieceId].reset();

    ChessPieceCfg pieceCfg;
    pieceCfg.boardPos=boardPos;
    pieceCfg.playerId=piecePlayerId;
    pieceCfg.pieceType=pieceType;
    pieceCfg.rsrcId=pieceRsrcId;
    // in general unfinishedPieceFont is a temporary "hack" that must be removed from everywhere, onece all chess pieces are implemented
    pieceCfg.unfinishedPieceFontId=0; 

    _pieces[_currPlayerId][_selectedPieceId]=PieceHandlerPopulator::createPiece(pieceType,_gameProxy);
    if(EXIT_SUCCESS!=_pieces[_currPlayerId][_selectedPieceId]->init(pieceCfg)){
        std::cerr<<""<<std::endl;
        return;
    }
}