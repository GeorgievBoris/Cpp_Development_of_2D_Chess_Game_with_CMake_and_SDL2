// Corresponding header
#include "game/utils/BoardUtils.h"
// C system headers
// C++ system headers
#include <cstdlib>
#include <iostream>
// Third-party headers
// Own headers
#include "utils/drawing/Rectangle.h"

#include "game/pieces/types/Pawn.h" // NOT added by Zhivko
#include "game/pieces/types/Rook.h" // NOT added by Zhivko
#include "game/pieces/types/King.h"// NOT added by Zhivko

extern const int32_t GAME_X_POS_SHIFT;
extern const int32_t GAME_Y_POS_SHIFT;

// NOTE: if these four variables DO NOT need to go out of this source file / be visible outside of it...
// ... use them either in a "namespace" or set them as "static"

static constexpr auto BOARD_SIZE=8;
// static constexpr auto FIRST_TILE_X_POS=58; // used initially by Zhivko
// static constexpr auto FIRST_TILE_Y_POS=60; // used initially by Zhivko
static constexpr auto FIRST_TILE_X_POS=111; // not added by Zhivko
static constexpr auto FIRST_TILE_Y_POS=114; // not added by Zhivko
static constexpr auto TILE_SIZE=98; 


BoardPos BoardUtils::getBoardPos(const Point& absPos){
    return {(absPos.y-FIRST_TILE_Y_POS)/TILE_SIZE, (absPos.x-FIRST_TILE_X_POS)/TILE_SIZE};
}

Point BoardUtils::getAbsPos(const BoardPos& boardPos){
    return {(boardPos.col*TILE_SIZE)+FIRST_TILE_X_POS, (boardPos.row*TILE_SIZE)+FIRST_TILE_Y_POS};
}

BoardPos BoardUtils::getInvertedBoardPos(const BoardPos& boardPos, WidgetFlip flipType){ // NOT added by Zhivko
    switch(flipType){
    case WidgetFlip::NONE:
        return boardPos; // return BoardPos(boardPos.row,boardPos.col);
    case WidgetFlip::HORIZONTAL:
        return BoardPos((BOARD_SIZE-1)-boardPos.row,boardPos.col);
    case WidgetFlip::VERTICAL:
        return BoardPos(boardPos.row,(BOARD_SIZE-1)-boardPos.col);
    case WidgetFlip::HORIZONTAL_AND_VERTICAL:
        return BoardPos((BOARD_SIZE-1)-boardPos.row,(BOARD_SIZE-1)-boardPos.col);
    }

    std::cerr<<"Error, BoardUtils::getInvertedBoardPos() encountered unknown WidgetFlip type!"<<std::endl;
    return BoardPos(0,0);
}

Point BoardUtils::getCenterPos(const int32_t& width, const int32_t& height){ // NOT added by Zhivko
    return Point((TILE_SIZE-width)/2,(TILE_SIZE-height)/2);
}

bool BoardUtils::isInsideBoard(const BoardPos& boardPos){

    const Rectangle bound{0, 0, BOARD_SIZE, BOARD_SIZE};
    const Point pos {boardPos.col, boardPos.row};

    return bound.isPointInside(pos);
}

bool BoardUtils::isInsideBoard(const Point& absPos){

    const Rectangle bound(FIRST_TILE_X_POS, FIRST_TILE_Y_POS, BOARD_SIZE*TILE_SIZE, BOARD_SIZE*TILE_SIZE);

    return bound.isPointInside(absPos);
}

int32_t BoardUtils::getOpponentId(int32_t activePlayerId){
    if(Defines::WHITE_PLAYER_ID==activePlayerId){
        return Defines::BLACK_PLAYER_ID;
    }
    return Defines::WHITE_PLAYER_ID;
}

BoardPos BoardUtils::getAdjacentPos(Defines::Directions dir,const BoardPos& currPos){
    BoardPos pos=currPos;
    switch(dir){
    case Defines::UP_LEFT:
        --pos.row;
        --pos.col;
        break;
    case Defines::UP:
        --pos.row;
        break;
    case Defines::UP_RIGHT:
        --pos.row;
        ++pos.col;
        break;
    case Defines::RIGHT:
        ++pos.col;
        break;
    case Defines::DOWN_RIGHT:
        ++pos.row;
        ++pos.col;
        break;
    case Defines::DOWN:
        ++pos.row;
        break;
    case Defines::DOWN_LEFT:
        ++pos.row;
        --pos.col;
        break;
    case Defines::LEFT:
        --pos.col;
        break;
    default:
        std::cerr<<"Received invalid dir: "<<dir<<std::endl;
        break;
    }
    return pos;
}

