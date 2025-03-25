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

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(_gameProxy, whitePiecesRsrcId,blackPiecesRsrcId, unfinishedPieceFontId, _pieces)){
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


    BoardPos boardPos=BoardUtils::getBoardPos(e.pos); // "const BoardPos& boardPos" originally added by Zhivko
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

            PieceHandler::alertGameBoardIfEnPassant(BoardUtils::getBoardPos(e.pos),moveTiles,piece->getPieceType()); // NOT added by Zhivko

            return;
        }
        ++relativePieceId;
    }
}

void PieceHandler::doMovePiece(BoardPos& boardPos){ // "const BoardPos& boardPos" originally added by Zhivko

    PieceHandler::unmarkPieces(); // NOT added by Zhivko

    int32_t collisionIdx=-1;

    const auto opponentId=BoardUtils::getOpponentId(_pieces[_currPlayerId][_selectedPieceId]->getPlayerId());

    BoardUtils::checkForEnPassant(_pieces[_currPlayerId][_selectedPieceId],_pieces[opponentId],boardPos,collisionIdx); // NOT added by Zhivko

    _pieces[_currPlayerId][_selectedPieceId]->setBoardPos(boardPos);

    
    if(-1!=collisionIdx || BoardUtils::doCollideWithPiece(boardPos,_pieces[opponentId],collisionIdx)){
        // Note: In this "if" statement - only the " -1!=collisionIdx || " part is NOT added by Zhivko !!!
        _pieces[opponentId].erase(_pieces[opponentId].begin()+collisionIdx); // remmeber this - very important !!!
    }
           
    _gameBoardProxy->onPieceUngrabbed();

    _gameProxy->onGameTurnFinished();
}

void PieceHandler::unmarkPieces(){ // PieceHandler::unmarkPieces() is NOT added by Zhivko
    for(std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        piece->setPieceAsLastMovedPiece(false);
    }
}

void PieceHandler::alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                    PieceType pieceType) const {  // PieceHandler::alertGameBoardForEnPassant() is NOT added by Zhivko

    if(PieceType::PAWN!=pieceType){
        return;
    }
    
    for(const TileData& tileData:moveTiles){
        if(TileType::TAKE!=tileData.tileType){
            continue;
        }

        constexpr Defines::Directions directions[2]={Defines::LEFT,Defines::RIGHT};
        for(const Defines::Directions dir:directions){
            const BoardPos enemyPawnPos=BoardUtils::getAdjacentPos(dir,boardPos);
            if(tileData.boardPos==enemyPawnPos){
                _gameBoardProxy->onEnPassant(enemyPawnPos);
                return;
            }
        }
    }
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
        std::cerr<<"Error, PieceHandlerPopulator::createPiece() failed. Pawn promotion failed!"<<std::endl;
        return;
    }
}


int32_t PieceHandler::restart(const std::function<void()>& gameRegenerateFboCallBack){ // PieceHandler::restart() is NOT added by Zhivko

    int32_t piecesWhitesRsrcId=INVALID_RSRC_ID;
    int32_t piecesBlackRsrcId=INVALID_RSRC_ID;
    for(ChessPiece::PlayerPieces& pieces:_pieces){
        std::unique_ptr<ChessPiece>& firstPiece=pieces.front();

        Defines::WHITE_PLAYER_ID==firstPiece->getPlayerId() ? piecesWhitesRsrcId=firstPiece->getRsrcId() : piecesBlackRsrcId=firstPiece->getRsrcId();
   
        pieces.clear(); // investigate further if the std::unique_ptr() will take care for deleting ChessPiece
    }

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(_gameProxy,piecesWhitesRsrcId,piecesBlackRsrcId,INVALID_RSRC_ID,_pieces)){

        std::cerr<<"PieceHandlerPopulator::populatePieceHandler() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    gameRegenerateFboCallBack();
    setCurrentPlayerId(Defines::WHITE_PLAYER_ID);
    
    return EXIT_SUCCESS;
}

void PieceHandler::setIsPieceGrabbed(){  // PieceHandler::setIsPieceGrabbed() is NOT added by Zhivko
    _isPieceGrabbed=false;
}