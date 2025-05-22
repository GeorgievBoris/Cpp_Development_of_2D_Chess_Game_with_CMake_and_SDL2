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

#include "game/pieces/types/Rook.h" // NOT added by Zhivko
#include "game/pieces/types/King.h" // NOT added by Zhivko
#include "game/pieces/types/Pawn.h" // NOT added by Zhivko

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

    if(_isCastlingPossible){ // NOT added by Zhivko
        _gameProxy->castleTextHide(); // NOT added by Zhivko
    }

    if(!BoardUtils::isInsideBoard(e.pos)){
        _gameBoardProxy->onPieceUngrabbed();
        _isCastlingPossible=false;
        return;
    }

    BoardPos boardPos=BoardUtils::getBoardPos(e.pos); // "const BoardPos boardPos" originally used by Zhivko
    if(!_gameBoardProxy->isMoveAllowed(boardPos)){
        _gameBoardProxy->onPieceUngrabbed();
        _isCastlingPossible=false;
        return;
    }

    std::unique_ptr<ChessPiece>& currPlayerSelectedPiece=_pieces[_currPlayerId][_selectedPieceId]; // NOT added by Zhivko
    
    std::pair<bool,std::pair<int32_t,BoardPos>> pair {false,{INVALID_RSRC_ID,BoardPos{INVALID_RSRC_ID,INVALID_RSRC_ID}}}; // NOT added by Zhivko
    
    if(_isCastlingPossible){
        BoardUtils::checkForCastling(_pieces[_currPlayerId],currPlayerSelectedPiece,boardPos,pair); // NOT added by Zhivko
        _isCastlingPossible=false;
    }
    
    int32_t collisionIdx=INVALID_RSRC_ID; // NOT added by Zhivko

    if(!PieceHandler::isMoveValid(boardPos,pair.first,collisionIdx)){ // NOT added by Zhivko
        _gameBoardProxy->onPieceUngrabbed(); // NOT added by Zhivko
        _isCastlingPossible=false;
        return;
    }

    doMovePiece(boardPos,collisionIdx,currPlayerSelectedPiece,pair); // "collisionIdx", "currPlayerSelectedPiece" and "pair" are NOT added by Zhivko
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
            const std::vector<TileData> moveTiles = _pieces[_currPlayerId][_selectedPieceId]->getMoveTiles(_pieces);
            _gameBoardProxy->onPieceGrabbed(BoardUtils::getBoardPos(e.pos),moveTiles);

            PieceHandler::alertGameBoardIfEnPassant(BoardUtils::getBoardPos(e.pos),moveTiles,piece->getPieceType()); // NOT added by Zhivko
            _isCastlingPossible=PieceHandler::alertGameBoardIfCastling(piece); // NOT added by Zhivko
            _gameBoardProxy->shiftMoveTilesPos(BoardUtils::getBoardPos(e.pos)); // NOT added by Zhivko 
            return;
        }
        ++relativePieceId;
    }
}

void PieceHandler::doMovePiece(const BoardPos& boardPos, int32_t& collisionIdx,
                                std::unique_ptr<ChessPiece>& selectedPiece, 
                                const std::pair<bool,std::pair<int32_t, BoardPos>>& pair){ // "collisionIdx", "selectedPiece" and "pair" are NOT added by Zhivko

    // PieceHandler::unmarkPieces(); // NOT added by Zhivko

    if(pair.first){ // NOT added by Zhivko
        // if Castling is performed, the KING or the ROOK positions get changed here
        _pieces[_currPlayerId][pair.second.first]->setBoardPos(pair.second.second); // NOT added by Zhivko
    } // NOT added by Zhivko

    if(INVALID_RSRC_ID!=collisionIdx){ // NOT added by Zhivko
        const auto opponentId=BoardUtils::getOpponentId(selectedPiece->getPlayerId());
        _pieces[opponentId].erase(_pieces[opponentId].begin()+collisionIdx); // NOT added by Zhivko
    }

    /* the commented code here, is originally added by Zhivko

    int32_t collisionIdx=-1;
    if(BoardUtils::doCollideWithPiece(boardPos,_pieces[opponentId],collisionIdx)){
        _pieces[opponentId].erase(_pieces[opponentId].begin()+collisionIdx); // remmeber this - very important !!!
    }
    */

    PieceHandler::checkPawnsStateForEnPassant(boardPos,selectedPiece);
    selectedPiece->setBoardPos(boardPos);
   
    if(!_isPawnPromoted){
        if(!PieceHandler::isOpponentKingInCheck()){
            PieceHandler::isOpponentInStalemate();
        }
    }

    _gameBoardProxy->onPieceUngrabbed();
    _gameProxy->onGameTurnFinished();
}

