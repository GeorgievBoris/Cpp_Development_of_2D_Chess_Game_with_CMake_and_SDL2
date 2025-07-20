// Corresponding header
#include "game/pieces/PieceMoveAnimator.h"
// C system headers
// C++ system headers
#include <iostream>
#include <memory>
// Third party-headers
// Own headers
#include "utils/drawing/DrawParams.h"
#include "game/pieces/types/Rook.h"
#include "game/pieces/types/King.h"
#include "game/pieces/types/Pawn.h"
#include "game/utils/BoardUtils.h"
#include "game/proxies/GameProxy.h"

extern const int32_t GAME_X_POS_SHIFT;
extern const int32_t GAME_Y_POS_SHIFT;


int32_t PieceMoveAnimator::init(GameProxy* gameProxy, int32_t movePieceTimerId, int32_t tileSize, int32_t firstTilePosX, int32_t firstTilePosY, int32_t& collisionIdx,
                                const std::function<bool()>& isKingInCheckCb, const std::function<void()>& isInStalemateCb){
    if(INVALID_RSRC_ID==movePieceTimerId){
        std::cerr<<"Received unsupported timerId\n";
        return EXIT_FAILURE;
    }

    if(nullptr==gameProxy){
        std::cerr<<"Error, nullptr provided for GameProxy\n";
        return EXIT_FAILURE;
    }
    _gameProxy=gameProxy;
    _movePieceTimerId=movePieceTimerId;
    _tileSize=tileSize;
    _firstTilePosX=firstTilePosX;
    _firstTilePosY=firstTilePosY;
    _isKingInCheckCb=isKingInCheckCb;
    _isInStalemateCb=isInStalemateCb;
    _collisionIdxPtr=&collisionIdx;
    return EXIT_SUCCESS;
}

