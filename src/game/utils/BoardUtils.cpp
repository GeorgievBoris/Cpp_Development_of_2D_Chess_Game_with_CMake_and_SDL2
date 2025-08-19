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

void BoardUtils::getBoardPosIfEnPassant(const std::unique_ptr<ChessPiece>& selectedPiece, const ChessPiece::PlayerPieces& enemyPieces,
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

void BoardUtils::getBoardPosIfCastling(const ChessPiece::PlayerPieces& pieces, const std::unique_ptr<ChessPiece>& piece,
                                            BoardPos& newBoardPos,
                                            std::pair<int32_t, BoardPos>& pair){ // BoardUtils::doCastling() is NOT added by Zhivko

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
            if(PieceType::ROOK!=currPiece->getPieceType() || currPiece->getIsTaken()){
                continue;
            }

            pair.second.row=pieceBoardPos.row;
            pair.first=counter;             

            if(newBoardPos.col>pieceBoardPos.col){
                if(0==currPiece->getBoardPos().col){
                    continue;
                }
                pair.second.col=kingColRight;
                return;
            }

            if(0!=currPiece->getBoardPos().col){
                continue;
            }

            pair.second.col=kingColLeft;        
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
    
        pair.first=static_cast<int32_t>(i);
        pair.second=pieces[i]->getBoardPos();
        0==pieceBoardPos.col ? (pair.second.col-=2, newBoardPos.col=pair.second.col+1) 
                             :  (pair.second.col+=2, newBoardPos.col=pair.second.col-1);        
        break;
    }
}

BoardPos BoardUtils::getKingBoardPos(const ChessPiece::PlayerPieces& pieces) {
    const size_t piecesNum=pieces.size();
    for(size_t i=0;i<piecesNum;++i){
        if(PieceType::KING==pieces[i]->getPieceType()){
            return pieces[i]->getBoardPos();
        }
    }
    std::cerr<<"Error, BoardUtils::getKingBoardPos() returns an invalid board position for king\n";
    return BoardPos(INVALID_RSRC_ID,INVALID_RSRC_ID);
}