bool BoardUtils::doCollideWithPiece(const BoardPos& selectedPos, const ChessPiece::PlayerPieces& pieces, int32_t& outCollisionRelativeId){
    const size_t size=pieces.size();
    for(size_t i=0;i<size;++i){
        if(pieces[i]->getBoardPos()==selectedPos){
             // important ... write this down in the book, if "outCollisionRelativeId" was of type "int32_t*" !!!
            // *outCollisionRelativeId=static_cast<int32_t>(i);
            outCollisionRelativeId=static_cast<int32_t>(i);
            return true;
        }
    }

    return false;
}

TileType BoardUtils::getTileType(const BoardPos& boardPos, const ChessPiece::PlayerPieces& playerPieces, const ChessPiece::PlayerPieces& enemyPieces){
    int32_t collisionIdx=-1;
    if(doCollideWithPiece(boardPos,playerPieces,collisionIdx)){
        return TileType::GUARD;
    }

    if(doCollideWithPiece(boardPos,enemyPieces,collisionIdx)){
        return TileType::TAKE;
    }

    return TileType::MOVE;
}

BoardPos BoardUtils::shiftBoardPositions(const BoardPos& boardPos) { //BoardUtils::shiftBoardPositions() is NOT added by Zhivko
    const Point absPos=BoardUtils::getAbsPos(boardPos);
    return BoardUtils::getBoardPos(Point(GAME_X_POS_SHIFT+absPos.x,GAME_Y_POS_SHIFT+absPos.y));
}

void BoardUtils::checkForEnPassant(const std::unique_ptr<ChessPiece>& selectedPiece, const ChessPiece::PlayerPieces& enemyPieces,
                                        BoardPos& boardPos, int32_t& outCollisionRelativeId){ // BoardUtils::checkForEnPassant() is NOT added by Zhivko  
    
    if(PieceType::PAWN!=selectedPiece->getPieceType()){
        return;
    }

    const BoardPos& selectedPieceBoardPos=selectedPiece->getBoardPos();
    const int32_t currPlayerId=selectedPiece->getPlayerId();

    constexpr Defines::PawnDefines enPassantRows[Defines::PLAYERS_COUNT] {Defines::WHITE_PLAYER_EN_PASSANT_ROW, Defines::BLACK_PLAYER_EN_PASSANT_ROW};

    if(enPassantRows[currPlayerId]!=selectedPieceBoardPos.row){
        return;
    }

    int32_t i=-1;
    
    for(const std::unique_ptr<ChessPiece>& piece:enemyPieces){
        ++i;

        if(PieceType::PAWN!=piece->getPieceType()){
            continue;
        }
     
        if(enPassantRows[currPlayerId]!=piece->getBoardPos().row){
            continue;
        }

        const ChessPiece* const chessPiecePtr=piece.get();
        const Pawn* const pawnPtr=static_cast<const Pawn*>(chessPiecePtr);

        if(!pawnPtr->isPawnTargetedForEnPassant()){
            continue;
        }
        
        if(selectedPieceBoardPos.row!=boardPos.row){
            Defines::WHITE_PLAYER_ID==currPlayerId ? boardPos=BoardUtils::getAdjacentPos(Defines::DOWN,boardPos) : 
                                                    boardPos=BoardUtils::getAdjacentPos(Defines::UP,boardPos);
        }

        if(piece->getBoardPos()==boardPos){
            outCollisionRelativeId=i;
        }

        Defines::WHITE_PLAYER_ID==currPlayerId ? boardPos=BoardUtils::getAdjacentPos(Defines::UP,boardPos) : 
                                                boardPos=BoardUtils::getAdjacentPos(Defines::DOWN,boardPos);            


        if(outCollisionRelativeId==i){
            break;
        }       
    }
}