void PieceMoveAnimator::start(std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& pieces, BoardPos& targetBoardPos, 
                                        std::pair<int32_t,BoardPos>& pair,
                                        const int32_t currPlayerId, const int32_t selectedPieceIdx, const int32_t collisionIdx){
    
    TimerClient::startTimer(20,_movePieceTimerId,TimerType::PULSE);

    if(Defines::BLACK_PLAYER_ID==currPlayerId){
        targetBoardPos=PieceMoveAnimator::getInvBoardPosForAnim(targetBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
    }
    _targetBoardPos=&targetBoardPos;

    std::unique_ptr<ChessPiece>& selectedPiece=pieces[currPlayerId][selectedPieceIdx];
    _movedPiecePtr=&selectedPiece;
    PieceMoveAnimator::shiftBoardPosOfMovedPiece(selectedPiece,currPlayerId);

    if(INVALID_RSRC_ID!=pair.first){
        std::unique_ptr<ChessPiece>& castlingPairPiece=pieces[currPlayerId][pair.first];
        PieceMoveAnimator::shiftBoardPosOfMovedPiece(castlingPairPiece,currPlayerId);
        _castlingPiecePtr=&castlingPairPiece;
        _castlingPairPtr=&pair;
        
        if(Defines::WHITE_PLAYER_ID==currPlayerId){
            return;
        }
        pair.second=PieceMoveAnimator::getInvBoardPosForAnim(pair.second,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        return;
    }

    if(INVALID_RSRC_ID==collisionIdx){
        return;
    }

    const int32_t opponentId=BoardUtils::getOpponentId(currPlayerId);
    std::unique_ptr<ChessPiece>& takenPiece=pieces[opponentId][collisionIdx];
    PieceMoveAnimator::shiftBoardPosOfMovedPiece(takenPiece,currPlayerId);
    _takenPiecePtr=&takenPiece;
    _opponentPiecesPtr=&pieces[opponentId];
}

BoardPos PieceMoveAnimator::getTakenPieceBoardPosInv(const BoardPos& takenPieceBoardPos){
    const Point absPosTaken=PieceMoveAnimator::getAbsPosForAnim(takenPieceBoardPos);
    const Point absPosTakenInv=PieceMoveAnimator::getInvAbsPosForAnim(absPosTaken,WidgetFlip::HORIZONTAL_AND_VERTICAL);
    const BoardPos takenPieceBoardPosInv=PieceMoveAnimator::getBoardPosForAnim(absPosTakenInv);    
    return takenPieceBoardPosInv;
}


void PieceMoveAnimator::onTimeout(int32_t timerId) {
    if(timerId!=_movePieceTimerId){
        std::cerr<<"PieceHandler received unsupported timerId: "<<timerId<<std::endl;
        return;
    }  

    BoardPos& targetBoardPos=*_targetBoardPos;
    ChessPiece* const currPiecePtr=_movedPiecePtr->get();
    const int32_t currPlayerId=currPiecePtr->getPlayerId();

    const Point absPos=PieceMoveAnimator::getAbsPosForAnim(currPiecePtr->getBoardPos());
    const Point absPosShifted(absPos.x-GAME_X_POS_SHIFT,absPos.y-GAME_Y_POS_SHIFT);
    const Point targetAbsPos=PieceMoveAnimator::getAbsPosForAnim(targetBoardPos);
    const Point absPosMoved=PieceMoveAnimator::getPosOfMovedPiece(absPosShifted,targetAbsPos,currPiecePtr->getPieceType());

    if(nullptr!=_takenPiecePtr){
        if(PieceMoveAnimator::doPiecesPosOverlap(targetAbsPos,absPosMoved)){
            // const int32_t opponentId=BoardUtils::getOpponentId(currPlayerId);
            ChessPiece* const opponentPiece=_takenPiecePtr->get();
            const Point absPosOppPiece=PieceMoveAnimator::getAbsPosForAnim(opponentPiece->getBoardPos());
            const BoardPos boardPosOppPieceShifted=PieceMoveAnimator::getBoardPosForAnim({absPosOppPiece.x-5,absPosOppPiece.y});
            opponentPiece->setBoardPos(boardPosOppPieceShifted);    
        }
    }

    if(targetAbsPos!=absPosMoved){
        const BoardPos temporaryBoardPos=PieceMoveAnimator::getBoardPosForAnim({absPosMoved.x+GAME_X_POS_SHIFT,absPosMoved.y+GAME_Y_POS_SHIFT});
        currPiecePtr->setBoardPos(temporaryBoardPos);
        if(nullptr==_castlingPairPtr){
            return;
        }
    }

    if(nullptr!=_castlingPairPtr){
        std::pair<int32_t, BoardPos>& pair=*_castlingPairPtr;
        ChessPiece* const pieceCastling=_castlingPiecePtr->get();
        const Point absPosPieceCastling=PieceMoveAnimator::getAbsPosForAnim(pieceCastling->getBoardPos());
        const Point absPosShiftedPieceCastling(absPosPieceCastling.x-GAME_X_POS_SHIFT,absPosPieceCastling.y-GAME_Y_POS_SHIFT);
        const Point targetAbsPosPieceCastling=PieceMoveAnimator::getAbsPosForAnim(pair.second);
        const Point absPosMovedPieceCastling=PieceMoveAnimator::getPosOfMovedPiece(absPosShiftedPieceCastling,targetAbsPosPieceCastling,pieceCastling->getPieceType());
        
        if(targetAbsPosPieceCastling!=absPosMovedPieceCastling){
            const BoardPos temporaryBoardPos=PieceMoveAnimator::getBoardPosForAnim({absPosMovedPieceCastling.x+GAME_X_POS_SHIFT,absPosMovedPieceCastling.y+GAME_Y_POS_SHIFT});
            pieceCastling->setBoardPos(temporaryBoardPos);
            return;
        }
        if(targetAbsPos!=absPosMoved){
            return;
        }
    }    
    
    TimerClient::stopTimer(_movePieceTimerId);
    if(Defines::BLACK_PLAYER_ID==currPlayerId){
        targetBoardPos=PieceMoveAnimator::getInvBoardPosForAnim(targetBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        if(nullptr!=_castlingPairPtr){
            std::pair<int32_t, BoardPos>& pair=*_castlingPairPtr;
            pair.second=PieceMoveAnimator::getInvBoardPosForAnim(pair.second,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        }
    }

    currPiecePtr->setBoardPos(targetBoardPos);

    const PieceType currPieceType=currPiecePtr->getPieceType();
    if(PieceType::PAWN==currPieceType){
        Pawn* const pawnPiecePtr=static_cast<Pawn*>(currPiecePtr);
        pawnPiecePtr->checkForPawnPromotion();
    } else if(PieceType::KING==currPieceType){
        King* const kingPiecePtr=static_cast<King*>(currPiecePtr);
        kingPiecePtr->setWhenFirstMoveIsMade();
    } else if(PieceType::ROOK==currPieceType){
        Rook* const rookPiecePtr=static_cast<Rook*>(currPiecePtr);
        rookPiecePtr->setWhenFirstMoveIsMade();        
    }

    if(nullptr!=_castlingPairPtr){
        // if Castling is performed, the KING or the ROOK positions get changed here
        std::pair<int32_t, BoardPos>& pair=*_castlingPairPtr;
        ChessPiece* const pieceCastling=_castlingPiecePtr->get();
        pieceCastling->setBoardPos(pair.second);
        if(PieceType::ROOK==currPieceType){
            King* const kingPiecePtr=static_cast<King*>(pieceCastling);
            kingPiecePtr->setWhenFirstMoveIsMade();
        }
        if(PieceType::KING==currPieceType){
            Rook* const rookPiecePtr=static_cast<Rook*>(pieceCastling);
            rookPiecePtr->setWhenFirstMoveIsMade();
        }
        pair.first=INVALID_RSRC_ID; pair.second=BoardPos(INVALID_RSRC_ID,INVALID_RSRC_ID); 
    }
    if(nullptr!=_takenPiecePtr){
        ChessPiece* const opponentPiece=_takenPiecePtr->get();
        opponentPiece->setIsTaken(true);
        const Point absPosTaken=PieceMoveAnimator::getAbsPosOfTakenPiece(*_opponentPiecesPtr);
        const BoardPos boardPosTaken=PieceMoveAnimator::getBoardPosForAnim(absPosTaken);
        opponentPiece->setBoardPos(boardPosTaken);
        if(_gameProxy->isPromotionActive()){
            if(Defines::BLACK_PLAYER_ID==currPlayerId){
                const Point absPosTakenInv=PieceMoveAnimator::getInvAbsPosForAnim(absPosTaken,WidgetFlip::HORIZONTAL_AND_VERTICAL);
                const BoardPos boardPosTakenInv=PieceMoveAnimator::getBoardPosForAnim(absPosTakenInv);
                opponentPiece->setBoardPos(boardPosTakenInv);
            } 
        }
    }

    _movedPiecePtr=nullptr; _takenPiecePtr=nullptr; _opponentPiecesPtr=nullptr; _targetBoardPos=nullptr;
    _castlingPairPtr=nullptr; _castlingPiecePtr=nullptr;
    
    if(_gameProxy->isPromotionActive()){
        if(Defines::BLACK_PLAYER_ID==currPlayerId){
            const BoardPos targetBoardPosInv=PieceMoveAnimator::getInvBoardPosForAnim(targetBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
            currPiecePtr->setBoardPos(targetBoardPosInv);
        }
    } else {
        *_collisionIdxPtr=INVALID_RSRC_ID;
        _gameProxy->setPieceMovementActive(false);
        if(!_isKingInCheckCb()){
            _isInStalemateCb();
        }
    }
    _gameProxy->onGameTurnFinished();
}

Point PieceMoveAnimator::getPosOfMovedPiece(const Point& currPos, const Point& targetPos, const PieceType pieceType){

    Point currAbsPos=currPos;
    switch(pieceType)
    {
    case PieceType::KNIGHT:
        {
        PieceMoveAnimator::moveDeltaX(currAbsPos,targetPos);
        if(targetPos.x==currAbsPos.x){
            PieceMoveAnimator::moveDeltaY(currAbsPos,targetPos);
        }
        return currAbsPos;
        }
    default:
        {
        PieceMoveAnimator::moveDeltaX(currAbsPos,targetPos);
        PieceMoveAnimator::moveDeltaY(currAbsPos,targetPos);
        return currAbsPos;
        }
    }
    std::cerr<<"Error, returning zero for the animated position of PieceType: "<<static_cast<uint32_t>(pieceType)<<std::endl;
    return Point::ZERO;
}

void PieceMoveAnimator::moveDeltaX(Point& currAbsPos,const Point& targetAbsPos){
    constexpr int32_t deltaX=5;
    
    if(targetAbsPos.x>currAbsPos.x){
        currAbsPos.x+=deltaX;
        if(targetAbsPos.x<currAbsPos.x){
            currAbsPos.x=targetAbsPos.x;
            return;
        }
    }
    if(targetAbsPos.x<currAbsPos.x){
        currAbsPos.x-=deltaX;
        if(targetAbsPos.x>currAbsPos.x){
            currAbsPos.x=targetAbsPos.x;
        }            
    }
}

void PieceMoveAnimator::moveDeltaY(Point& currAbsPos,const Point& targetAbsPos){
    constexpr int32_t deltaY=5;

    if(targetAbsPos.y>currAbsPos.y){
        currAbsPos.y+=deltaY;
        if(targetAbsPos.y<currAbsPos.y){
            currAbsPos.y=targetAbsPos.y;
            return;
        }
    }

    if(targetAbsPos.y<currAbsPos.y){
        currAbsPos.y-=deltaY;
        if(targetAbsPos.y>currAbsPos.y){
            currAbsPos.y=targetAbsPos.y;
        }
    }   
}

BoardPos PieceMoveAnimator::getBoardPosForAnim(const Point& absPos){
    return {(absPos.y-_firstTilePosY)/_tileSize, (absPos.x-_firstTilePosX)/_tileSize,
            (absPos.y-_firstTilePosY)%_tileSize, (absPos.x-_firstTilePosX)%_tileSize};
}
Point PieceMoveAnimator::getAbsPosForAnim(const BoardPos& boardPos){
    return {(boardPos.col*_tileSize)+_firstTilePosX+boardPos.remCol, (boardPos.row*_tileSize)+_firstTilePosY+boardPos.remRow};
}


BoardPos PieceMoveAnimator::getInvBoardPosForAnim(const BoardPos& boardPos, WidgetFlip flipType){
    switch(flipType){
    case WidgetFlip::NONE:
        return boardPos; // return BoardPos(boardPos.row,boardPos.col);
    case WidgetFlip::HORIZONTAL:
        return BoardPos((_BOARD_SIZE-1)-boardPos.row,boardPos.col,boardPos.remRow,boardPos.remCol);
    case WidgetFlip::VERTICAL:
        return BoardPos(boardPos.row,(_BOARD_SIZE-1)-boardPos.col,boardPos.remRow,boardPos.remCol);
    case WidgetFlip::HORIZONTAL_AND_VERTICAL:
        return BoardPos((_BOARD_SIZE-1)-boardPos.row,(_BOARD_SIZE-1)-boardPos.col,boardPos.remRow,boardPos.remCol);
    }

    std::cerr<<"Error, BoardUtils::getInvBoardPosForAnim() encountered unknown WidgetFlip type!"<<std::endl;
    return BoardPos(0,0,0,0);
}

Point PieceMoveAnimator::getInvAbsPosForAnim(const Point& absPos, WidgetFlip flipType){
    switch(flipType){
    case WidgetFlip::NONE:
        return absPos; // return BoardPos(boardPos.row,boardPos.col);
    case WidgetFlip::HORIZONTAL:
        return Point(absPos.x,2*_firstTilePosY+(_BOARD_SIZE-1)*_tileSize-absPos.y);
    case WidgetFlip::VERTICAL:
        return Point(2*_firstTilePosX+(_BOARD_SIZE-1)*_tileSize-absPos.x,absPos.y);
    case WidgetFlip::HORIZONTAL_AND_VERTICAL:
        return Point(2*_firstTilePosX+(_BOARD_SIZE-1)*_tileSize-absPos.x,2*_firstTilePosY+(_BOARD_SIZE-1)*_tileSize-absPos.y);
    }

    std::cerr<<"Error, BoardUtils::getInvAbsPosForAnim() encountered unknown WidgetFlip type!"<<std::endl;
    return Point::ZERO;
}

void PieceMoveAnimator::shiftBoardPosOfMovedPiece(std::unique_ptr<ChessPiece>& piece,const int32_t currPlayerId) {  // BoardUtils::shiftBoardPosOfMovedPiece() is NOT added by Zhivko
    const BoardPos boardPosInit=piece->getBoardPos();
    if(Defines::BLACK_PLAYER_ID==currPlayerId){
        const BoardPos boardPosInv=PieceMoveAnimator::getInvBoardPosForAnim(boardPosInit,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        const Point absPosInv=PieceMoveAnimator::getAbsPosForAnim(boardPosInv);
        const BoardPos boardPosNew=PieceMoveAnimator::getBoardPosForAnim({absPosInv.x+GAME_X_POS_SHIFT,absPosInv.y+GAME_Y_POS_SHIFT});
        piece->setBoardPos(boardPosNew);
        piece->setWidgetFlip(WidgetFlip::NONE);
        return;
    }
    const Point absPosInit=PieceMoveAnimator::getAbsPosForAnim(boardPosInit);
    const BoardPos boardPosNew=PieceMoveAnimator::getBoardPosForAnim({absPosInit.x+GAME_X_POS_SHIFT,absPosInit.y+GAME_Y_POS_SHIFT});
    piece->setBoardPos(boardPosNew);
    return;
}

bool PieceMoveAnimator::doPiecesPosOverlap(const Point& targetPos, const Point& posTopLeft){ // BoardUtils::isInsideBorder() is NOT added by Zhivko
    const Rectangle border(targetPos.x,targetPos.y,_tileSize,_tileSize);
    const Point posTopRight(posTopLeft.x+_tileSize,posTopLeft.y);
    const Point posBottLeft(posTopLeft.x,posTopLeft.y+_tileSize);
    const Point posBottRight(posTopLeft.x+_tileSize,posTopLeft.y+_tileSize);
    return border.isPointInside(posTopLeft) || border.isPointInside(posTopRight) || 
           border.isPointInside(posBottLeft) || border.isPointInside(posBottRight);
}

Point PieceMoveAnimator::getAbsPosOfTakenPiece(const ChessPiece::PlayerPieces& pieces) { // BoardUtils::getAbsPosOfTakenPiece() is NOT added by Zhivko
    const size_t size=pieces.size();
    const int32_t playerId=pieces.front()->getPlayerId();
    uint32_t counter{0};
    for(size_t i=0;i<size;++i){
        if(pieces[i]->getIsTaken()){
            ++counter;
        }
    }

    if(Defines::WHITE_PLAYER_ID==playerId){
        const int32_t xPos1=_firstTilePosX+(_BOARD_SIZE-counter)*_tileSize;
        const int32_t xPosLimit=_firstTilePosX-_tileSize;
        const int32_t yPos1=_firstTilePosY-_tileSize;
        if(xPos1>=xPosLimit){
            return Point(xPos1,yPos1);
        }

        for(size_t i=0;i<size;++i){
            const BoardPos boardPos=pieces[i]->getBoardPos();
            const Point absPos=PieceMoveAnimator::getAbsPosForAnim(boardPos);
            if(yPos1==absPos.y){
                --counter;
            }
        }
        const int32_t xPos2=xPosLimit;
        const int32_t yPos2=counter*_tileSize;          
        return Point(xPos2,yPos2);
    }

    const int32_t xPos1=_firstTilePosX+(counter-1)*_tileSize;
    const int32_t xPosLimit=_firstTilePosX+_BOARD_SIZE*_tileSize;
    const int32_t yPos1=_firstTilePosY+_BOARD_SIZE*_tileSize;  

    if(xPosLimit>=xPos1){
        return Point(xPos1,yPos1);
    }

    for(size_t i=0;i<size;++i){
        const BoardPos boardPos=pieces[i]->getBoardPos();
        const Point absPos=PieceMoveAnimator::getAbsPosForAnim(boardPos);
        if(absPos.y==yPos1){
            --counter;
        }
    }
    const int32_t xPos2=_firstTilePosX+_BOARD_SIZE*_tileSize;
    const int32_t yPos2=_firstTilePosY+(_BOARD_SIZE-counter)*_tileSize;
    return Point(xPos2,yPos2);
}