void PieceHandler::checkPawnsStateForEnPassant(const BoardPos& newBoardPos, const std::unique_ptr<ChessPiece>& selectedPiece){ // PieceHandler::checkPawnsForEnPassant() is NOT added by Zhivko

    const std::unique_ptr<ChessPiece>* chessPieceUnqPtr=&selectedPiece;

    const PieceType selectedPieceType=selectedPiece->getPieceType();

    if(PieceType::PAWN!=selectedPieceType){
        const size_t size=_pieces[_currPlayerId].size();
        for(size_t indx=0; indx<size;++indx){
            if(PieceType::PAWN==_pieces[_currPlayerId][indx]->getPieceType()){
                chessPieceUnqPtr=&_pieces[_currPlayerId][indx];
                break;
            }
        }
    }

    ChessPiece* const chessPiecePtr=chessPieceUnqPtr->get();
    Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
    pawnPtr->checkStateForEnPassant(newBoardPos,_pieces[_currPlayerId],selectedPieceType);
}

bool PieceHandler::isMoveValid(BoardPos& boardPos, bool isCastlingDone, int32_t& collisionIdx){ // PieceHandler::isMoveValid() is NOT added by Zhivko

    if(isCastlingDone){
        return true; 
    }

    const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);

    BoardUtils::doCollideWithPiece(boardPos,_pieces[opponentId],collisionIdx); 
    BoardUtils::checkForEnPassant(_pieces[_currPlayerId][_selectedPieceId],_pieces[opponentId],boardPos,collisionIdx);

    if(PieceHandler::isNextMoveCheckForKing(_currPlayerId,_selectedPieceId,collisionIdx,boardPos)){
        return false;
    }

    if(_isKingInCheck){ 
        _isKingInCheck=false;
    }
        
    return true;
}

void PieceHandler::alertGameBoardIfEnPassant(const BoardPos& boardPos, const std::vector<TileData>& moveTiles,
                                                    PieceType pieceType) const {  // PieceHandler::alertGameBoardIfEnPassant() is NOT added by Zhivko

    if(PieceType::PAWN!=pieceType){
        return;
    }
    
    for(const TileData& tileData:moveTiles){
        if(TileType::TAKE!=tileData.tileType){
            continue;
        }

        if(tileData.boardPos.row!=boardPos.row){
            continue;
        }

        constexpr Defines::Directions directions[2]={Defines::LEFT,Defines::RIGHT};
        for(const Defines::Directions& dir:directions){
            BoardPos pawnPos=BoardUtils::getAdjacentPos(dir,boardPos); // must not be a constant
            if(tileData.boardPos!=pawnPos){
                continue;
            }
            _gameBoardProxy->onEnPassant(pawnPos); // for the TAKE position when EnPassant is performed
            
            Defines::WHITE_PLAYER_ID==_currPlayerId ?
            pawnPos=BoardUtils::getAdjacentPos(Defines::UP,pawnPos):
            pawnPos=BoardUtils::getAdjacentPos(Defines::DOWN,pawnPos);

            _gameBoardProxy->onEnPassant(pawnPos); // for the MOVE position when EnPassant is performed
            return;
        }
    }
}

bool PieceHandler::alertGameBoardIfCastling(const std::unique_ptr<ChessPiece>& piece) const{ // PieceHandler::alertGameBoardIfCastlling() is NOT added by Zhivko
    
    PieceType pieceType=piece->getPieceType();

    if(PieceType::KING!=pieceType && PieceType::ROOK!=pieceType){
        return false;
    }

    // later try to reduce this code by making a method "virtual bool getIsCastlePossible() const" that is NOT pure virtual in the ChessPiece class ???
    const ChessPiece* const chessPiecePtr=piece.get();

    if(PieceType::ROOK==pieceType){
        
        const Rook* const rookPiecePtr=static_cast<const Rook*>(chessPiecePtr);
        
        if(!rookPiecePtr->getIsCastlePossible()){
            return false;
        }
        const int32_t kingIdx=PieceHandler::getKingIndex(_currPlayerId);
        const BoardPos kingBoardPos=_pieces[_currPlayerId][kingIdx]->getBoardPos();
        _gameBoardProxy->onCastling(kingBoardPos);
        _gameProxy->castleTextShow();
        return true;
    }

    const King* const kingPiecePtr=static_cast<const King*>(chessPiecePtr);

    if(kingPiecePtr->getIsCastlePossible()){
        _gameProxy->castleTextShow();
        return true;
    }
    return false;
}


