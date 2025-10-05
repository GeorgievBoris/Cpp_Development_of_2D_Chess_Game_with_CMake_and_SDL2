// Corresponding header
#include "game/pieces/PieceHandler.h"
// C system headers
#include <cstring>
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

int32_t PieceHandler::init(GameBoardProxy* gameBoardProxy, GameProxy* gameProxy,
                 int32_t whitePiecesRsrcId, int32_t blackPiecesRsrcId,
                 int32_t whitePiecesHalvesRsrcId, int32_t blackPiecesHalvesRsrcId,
                 int32_t movePieceTimerId, int32_t movePieceHalvesTimerId){

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

    if(EXIT_SUCCESS!=_pieceMoveAnimator.init(gameProxy, whitePiecesHalvesRsrcId, blackPiecesHalvesRsrcId,
                                             movePieceTimerId,movePieceHalvesTimerId,
                                             TILE_SIZE,FIRST_TILE_X_POS,FIRST_TILE_Y_POS, _collisionIdx,
                                            [&](){return PieceHandler::isOpponentKingInCheck();},
                                            [&](bool isOpponentKingInCheck){return PieceHandler::isOpponentInStalemate(isOpponentKingInCheck);},
                                            [&](){return PieceHandler::isDeadPosition();})){
        std::cerr<<"PieceMoveAnimator::init() failed\n";
        return EXIT_FAILURE;
    }
  
    return EXIT_SUCCESS;
}

void PieceHandler::drawOnFbo(Fbo& fbo) const {
    
    if(!_gameProxy->isGameBoardAnimatorActive()){
        const GameEndType gameEndType=_gameProxy->getGameEndType();
        if(GameEndType::NONE!=gameEndType && GameEndType::DRAW!=gameEndType){
            int32_t playerId;
            GameEndType::WINNER_AUTOMATIC==gameEndType ? playerId=_currPlayerId : playerId=BoardUtils::getOpponentId(_currPlayerId);
            for(const std::unique_ptr<ChessPiece>& piece:_pieces[playerId]){
                piece->drawOnFbo(fbo);
            }
            return;            
        } else if(GameEndType::DRAW==gameEndType){
            return;
        }
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

void PieceHandler::draw() const {
    
    if(_gameProxy->isGameBoardAnimatorActive()){
        return;
    }

    const GameEndType gameEndType=_gameProxy->getGameEndType();
    if(GameEndType::NONE==gameEndType){
        if(!_gameProxy->isPieceMovementActive()){
            return;
        }
        _pieces[_currPlayerId][_selectedPieceId]->draw();
        if(INVALID_RSRC_ID!=_collisionIdx){
            if(_pieceMoveAnimator.isCapturedPieceActive()){
                _pieceMoveAnimator.draw();
            } else {
                const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
                _pieces[opponentId][_collisionIdx]->draw();
            }
        }
        
        if(INVALID_RSRC_ID!=pair.first){
            _pieces[_currPlayerId][pair.first]->draw();
        }
        return;
    }    
    
    if(GameEndType::DRAW==gameEndType){
        for(const ChessPiece::PlayerPieces& player:_pieces){
            for(const std::unique_ptr<ChessPiece>& piece:player){
                piece->draw();
            }
        }        
        return;
    }
    int32_t playerId;
    GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType() ? playerId=BoardUtils::getOpponentId(_currPlayerId) : playerId=_currPlayerId;
   
    for(const std::unique_ptr<ChessPiece>& piece:_pieces[playerId]){
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
    _pieceMoveAnimator.start(_pieces,_targetBoardPos,pair,_currPlayerId,_selectedPieceId,_pawnEnPassantPtr);
    _gameProxy->setPieceMovementActive(true);
    _gameBoardProxy->onPieceUngrabbed();
    
    if(INVALID_RSRC_ID==_collisionIdx){
        return;
    }
    Defines::WHITE_PLAYER_ID==_currPlayerId ? --_blackPiecesCount : --_whitePiecesCount;
    const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
    const PieceType takenPieceType=_pieces[opponentId][_collisionIdx]->getPieceType();
    if(PieceType::KNIGHT==takenPieceType || PieceType::BISHOP==takenPieceType){
        --_bishopsAndKnightsCount;
    }
}

void PieceHandler::checkPawnMoveForEnPassant(){
    std::unique_ptr<ChessPiece>& currPieceRef=_pieces[_currPlayerId][_selectedPieceId];    
    
    if(PieceType::PAWN!=currPieceRef->getPieceType()){
        if(nullptr!=_pawnEnPassantPtr){
            ChessPiece* const chessPiecePtr=_pawnEnPassantPtr->get();
            Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
            pawnPtr->setIsPawnTargetedForEnPassant(false);
            _pawnEnPassantPtr=nullptr;
        }
        return;
    }

    if(PieceType::PAWN==currPieceRef->getPieceType()){
        if(nullptr!=_pawnEnPassantPtr){
            if(_targetBoardPos.row!=currPieceRef->getBoardPos().row){
                ChessPiece* const chessPiecePtr=_pawnEnPassantPtr->get();
                Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
                pawnPtr->setIsPawnTargetedForEnPassant(false);
                _pawnEnPassantPtr=nullptr;                
            } else {
                return;
            }
        }
    }

    ChessPiece* const chessPiecePtr=currPieceRef.get();
    Pawn* const pawnPtr=static_cast<Pawn*>(chessPiecePtr);
    pawnPtr->checkMoveForEnPassant(_targetBoardPos);
    if(pawnPtr->isPawnTargetedForEnPassant()){
        _pawnEnPassantPtr=&currPieceRef;
    }
}

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
            return true;
        }
    }
    return false;
}

