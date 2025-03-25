// Corresponding header
#include "game/utils/BoardUtils.h"
// C system headers
// C++ system headers
#include <cstdlib>
#include <iostream>
// Third-party headers
// Own headers
#include "utils/drawing/Rectangle.h"

// NOTE: if these four variables DO NOT need to go out of this source file / be visible outside of it...
// ... use them either in a "namespace" or set them as "static"

static constexpr auto BOARD_SIZE=8;
static constexpr auto FIRST_TILE_X_POS=58;
static constexpr auto FIRST_TILE_Y_POS=60;
static constexpr auto TILE_SIZE=98;


BoardPos BoardUtils::getBoardPos(const Point& absPos){
    return {(absPos.y-FIRST_TILE_Y_POS)/TILE_SIZE, (absPos.x-FIRST_TILE_X_POS)/TILE_SIZE};
}

Point BoardUtils::getAbsPos(const BoardPos& boardPos){
    return {(boardPos.col*TILE_SIZE)+FIRST_TILE_X_POS, (boardPos.row*TILE_SIZE)+FIRST_TILE_Y_POS};
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

bool BoardUtils::doCollideWithPiece(const BoardPos& selectedPos, const ChessPiece::PlayerPieces& pieces,
                                            int32_t& outCollisionRelativeId){
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

TileType BoardUtils::getTileType(const BoardPos& boardPos, const ChessPiece::PlayerPieces& playerPieces,
                                                            const ChessPiece::PlayerPieces& enemyPieces){
    int32_t collisionIdx=-1;
    if(doCollideWithPiece(boardPos,playerPieces,collisionIdx)){
        return TileType::GUARD;
    }

    if(doCollideWithPiece(boardPos,enemyPieces,collisionIdx)){
        return TileType::TAKE;
    }

    return TileType::MOVE;
}

void BoardUtils::checkForEnPassant(const std::unique_ptr<ChessPiece>& selectedPiece, const ChessPiece::PlayerPieces& enemyPieces,
                                        BoardPos& boardPos, int32_t& outCollisionRelativeId){ // BoardUtils::checkForEnPassant() method is NOT added by Zhivko  
    
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

        if(!piece->isPieceTheLastMovedPiece()){
            continue;
        }

        if(!piece->isPieceFirstMoveNow()){
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