bool PieceHandler::isOpponentKingInCheck() { // PieceHandler::isOpponentKingInCheck() is NOT added by Zhivko
    const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
    const int32_t opponentPlayerKingIndex=getKingIndex(opponentId);
    const BoardPos opponentPlayerKingPosition=_pieces[opponentId][opponentPlayerKingIndex]->getBoardPos();

    const int32_t size=static_cast<int32_t>(_pieces[_currPlayerId].size());
    for(int32_t idx=0;idx<size;++idx){
        const std::vector<TileData> currPlayerMoveTiles=_pieces[_currPlayerId][idx]->getMoveTiles(_pieces);
        for(const TileData& tileData:currPlayerMoveTiles){
            if(TileType::TAKE!=tileData.tileType){
                continue;
            }
            if(opponentPlayerKingPosition!=tileData.boardPos){
                continue;
            }
            _isKingInCheck=true;
            std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
            if(!PieceHandler::isOpponentKingInMate()){
                std::cerr<<"pieces is in CHECK!"<<std::endl;
            }
            return true;
        }
    }

    return false;
}

bool PieceHandler::isOpponentKingInMate(){ // PieceHandler::checkKingForMate() is NOT added by Zhivko
    const int32_t opponentPlayerId=BoardUtils::getOpponentId(_currPlayerId);

    int32_t selectedPieceId=-1;
    for(const std::unique_ptr<ChessPiece>& piece:_pieces[opponentPlayerId]){
        ++selectedPieceId;
        std::vector<TileData> moveTiles=piece->getMoveTiles(_pieces);
        for(TileData& tileData : moveTiles){
            if(TileType::GUARD==tileData.tileType){
                continue;
            }

            int32_t collisionIdx=INVALID_RSRC_ID;
            if(TileType::TAKE==tileData.tileType){
                BoardUtils::doCollideWithPiece(tileData.boardPos,_pieces[_currPlayerId],collisionIdx);
            }
            BoardUtils::checkForEnPassant(piece,_pieces[_currPlayerId],tileData.boardPos,collisionIdx);
            
            if(!PieceHandler::isNextMoveCheckForKing(opponentPlayerId,selectedPieceId,collisionIdx,tileData.boardPos)){
                return false;
            }
        }
    }
    if(_isKingInCheck){
        std::cerr<<"pieces is in CHECKMATE."<<std::endl;
        std::cerr<<"The game has finished."<<std::endl;
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID==_currPlayerId? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has won the game!"<<std::endl;
    }
    _gameProxy->onGameFinish();
    return true;
}

void PieceHandler::isOpponentInStalemate(){ // PieceHandler::isOpponentInStalemate() is NOT added by Zhivko
    if(!PieceHandler::isOpponentKingInMate()){
        return;
    }

    std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId? std::cerr<<"white " : std::cerr<<"black ";
    std::cerr<<"pieces is in STALEMATE. The game ends in a draw!"<<std::endl;
}