bool PieceHandler::isOpponentInStalemate(bool isKingInCheck){ // PieceHandler::isOpponentKingInMate() is NOT added by Zhivko
    const int32_t currPlayerId=_currPlayerId; // save the initial value
    const int32_t selectedPieceId=_selectedPieceId; // save the initial value
    const BoardPos targetBoardPos=_targetBoardPos; // save the initial value
    
    if(isKingInCheck){
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
    }

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
                if(isKingInCheck){
                    std::cerr<<"pieces is in CHECK!"<<std::endl;
                }
                return false;
            }
        }
        ++_selectedPieceId;
    }

    _currPlayerId=currPlayerId; _selectedPieceId=selectedPieceId; _targetBoardPos=targetBoardPos; 
    if(isKingInCheck){
        std::cerr<<"pieces is in CHECKMATE!"<<std::endl;
        std::cerr<<"The game has finished!"<<std::endl;
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID==_currPlayerId? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces has won the game!"<<std::endl;
    } else {
        std::cerr<<"The player with "; Defines::WHITE_PLAYER_ID!=_currPlayerId ? std::cerr<<"white " : std::cerr<<"black ";
        std::cerr<<"pieces is in STALEMATE. The game ends in a draw!"<<std::endl;              
    }

    return true;
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
    currPiece->activateAlphaModulation();

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

    const bool isOpponentKingInCheck=PieceHandler::isOpponentKingInCheck();
    if(PieceHandler::isOpponentInStalemate(isOpponentKingInCheck)){
        isOpponentKingInCheck ? _gameProxy->setGameEndType(GameEndType::WINNER_NON_AUTOMATIC) : _gameProxy->setGameEndType(GameEndType::DRAW);
        return;
    }

    if(isOpponentKingInCheck){
        return;
    }

    // NOTE: probably it does not make sense to check for a dead position, when a pawn piece gets promoted
    if(PieceHandler::isDeadPosition()){
        _gameProxy->setGameEndType(GameEndType::DRAW);   
    }

}

int32_t PieceHandler::restart(){
    int32_t piecesWhitesRsrcId=INVALID_RSRC_ID;
    int32_t piecesBlackRsrcId=INVALID_RSRC_ID;
    for(ChessPiece::PlayerPieces& pieces:_pieces){
        for(std::unique_ptr<ChessPiece>&piece:pieces){
            piece->deactivateAlphaModulation();
        }
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
        _gameProxy->setGameEndType(GameEndType::WINNER_AUTOMATIC);
    }
    _isPieceGrabbed=false;
    _gameBoardProxy->onPieceUngrabbed();
    _gameProxy->onGameTurnFinished();
}

const ChessPiece::PlayerPieces& PieceHandler::getWinnerPieces(){
    if(GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType()){
        const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
        return _pieces[opponentId];
    }
    return _pieces[_currPlayerId];
    // return _pieces[_currPlayerId];
}

