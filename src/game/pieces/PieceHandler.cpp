// Corresponding header
#include "game/pieces/PieceHandler.h"
// C system headers
// C++ system headers
#include <iostream>
#include <cmath>  // NOT added by Zhivko
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

extern const int32_t GAME_X_POS_SHIFT;
extern const int32_t GAME_Y_POS_SHIFT;

int32_t PieceHandler::init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy,int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId, int32_t movePieceTimerId){

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

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(_gameProxy, static_cast<PieceHandlerProxy*>(this), whitePiecesRsrcId,blackPiecesRsrcId, _pieces)){
        std::cerr<<"PieceHandlerPopulator::populatePieceHandler() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    const Point absPosZeros=BoardUtils::getAbsPos(BoardPos(0,0));
    const Point absPosOnes=BoardUtils::getAbsPos(BoardPos(1,1));
    const int32_t TILE_SIZE=absPosOnes.x-absPosZeros.x; // "absPosOnes.y-absPosZeros.y" gives the same result
    const int32_t FIRST_TILE_X_POS=absPosZeros.x;
    const int32_t FIRST_TILE_Y_POS=absPosZeros.y;

    if(EXIT_SUCCESS!=_pieceMoveAnimator.init(gameProxy,movePieceTimerId,TILE_SIZE,FIRST_TILE_X_POS,FIRST_TILE_Y_POS,_collisionIdx,
                                    [&](){return PieceHandler::isOpponentKingInCheck();},
                                    [&](){PieceHandler::isOpponentInStalemate();})){
        std::cerr<<"PieceMoveAnimator::init() failed\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void PieceHandler::drawOnFbo(Fbo& fbo) const {
    if(_gameProxy->isWinnerAnimatorActive()){
        const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
            // The entire IF statement and its content are NOT added by Zhivko
        for(const std::unique_ptr<ChessPiece>& piece:_pieces[opponentId]){
            piece->drawOnFbo(fbo);
        }
        return;
    }
    
    if(_gameProxy->isPieceMovementActive()){
        // The entire IF statement and its content are NOT added by Zhivko
        const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
        for(size_t i=0;i<_pieces.size();++i){
            for(size_t j=0;j<_pieces[i].size();++j){
                if(static_cast<int32_t>(j)==_selectedPieceId && static_cast<int32_t>(i)==_currPlayerId){
                    continue;
                }
                if(static_cast<int32_t>(j)==pair.first && static_cast<int32_t>(i)==_currPlayerId){
                    continue;
                }
                if(static_cast<int32_t>(j)==_collisionIdx && static_cast<int32_t>(i)==opponentId){
                    continue;
                }
                _pieces[i][j]->drawOnFbo(fbo);
            }
        }
        return;
    }
   
    for(const ChessPiece::PlayerPieces& playerPieces:_pieces){
        for(const std::unique_ptr<ChessPiece>& piece:playerPieces){
            piece->drawOnFbo(fbo);
        }
    }
}

void PieceHandler::draw() const{
    /* Original code used by Zhivko
    
    for(const ChessPiece::PlayerPieces& playerPieces:_pieces){
        for(const std::unique_ptr<ChessPiece>& piece:playerPieces){
            piece->draw();
        }
    }
    */
   
   // The below code is NOT added by Zhivko
    if(!_gameProxy->isWinnerAnimatorActive()){
        if(!_gameProxy->isPieceMovementActive()){
            return;
        }
        _pieces[_currPlayerId][_selectedPieceId]->draw();
        if(INVALID_RSRC_ID!=_collisionIdx){
            const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
            _pieces[opponentId][_collisionIdx]->draw();
        }
        
        if(INVALID_RSRC_ID!=pair.first){
            _pieces[_currPlayerId][pair.first]->draw();
        }
        return;
    }

    for(const std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        piece->draw();
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
        return;
    }

    _targetBoardPos=BoardUtils::getBoardPos(e.pos);
    if(!_gameBoardProxy->isMoveAllowed(_targetBoardPos)){
        _gameBoardProxy->onPieceUngrabbed();
        return;
    }

    const std::unique_ptr<ChessPiece>& currPlayerSelectedPiece=_pieces[_currPlayerId][_selectedPieceId]; // NOT added by Zhivko
    
    if(_isCastlingPossible){
        BoardUtils::getBoardPosIfCastling(_pieces[_currPlayerId],currPlayerSelectedPiece,_targetBoardPos,pair); // NOT added by Zhivko
    }
    
    if(INVALID_RSRC_ID==pair.first){ // NOT added by Zhivko
        if(!PieceHandler::isMoveValid()){ // NOT added by Zhivko
            _gameBoardProxy->onPieceUngrabbed();
            return;
        }        
    }
    PieceHandler::doMovePiece();
}

void PieceHandler::handlePieceUngrabbedEvent(const InputEvent& e){
    if(TouchEvent::TOUCH_RELEASE!=e.type){
        return;
    }
    _isCastlingPossible=false;
    int32_t relativePieceId=-1;
    for(const std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        ++relativePieceId;
        if(piece->getIsTaken()){
            continue; // this entire IF statement is NOT added by Zhivk
        }

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
    }
}

void PieceHandler::doMovePiece(){
    PieceHandler::checkPawnMoveForEnPassant();
    _pieceMoveAnimator.start(_pieces,_targetBoardPos,pair,_currPlayerId,_selectedPieceId,_collisionIdx);
    _gameProxy->setPieceMovementActive(true);
    _gameBoardProxy->onPieceUngrabbed();
}

void PieceHandler::checkPawnMoveForEnPassant(){
    if(nullptr!=_enPassantPawnId){
        ChessPiece* const chessPiecePtr=_enPassantPawnId->get();
        Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
        pawnPtr->setIsPawnTargetedForEnPassant(false);
        _enPassantPawnId=nullptr;
    }
    
    std::unique_ptr<ChessPiece>& pieceRef=_pieces[_currPlayerId][_selectedPieceId];
    if(PieceType::PAWN!=pieceRef->getPieceType()){
        return;
    }

    ChessPiece* const chessPiecePtr=pieceRef.get();
    Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
    pawnPtr->checkMoveForEnPassant(_targetBoardPos);
    if(pawnPtr->isPawnTargetedForEnPassant()){
        _enPassantPawnId=&pieceRef;
    }
}

// void PieceHandler::checkPawnsStateForEnPassant(){ // PieceHandler::checkPawnsForEnPassant() is NOT added by Zhivko

//     const std::unique_ptr<ChessPiece>* chessPieceUnqPtr=&_pieces[_currPlayerId][_selectedPieceId];
//     ChessPiece* const chessPiecePtr=chessPieceUnqPtr->get();
//     const PieceType selectedPieceType=chessPiecePtr->getPieceType();

//     if(PieceType::PAWN!=selectedPieceType){
//         const size_t size=_pieces[_currPlayerId].size();
//         for(size_t indx=0; indx<size;++indx){
//             if(PieceType::PAWN==_pieces[_currPlayerId][indx]->getPieceType()){
//                 chessPieceUnqPtr=&_pieces[_currPlayerId][indx];
//                 break;
//             }
//         }
//     }
//     Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
//     pawnPtr->checkStateForEnPassant(_targetBoardPos,_pieces[_currPlayerId],selectedPieceType);
// }

bool PieceHandler::isMoveValid(){ // PieceHandler::isMoveValid() is NOT added by Zhivko
    const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);

    BoardUtils::doCollideWithPiece(_targetBoardPos,_pieces[opponentId],_collisionIdx);
    BoardUtils::getBoardPosIfEnPassant(_pieces[_currPlayerId][_selectedPieceId],_pieces[opponentId],_targetBoardPos,_collisionIdx);
    const int32_t collisionIdx=_collisionIdx;

    if(PieceHandler::isNextMoveCheckForKing()){
        return false;
    }
    _collisionIdx=collisionIdx;

    const int32_t kingIdx=PieceHandler::getKingIndex(_currPlayerId);
    ChessPiece* const chessPiecePtr=_pieces[_currPlayerId][kingIdx].get();
    King* const kingPiecePtr=static_cast<King*>(chessPiecePtr);

    if(kingPiecePtr->isInCheck()){
        kingPiecePtr->setIsInCheck(false);
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
        if(!rookPiecePtr->getIsCastlingPossible()){
            return false;
        }
        const int32_t kingIdx=PieceHandler::getKingIndex(_currPlayerId);
        const BoardPos kingBoardPos=_pieces[_currPlayerId][kingIdx]->getBoardPos();
        _gameBoardProxy->onCastling(kingBoardPos);
        _gameProxy->castleTextShow();
        return true;
    }

    const King* const kingPiecePtr=static_cast<const King*>(chessPiecePtr);

    if(!kingPiecePtr->getIsCastlePossible()){
        return false;
    }
    _gameProxy->castleTextShow();
    return true;
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
            ChessPiece* const chessPiecePtr=_pieces[opponentId][opponentPlayerKingIndex].get();
            King* const kingPiecePtr=static_cast<King*>(chessPiecePtr);
            kingPiecePtr->setIsInCheck(true);
            std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
            if(!PieceHandler::isOpponentKingInMate(true)){
                std::cerr<<"pieces is in CHECK!"<<std::endl;
            }
            return true;
        }
    }
    return false;
}