bool PieceHandler::isNextMoveCheckForKing(int32_t playerId, int32_t selectedPieceId, int32_t collisionIdx, const BoardPos& boardPos){ // PieceHandler::isNextMoveCheckForKing() is NOT added by Zhivko
    // the function checks if the King of player "playerId" is given check by any of the chess pieces of player "opponentPlayerID"

    const int32_t playerKingIdx=getKingIndex(playerId);
    const int32_t opponentPlayerId=BoardUtils::getOpponentId(playerId);
    const int32_t opponentKingIndex=PieceHandler::getKingIndex(opponentPlayerId);

    if(INVALID_RSRC_ID==playerKingIdx){
        std::cerr<<"Error, PieceHandler::getKingIndex() cannot find KING piece for player with ID: "<<playerId<<std::endl;
        return true;
    }

    const std::unique_ptr<ChessPiece>& playerSelectedPiece=_pieces[playerId][selectedPieceId]; // why put a "const" qualifier in front ?
    const BoardPos playerSelectedPieceOldPosition=playerSelectedPiece->getBoardPos();
    playerSelectedPiece->setBoardPos(boardPos);

    const BoardPos& currPlayerKingBoardPos=_pieces[playerId][playerKingIdx]->getBoardPos(); // the position of the king of the current Player
    const BoardPos opponentKingBoardPos=_pieces[opponentPlayerId][opponentKingIndex]->getBoardPos();
    if(INVALID_RSRC_ID==collisionIdx){
        const bool isTaken=isPositionTaken(opponentPlayerId,currPlayerKingBoardPos,opponentKingBoardPos);
        playerSelectedPiece->setBoardPos(playerSelectedPieceOldPosition);
        
        if(isTaken){
            return true;
        }
        return false;
    }

    const std::unique_ptr<ChessPiece>& opponentCollidedPiece=_pieces[opponentPlayerId][collisionIdx]; // why put a "const" qualifier in front ?
    const BoardPos opponentCollidedPieceOldPosition=opponentCollidedPiece->getBoardPos();
    
    opponentCollidedPiece->setBoardPos(opponentKingBoardPos);

    const bool isTaken=PieceHandler::isPositionTaken(opponentPlayerId,currPlayerKingBoardPos,opponentKingBoardPos);
 
    playerSelectedPiece->setBoardPos(playerSelectedPieceOldPosition);
    opponentCollidedPiece->setBoardPos(opponentCollidedPieceOldPosition);
    if(isTaken){
        return true;
    }
    return false;
}

bool PieceHandler::isPositionTaken(int32_t nonAttackedPlayerId, const BoardPos& attackedKingPos, const BoardPos& nonAttackedKingPos) const { // PieceHandler::isPositionTaken() is NOT added by Zhivko
    const int32_t numOfPieces = static_cast<int32_t>(_pieces[nonAttackedPlayerId].size());
    for(int32_t idx=0;idx<numOfPieces;++idx){

        const std::unique_ptr<ChessPiece>& piece=_pieces[nonAttackedPlayerId][idx];

        if(PieceType::KING!=piece->getPieceType() && nonAttackedKingPos==piece->getBoardPos()){
            continue;
        }

        const std::vector<TileData> moveTiles=piece->getMoveTiles(_pieces);
        for(const TileData& tileData : moveTiles){
            if(TileType::TAKE!=tileData.tileType){
                continue;
            }

            if(attackedKingPos!=tileData.boardPos){
                continue;
            }          
            return true;
        }
    }
    return false;
}

int32_t PieceHandler::getKingIndex(int32_t playerId) const { // PieceHandler::getKingIndex() is NOT added by Zhivko
    const size_t size=_pieces[playerId].size();
    for(size_t idx=0;idx<size;++idx){
        if(PieceType::KING==_pieces[playerId][idx]->getPieceType()){
            return static_cast<int32_t>(idx);
        }
    }
    return INVALID_RSRC_ID;
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
    _isPawnPromoted=false; // NOT added by Zhivko
    if(!PieceHandler::isOpponentKingInCheck()){ // NOT added by Zhivko
        PieceHandler::isOpponentInStalemate(); // NOT added by Zhivko
    }
}

// PieceHandler::restart() is added by me
int32_t PieceHandler::restart(const std::function<void()>& gameRegenerateFboCallBack){

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
    PieceHandler::setCurrentPlayerId(Defines::WHITE_PLAYER_ID);
    _isPieceGrabbed=false;
    _isKingInCheck=false;
    _isCastlingPossible=false;
    _isPawnPromoted=false;
    
    return EXIT_SUCCESS;
}

void PieceHandler::onPawnPromotion(){ // PieceHandler::onPawnPromotion() is NOT added by Zhivko
    _isPawnPromoted=true;
}

void PieceHandler::onTurnTimeElapsed(){ // PieceHandler::onTurnTimeElapsed() is NOT added by Zhivko
    if(_isKingInCheck){
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID==_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has run out of time to move a chess piece."<<std::endl;
        std::cerr<<"The game has finished."<<std::endl;
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has won the game."<<std::endl;
        _gameProxy->onGameFinish();
    }
    _isPieceGrabbed=false;
    _gameProxy->onGameTurnFinished();
}