void PieceHandler::changePawnPosIfEnPassant(const std::pair<int32_t,int32_t>& pawnPair, const BoardPos& boardPos){ // PieceHandler::changePawnPosIfEnPassant() is NOT added by Zhivko
    // NOTE: this method is only used in the Pawn::areEnPassantTilesValid() method
    _pieces[pawnPair.first][pawnPair.second]->setBoardPos(boardPos);
}

void PieceHandler::rotateWinnerPieces(const bool isNoWinner, const double angle){

    // logic is: obtain the current rotation angle of an arbitrary chess piece (i.e the first one)...
    // the remaining chess pieces feature the same current rotation angle
    
    int32_t playerId;
    GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType() ? playerId=BoardUtils::getOpponentId(_currPlayerId) : playerId=_currPlayerId;

    const double currAngle=_pieces[playerId].front()->getRotationAngle();

    if(isNoWinner){
        for(const ChessPiece::PlayerPieces& player:_pieces){
            for(const std::unique_ptr<ChessPiece>& piece:player){
                // how is  that possible ... a constant reference calls a non-constatnt method and no errors are given by compiler ???
                piece->setRotationAngle(currAngle+angle);
            }
        }
        return;   
    }

    for(const std::unique_ptr<ChessPiece>& piece:_pieces[playerId]){
        // how is  that possible ... a constant reference calls a non-constatnt method and no errors are given by compiler ???
        piece->setRotationAngle(currAngle+angle);
    }
}

const BoardPos PieceHandler::getBoardPosOfKingAndAttackingPiece(const int32_t playerId) const {

    if(GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType()){
        if(playerId==_currPlayerId){
            const int32_t kingIndx=PieceHandler::getKingIndex(playerId);
            return _pieces[playerId][kingIndx]->getBoardPos();
        }

        const int32_t lastMovedPieceId=_pieceMoveAnimator.getLastMovedPieceId();
        if(lastMovedPieceId==_selectedPieceId){
            return _pieces[playerId][_selectedPieceId]->getBoardPos();
        }
        return _pieces[playerId][lastMovedPieceId]->getBoardPos();
    }

    if(playerId!=_currPlayerId){
        const int32_t kingIndx=PieceHandler::getKingIndex(playerId);
        return _pieces[playerId][kingIndx]->getBoardPos();
    }
    
    const int32_t lastMovedPieceId=_pieceMoveAnimator.getLastMovedPieceId();
    if(lastMovedPieceId==_selectedPieceId){
        return _pieces[_currPlayerId][_selectedPieceId]->getBoardPos();
    }
    return _pieces[_currPlayerId][lastMovedPieceId]->getBoardPos();
}