bool PieceHandler::isOpponentKingInMate(bool isKingInCheck){ // PieceHandler::isOpponentKingInMate() is NOT added by Zhivko
    const int32_t currPlayerId=_currPlayerId; // save the initial value
    const int32_t selectedPieceId=_selectedPieceId; // save the initial value
    const BoardPos targetBoardPos=_targetBoardPos; // save the initial value

    _currPlayerId=BoardUtils::getOpponentId(_currPlayerId);
    
    _selectedPieceId=0;

    for(const std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        const std::vector<TileData> moveTiles=piece->getMoveTiles(_pieces);
        for(const TileData& tileData : moveTiles){
            if(TileType::GUARD==tileData.tileType){
                continue;
            }

            const int32_t oppPlayerId=BoardUtils::getOpponentId(_currPlayerId);
            _targetBoardPos=tileData.boardPos;

            if(TileType::TAKE==tileData.tileType){
                BoardUtils::doCollideWithPiece(_targetBoardPos,_pieces[oppPlayerId],_collisionIdx);
            }
            BoardUtils::getBoardPosIfEnPassant(piece,_pieces[oppPlayerId],_targetBoardPos,_collisionIdx);
            
            if(!PieceHandler::isNextMoveCheckForKing()){
                _currPlayerId=currPlayerId; _selectedPieceId=selectedPieceId; _targetBoardPos=targetBoardPos;
                return false;
            }
        }
        ++_selectedPieceId;
    }

    _currPlayerId=currPlayerId; _selectedPieceId=selectedPieceId; _targetBoardPos=targetBoardPos; 
    if(isKingInCheck){
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

bool PieceHandler::isNextMoveCheckForKing(){ // PieceHandler::isNextMoveCheckForKing() is NOT added by Zhivko 
    // the function checks if the King of player "playerId" is given check by any of the chess pieces of player "opponentPlayerID"
    const int32_t playerKingIdx=getKingIndex(_currPlayerId);
    const int32_t opponentPlayerId=BoardUtils::getOpponentId(_currPlayerId);

    if(INVALID_RSRC_ID==playerKingIdx){
        std::cerr<<"Error, PieceHandler::getKingIndex() cannot find KING piece for player with ID: "<<_currPlayerId<<std::endl;
        return true;
    }

    const std::unique_ptr<ChessPiece>& playerSelectedPiece=_pieces[_currPlayerId][_selectedPieceId]; // why put a "const" qualifier in front ?
    const BoardPos playerSelectedPieceOldPosition=playerSelectedPiece->getBoardPos();
    playerSelectedPiece->setBoardPos(_targetBoardPos);
    const BoardPos currPlayerKingBoardPos=_pieces[_currPlayerId][playerKingIdx]->getBoardPos(); // the position of the king of the current Player
    
    const bool isTaken=isPositionTaken(opponentPlayerId,currPlayerKingBoardPos);
    playerSelectedPiece->setBoardPos(playerSelectedPieceOldPosition);
    if(0<=_collisionIdx){
        _collisionIdx=INVALID_RSRC_ID;
    }
    if(isTaken){
        return true;
    }
    return false;
}

bool PieceHandler::isPositionTaken(const int32_t nonAttackedPlayerId, const BoardPos& attackedKingPos) const { // PieceHandler::isPositionTaken() is NOT added by Zhivko
    const int32_t numOfPieces = static_cast<int32_t>(_pieces[nonAttackedPlayerId].size());
    for(int32_t idx=0;idx<numOfPieces;++idx){
        const std::unique_ptr<ChessPiece>& piece=_pieces[nonAttackedPlayerId][idx];
        if(idx==_collisionIdx){
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
    std::unique_ptr<ChessPiece>& currPiece=_pieces[_currPlayerId][_selectedPieceId];
    const auto boardPos=currPiece->getBoardPos();
    const auto piecePlayerId=currPiece->getPlayerId();
    const auto pieceRsrcId=currPiece->getRsrcId();

    currPiece.reset();

    ChessPieceCfg pieceCfg;
    pieceCfg.boardPos=boardPos;
    pieceCfg.playerId=piecePlayerId;
    pieceCfg.pieceType=pieceType;
    pieceCfg.rsrcId=pieceRsrcId;

    currPiece=PieceHandlerPopulator::createPiece(pieceType,_gameProxy,static_cast<PieceHandlerProxy*>(this));
    if(EXIT_SUCCESS!=currPiece->init(pieceCfg)){
        std::cerr<<"Error, PieceHandlerPopulator::createPiece() failed. Pawn promotion failed!"<<std::endl;
        return;
    }

    if(Defines::BLACK_PLAYER_ID==_currPlayerId){ // the entire IF statement is NOT added by Zhivko
        currPiece->setBoardPos(_targetBoardPos);
        if(INVALID_RSRC_ID!=_collisionIdx){
            const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
            const BoardPos boardPosTaken=_pieces[opponentId][_collisionIdx]->getBoardPos();
            const BoardPos boardPosTakenInv=_pieceMoveAnimator.getTakenPieceBoardPosInv(boardPosTaken);
            _pieces[opponentId][_collisionIdx]->setBoardPos(boardPosTakenInv);
            _collisionIdx=INVALID_RSRC_ID;         
        }
    }

    _gameProxy->setPieceMovementActive(false); // NOT added by Zhivko
    if(!PieceHandler::isOpponentKingInCheck()){ // NOT added by Zhivko
        PieceHandler::isOpponentInStalemate(); // NOT added by Zhivko
    }
}

// PieceHandler::restart() is added by me
int32_t PieceHandler::restart(){
    int32_t piecesWhitesRsrcId=INVALID_RSRC_ID;
    int32_t piecesBlackRsrcId=INVALID_RSRC_ID;
    for(ChessPiece::PlayerPieces& pieces:_pieces){
        std::unique_ptr<ChessPiece>& firstPiece=pieces.front();

        Defines::WHITE_PLAYER_ID==firstPiece->getPlayerId() ? piecesWhitesRsrcId=firstPiece->getRsrcId() : piecesBlackRsrcId=firstPiece->getRsrcId();
   
        pieces.clear(); // investigate further if the std::unique_ptr() will take care for deleting ChessPiece
    }

    if(EXIT_SUCCESS!=PieceHandlerPopulator::populatePieceHandler(_gameProxy,static_cast<PieceHandlerProxy*>(this),
                                                                    piecesWhitesRsrcId,piecesBlackRsrcId,_pieces)){
        std::cerr<<"PieceHandlerPopulator::populatePieceHandler() failed"<<std::endl;
        return EXIT_FAILURE;
    }

    PieceHandler::setCurrentPlayerId(Defines::WHITE_PLAYER_ID);
    _isPieceGrabbed=false;
    _isCastlingPossible=false;
    return EXIT_SUCCESS;
}

void PieceHandler::onTurnTimeElapsed(){ // PieceHandler::onTurnTimeElapsed() is NOT added by Zhivko
    const int32_t kingIdx=PieceHandler::getKingIndex(_currPlayerId);
    const ChessPiece* chessPiecePtr=_pieces[_currPlayerId][kingIdx].get();
    const King* kingPiecePtr=static_cast<const King*>(chessPiecePtr);

    if(kingPiecePtr->isInCheck()){
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID==_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has run out of time to move a chess piece."<<std::endl;
        std::cerr<<"The game has finished."<<std::endl;
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has won the game."<<std::endl;
        _gameProxy->onGameFinish();
        _gameProxy->setAutomaticWin(true);
    }
    _isPieceGrabbed=false;
    _gameBoardProxy->onPieceUngrabbed();
    _gameProxy->onGameTurnFinished();
}

const ChessPiece::PlayerPieces& PieceHandler::getWinnerPieces(){
    return _pieces[_currPlayerId];
}

void PieceHandler::changePawnPosIfEnPassant(const std::pair<int32_t,int32_t>& pawnPair, const BoardPos& boardPos){ // PieceHandler::changePawnPosIfEnPassant() is NOT added by Zhivko
    // NOTE: this method is only used in the Pawn::areEnPassantTilesValid() method
    _pieces[pawnPair.first][pawnPair.second]->setBoardPos(boardPos);
}

void PieceHandler::rotateWinnerPieces(double angle){

    // obtain the current rotation angle of an arbitrary chess piece (i.e the first one)...
    // the remaining chess pieces feature the same current rotation angle
    const double currAngle=_pieces[_currPlayerId].front()->getRotationAngle();

    for(const std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        piece->setRotationAngle(currAngle+angle);
    }
}

void PieceHandler::shiftWinnerPiecesPos(){ // NOT added by Zhivko

    if(Defines::BLACK_PLAYER_ID==_currPlayerId){
        if(_gameProxy->isAutomaticWin()){
            for(std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
                const BoardPos& pieceOldBoardPos=piece->getBoardPos();
                const BoardPos& invertedPieceBoardPos=BoardUtils::getInvertedBoardPos(pieceOldBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
                const Point& invertedPieceAbsPos=BoardUtils::getAbsPos(invertedPieceBoardPos);
                const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(invertedPieceAbsPos.x+GAME_X_POS_SHIFT,invertedPieceAbsPos.y+GAME_Y_POS_SHIFT));
                piece->setBoardPos(pieceNewBoardPos);
                piece->setWidgetFlip(WidgetFlip::NONE);
            }
            return;
        }
        for(std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
            const BoardPos& pieceOldBoardPos=piece->getBoardPos();
            const Point& pieceAbsPos=BoardUtils::getAbsPos(pieceOldBoardPos);
            const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(pieceAbsPos.x+GAME_X_POS_SHIFT,pieceAbsPos.y+GAME_Y_POS_SHIFT));
            piece->setBoardPos(pieceNewBoardPos);
        }
        return;
    }

    if(_gameProxy->isAutomaticWin()){
        for(std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
            const BoardPos& pieceOldBoardPos=piece->getBoardPos();
            const Point& pieceAbsPos=BoardUtils::getAbsPos(pieceOldBoardPos);
            const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(pieceAbsPos.x+GAME_X_POS_SHIFT,pieceAbsPos.y+GAME_Y_POS_SHIFT));
            piece->setBoardPos(pieceNewBoardPos);
        }
        return;
    }
    for(std::unique_ptr<ChessPiece>& piece:_pieces[_currPlayerId]){
        const BoardPos& pieceOldBoardPos=piece->getBoardPos();
        const BoardPos& invertedPieceBoardPos=BoardUtils::getInvertedBoardPos(pieceOldBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        const Point& invertedPieceAbsPos=BoardUtils::getAbsPos(invertedPieceBoardPos);
        const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(invertedPieceAbsPos.x+GAME_X_POS_SHIFT,invertedPieceAbsPos.y+GAME_Y_POS_SHIFT));
        piece->setBoardPos(pieceNewBoardPos);
        piece->setWidgetFlip(WidgetFlip::NONE);
    }
}