void BoardUtils::checkForCastling(const ChessPiece::PlayerPieces& pieces, const std::unique_ptr<ChessPiece>& piece,
                                            BoardPos& newBoardPos,
                                            std::pair<bool, std::pair<int32_t, BoardPos>>& pair){ // BoardUtils::doCastling() is NOT added by Zhivko

    const PieceType& pieceType=piece->getPieceType();

    const BoardPos pieceBoardPos=piece->getBoardPos();

    if(pieceBoardPos.row!=newBoardPos.row){
        return;
    }    

    if(PieceType::KING==pieceType){

        const int32_t kingColRight=pieceBoardPos.col+1;
        const int32_t kingColLeft=pieceBoardPos.col-1;

        if(kingColRight>=newBoardPos.col && kingColLeft<=newBoardPos.col){
            return;
        }

        int32_t counter=-1;
        for(const std::unique_ptr<ChessPiece>& currPiece:pieces){
            ++counter;
            if(PieceType::ROOK!=currPiece->getPieceType()){
                continue;
            }

            pair.second.second.row=pieceBoardPos.row;
            pair.second.first=counter;
            pair.first=true;                

            if(newBoardPos.col>pieceBoardPos.col){
                if(0==currPiece->getBoardPos().col){
                    continue;
                }
                pair.second.second.col=kingColRight;
                return;
            }

            if(0!=currPiece->getBoardPos().col){
                continue;
            }

            pair.second.second.col=kingColLeft;        
            return;
        }
        return;
    } // the logic for the KING ends here

    for(size_t i=0;i<pieces.size();++i){
        if(PieceType::KING!=pieces[i]->getPieceType()){
            continue;
        }

        if(pieces[i]->getBoardPos()!=newBoardPos){
            return;
        }
    
        pair.second.first=static_cast<int32_t>(i);
        pair.second.second=pieces[i]->getBoardPos();
        pair.first=true;
        0==pieceBoardPos.col ? (pair.second.second.col-=2, newBoardPos.col=pair.second.second.col+1) 
                             :  (pair.second.second.col+=2, newBoardPos.col=pair.second.second.col-1);        
        break;
    }
}

Point BoardUtils::getPosOfMovedPiece(const Point& currPos, const Point& targetPos, const PieceType pieceType){ // NOT added by Zhivko

    Point currAbsPos=currPos;
    switch(pieceType)
    {
    case PieceType::KNIGHT:
        {
        BoardUtils::moveDeltaX(currAbsPos,targetPos);
        if(targetPos.x==currAbsPos.x){
            BoardUtils::moveDeltaY(currAbsPos,targetPos);
        }
        return currAbsPos;
        }
    default:
        {
        BoardUtils::moveDeltaX(currAbsPos,targetPos);
        BoardUtils::moveDeltaY(currAbsPos,targetPos);
        return currAbsPos;
        }
    }
    std::cerr<<"Error, returning zero for the animated position of PieceType: "<<static_cast<uint32_t>(pieceType)<<std::endl;
    return Point::ZERO;
}