void PieceHandler::shiftWinnerPiecesPos(const bool isNoWinner){ // NOT added by Zhivko
    
    if(isNoWinner){       
        for(const ChessPiece::PlayerPieces& player:_pieces){
            for(const std::unique_ptr<ChessPiece>& piece:player){
                piece->setWidgetFlip(WidgetFlip::NONE);
                if(Defines::BLACK_PLAYER_ID==_currPlayerId){
                    continue;
                }
                const BoardPos& pieceOldBoardPos=piece->getBoardPos();
                const BoardPos& invertedPieceBoardPos=BoardUtils::getInvertedBoardPos(pieceOldBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
                const Point& invertedPieceAbsPos=BoardUtils::getAbsPos(invertedPieceBoardPos);
                const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(invertedPieceAbsPos.x+GAME_X_POS_SHIFT,invertedPieceAbsPos.y+GAME_Y_POS_SHIFT));
                piece->setBoardPos(pieceNewBoardPos);
            }
        }
        return;
    }


    if(Defines::BLACK_PLAYER_ID==_currPlayerId){
        if(GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType()){
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

    if(GameEndType::WINNER_AUTOMATIC==_gameProxy->getGameEndType()){
        const int32_t opponentId=BoardUtils::getOpponentId(_currPlayerId);
        for(std::unique_ptr<ChessPiece>& piece:_pieces[opponentId]){
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
        const BoardPos& invertedPieceBoardPos=BoardUtils::getInvertedBoardPos(pieceOldBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        const Point& invertedPieceAbsPos=BoardUtils::getAbsPos(invertedPieceBoardPos);
        const BoardPos pieceNewBoardPos=BoardUtils::getBoardPos(Point(invertedPieceAbsPos.x+GAME_X_POS_SHIFT,invertedPieceAbsPos.y+GAME_Y_POS_SHIFT));
        piece->setBoardPos(pieceNewBoardPos);
        piece->setWidgetFlip(WidgetFlip::NONE);
    }
}

bool PieceHandler::isDeadPosition(){
    // check for:
    // 1) king against king
    // 2) king against king and knight
    // 3) king against king and bishop

    if(1==_whitePiecesCount && 1==_blackPiecesCount){
        return true;
    }

    const bool areThreePiecesLeft=((1==_whitePiecesCount && 2==_blackPiecesCount) || (2==_whitePiecesCount && 1==_blackPiecesCount));
    const bool isBishopOrKnightLeft = 0<_bishopsAndKnightsCount;
    if(areThreePiecesLeft && isBishopOrKnightLeft){
        return true;
    }

    if(Defines::TOTAL_PIECES_COUNT==_whitePiecesCount || Defines::TOTAL_PIECES_COUNT==_blackPiecesCount){
        return false;
    }

    // below code checks for dead position if pawns are on the board

    // constexpr size_t maxPiecesNum=10; // 8 pawns + king + 1 bishop

    // if(maxPiecesNum<_whitePiecesCount || maxPiecesNum<_blackPiecesCount){
    //     return false;
    // }

    // int32_t chessColumns[Defines::PAWNS_COUNT]{};
    // std::memset(chessColumns,2,sizeof(chessColumns));

    int32_t idx{};
    for(const ChessPiece::PlayerPieces& player:_pieces){
        for(const std::unique_ptr<ChessPiece>& piece:player){
            if(piece->getIsTaken()){
                continue;
            }

            const PieceType currPieceType=piece->getPieceType();

            if(PieceType::PAWN==currPieceType){
                const std::vector<TileData>& moveTiles = piece->getMoveTiles(_pieces); // CHECK IF recursion happens !!!
                if(moveTiles.empty()){
                    continue;
                }
            
                for(const TileData& tileData:moveTiles){
                    if(TileType::GUARD!=tileData.tileType){
                        return false;
                    }
                }
                continue;
            }

            if(PieceType::QUEEN==currPieceType){
                return false;
            }
            
            if(piece->isDeadPosition(_pieces,idx)){
                idx=0;
                continue;
            }
            return false;

            // ChessPiece* const chessPiecePtr=piece.get();
            // if(PieceType::KING==currPieceType){
            //     int32_t index{};
            //     King* const kingPiecePtr=static_cast<King*>(chessPiecePtr);
            //     if(kingPiecePtr->isDeadPosition(_pieces,index)){
            //         continue;
            //     }
            //     return false;
            // }

            // if(PieceType::BISHOP==currPieceType){
            //     int32_t index{};
            //     Bishop* const bishopPiecePtr=static_cast<Bishop*>(chessPiecePtr);
            //     if(bishopPiecePtr->isDeadPosition(_pieces,index)){
            //         continue;
            //     }
            //     return false;
            // }

            // if(PieceType::ROOK==currPieceType){
            //     int32_t index{};
            //     Rook* const rookPiecePtr=static_cast<Rook*>(chessPiecePtr);
            //     if(rookPiecePtr->isDeadPosition(_pieces,index)){
            //         continue;
            //     }
            //     return false;
            // }          

            // if(PieceType::KNIGHT==currPieceType){
            //     int32_t index{};
            //     Knight* const rookPiecePtr=static_cast<Rook*>(chessPiecePtr);
            //     if(rookPiecePtr->isDeadPosition(_pieces,index)){
            //         continue;
            //     }
            //     return false;
            // }                

        }
    }

    // int32_t idx{};
    // std::cout<<"idx = "<<idx<<'\n';
    // const int32_t numOfCols=Defines::PAWNS_COUNT-1; // last column "h" with index 7 is irrelevant for analysis-does not matter if pawn pieces are there or not

    // for(int32_t i=0;i<numOfCols;++i){
    //     idx+=chessColumns[i];

    //     if(0==i%2){ // think for smarter condition
    //         if(0<i && 2==idx){
    //             return false;
    //         }
    //         continue;
    //     }

    //     if(0==idx && 1<i){
    //         return false;
    //     }        

    //     if(4==idx){
    //         return false;
    //     }
    //     idx=0;
    // }

    return true;
}