void BoardUtils::moveDeltaX(Point& currAbsPos,const Point& targetAbsPos){ // BoardUtils::moveDeltaX() is NOT added by Zhivko
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

void BoardUtils::moveDeltaY(Point& currAbsPos,const Point& targetAbsPos){ // BoardUtils::moveDeltaY() is NOT added by Zhivko 
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

BoardPos BoardUtils::getBoardPosForAnim(const Point& absPos){ // BoardUtils::getBoardPosForAnim() is NOT added by Zhivko
    return {(absPos.y-FIRST_TILE_Y_POS)/TILE_SIZE, (absPos.x-FIRST_TILE_X_POS)/TILE_SIZE,
            (absPos.y-FIRST_TILE_Y_POS)%TILE_SIZE, (absPos.x-FIRST_TILE_X_POS)%TILE_SIZE};
}
Point BoardUtils::getAbsPosForAnim(const BoardPos& boardPos){ // BoardUtils::getAbsPosForAnim() is NOT added by Zhivko
    return {(boardPos.col*TILE_SIZE)+FIRST_TILE_X_POS+boardPos.remCol, (boardPos.row*TILE_SIZE)+FIRST_TILE_Y_POS+boardPos.remRow};
}

BoardPos BoardUtils::getInvBoardPosForAnim(const BoardPos& boardPos, WidgetFlip flipType){ // BoardUtils::getAnimatedInvertedBoardPos() is NOT added by Zhivko
    switch(flipType){
    case WidgetFlip::NONE:
        return boardPos; // return BoardPos(boardPos.row,boardPos.col);
    case WidgetFlip::HORIZONTAL:
        return BoardPos((BOARD_SIZE-1)-boardPos.row,boardPos.col,boardPos.remRow,boardPos.remCol);
    case WidgetFlip::VERTICAL:
        return BoardPos(boardPos.row,(BOARD_SIZE-1)-boardPos.col,boardPos.remRow,boardPos.remCol);
    case WidgetFlip::HORIZONTAL_AND_VERTICAL:
        return BoardPos((BOARD_SIZE-1)-boardPos.row,(BOARD_SIZE-1)-boardPos.col,boardPos.remRow,boardPos.remCol);
    }

    std::cerr<<"Error, BoardUtils::getInvertedBoardPos() encountered unknown WidgetFlip type!"<<std::endl;
    return BoardPos(0,0,0,0);
}

void BoardUtils::shiftBoardPosOfMovedPiece(std::unique_ptr<ChessPiece>& piece,const int32_t currPlayerId, BoardPos& targetBoardPos) {  // BoardUtils::shiftBoardPosOfMovedPiece() is NOT added by Zhivko
    const BoardPos boardPosInit=piece->getBoardPos();
    if(Defines::BLACK_PLAYER_ID==currPlayerId){
        const BoardPos boardPosInv=BoardUtils::getInvBoardPosForAnim(boardPosInit,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        const Point absPosInv=BoardUtils::getAbsPosForAnim(boardPosInv);
        const BoardPos boardPosNew=BoardUtils::getBoardPosForAnim({absPosInv.x+GAME_X_POS_SHIFT,absPosInv.y+GAME_Y_POS_SHIFT});
        piece->setBoardPos(boardPosNew);
        piece->setWidgetFlip(WidgetFlip::NONE);
        if(Defines::BLACK_PLAYER_ID==piece->getPlayerId()){
            targetBoardPos=BoardUtils::getInvBoardPosForAnim(targetBoardPos,WidgetFlip::HORIZONTAL_AND_VERTICAL);
        }
        return;
    }
    const Point absPosInit=BoardUtils::getAbsPosForAnim(boardPosInit);
    const BoardPos boardPosNew=BoardUtils::getBoardPosForAnim({absPosInit.x+GAME_X_POS_SHIFT,absPosInit.y+GAME_Y_POS_SHIFT});
    piece->setBoardPos(boardPosNew);
    return;
}

bool BoardUtils::doPiecesPosOverlap(const Point& targetPos, const Point& posTopLeft){ // BoardUtils::isInsideBorder() is NOT added by Zhivko
    const Rectangle border(targetPos.x,targetPos.y,TILE_SIZE,TILE_SIZE);
    const Point posTopRight(posTopLeft.x+TILE_SIZE,posTopLeft.y);
    const Point posBottLeft(posTopLeft.x,posTopLeft.y+TILE_SIZE);
    const Point posBottRight(posTopLeft.x+TILE_SIZE,posTopLeft.y+TILE_SIZE);
    return border.isPointInside(posTopLeft) || border.isPointInside(posTopRight) || 
           border.isPointInside(posBottLeft) || border.isPointInside(posBottRight);
}

Point BoardUtils::getAbsPosOfTakenPiece(const ChessPiece::PlayerPieces& pieces) { // BoardUtils::getAbsPosOfTakenPiece() is NOT added by Zhivko
    const size_t size=pieces.size();
    const int32_t playerId=pieces.front()->getPlayerId();
    uint32_t counter{0};
    for(size_t i=0;i<size;++i){
        if(pieces[i]->getIsTaken()){
            ++counter;
        }
    }

    if(Defines::WHITE_PLAYER_ID==playerId){
        const int32_t xPos1=FIRST_TILE_X_POS+(BOARD_SIZE-counter)*TILE_SIZE;
        const int32_t xPosLimit=FIRST_TILE_X_POS-TILE_SIZE;
        const int32_t yPos1=FIRST_TILE_Y_POS-TILE_SIZE;
        if(xPos1>=xPosLimit){
            return Point(xPos1,yPos1);
        }

        for(size_t i=0;i<size;++i){
            const BoardPos boardPos=pieces[i]->getBoardPos();
            const Point absPos=BoardUtils::getAbsPosForAnim(boardPos);
            if(yPos1==absPos.y){
                --counter;
            }
        }
        const int32_t xPos2=xPosLimit;
        const int32_t yPos2=counter*TILE_SIZE;          
        return Point(xPos2,yPos2);
    }

    const int32_t xPos1=FIRST_TILE_X_POS+(counter-1)*TILE_SIZE;
    const int32_t xPosLimit=FIRST_TILE_X_POS+BOARD_SIZE*TILE_SIZE;
    const int32_t yPos1=FIRST_TILE_Y_POS+BOARD_SIZE*TILE_SIZE;  

    if(xPosLimit>=xPos1){
        return Point(xPos1,yPos1);
    }

    for(size_t i=0;i<size;++i){
        const BoardPos boardPos=pieces[i]->getBoardPos();
        const Point absPos=BoardUtils::getAbsPosForAnim(boardPos);
        if(absPos.y==yPos1){
            --counter;
        }
    }
    const int32_t xPos2=FIRST_TILE_X_POS+BOARD_SIZE*TILE_SIZE;
    const int32_t yPos2=FIRST_TILE_Y_POS+(BOARD_SIZE-counter)*TILE_SIZE;
    return Point(xPos2,yPos2);
}