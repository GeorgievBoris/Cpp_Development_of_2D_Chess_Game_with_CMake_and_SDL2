// Corresponding header
#include "game/pieces/types/King.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"
#include "game/pieces/types/Rook.h"

std::vector<MoveDirection> King::getBoardMoves() const {
    // Note: Probably can use std::unordered_map instead of std::vector ?
    constexpr int32_t kingMaxDirs=8;
    std::vector<MoveDirection> boardMoves(kingMaxDirs);

    constexpr std::array<Defines::Directions,kingMaxDirs> kingDirs {
        Defines::UP,
        Defines::UP_RIGHT,
        Defines::RIGHT,
        Defines::DOWN_RIGHT,
        Defines::DOWN,
        Defines::DOWN_LEFT,
        Defines::LEFT,
        Defines::UP_LEFT
    };

    BoardPos futurePos;
    for(int32_t i=0;i<kingMaxDirs;++i){
        futurePos=_boardPos;
        futurePos=BoardUtils::getAdjacentPos(kingDirs[i],futurePos);
        if(BoardUtils::isInsideBoard(futurePos)){
            boardMoves[i].push_back(futurePos);
        }
    }
    return boardMoves;
}

bool King::isCastlePossible(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers, 
                            const BoardPos& rookBoardPos) const {

    const int32_t opponentPlayerId=BoardUtils::getOpponentId(_playerId);
    const int32_t activePlayerNumOfPieces=static_cast<int32_t>(activePlayers[_playerId].size());
    const BoardPos kingBoardPos=ChessPiece::getBoardPos();

    int32_t columnMin=rookBoardPos.col;
    int32_t columnMax=kingBoardPos.col;

    if(columnMin>columnMax){
        const int32_t columnMinCopy=columnMin;
        columnMin=columnMax;
        columnMax=columnMinCopy;
    }

    for(int32_t j=0;j<activePlayerNumOfPieces;++j){
        const BoardPos currPiecePos=activePlayers[_playerId][j]->getBoardPos();

        if(kingBoardPos.row!=currPiecePos.row){
            continue;
        }

        if(currPiecePos.col<=columnMin || currPiecePos.col>=columnMax){
            continue;
        }
    
        return false;
    }  

    const int32_t opponentPlayerNumOfPieces=static_cast<int32_t>(activePlayers[opponentPlayerId].size());

    for(int32_t j=0;j<opponentPlayerNumOfPieces;++j){

        const BoardPos opponentPiecePos=activePlayers[opponentPlayerId][j]->getBoardPos();

        if(kingBoardPos.row!=opponentPiecePos.row){
            continue;
        }

        if(opponentPiecePos.col<columnMin || opponentPiecePos.col>columnMax){
            // not possible to have an opponent chess piece located at the position of your king
            continue;
        }
    
        return false;
    }

    for(int32_t j=0;j<opponentPlayerNumOfPieces;++j){
        const std::unique_ptr<ChessPiece>& enemyChessPiece=activePlayers[opponentPlayerId][j];   

        const std::vector<TileData> enemyPieceMoveTiles =enemyChessPiece->getMoveTiles(activePlayers);

        const int32_t sizeTileData=static_cast<int32_t>(enemyPieceMoveTiles.size());
        for(int32_t k=0;k<sizeTileData;++k){
            if(TileType::GUARD==enemyPieceMoveTiles[k].tileType){
                continue;
            }
            const BoardPos& tileDataPos=enemyPieceMoveTiles[k].boardPos;

            if(tileDataPos.row!=kingBoardPos.row){
                continue;
            }

            if(tileDataPos.col==rookBoardPos.col){
                continue;
            }            

            if(tileDataPos.col<columnMin || tileDataPos.col>columnMax){
                continue;
            }

            if((tileDataPos.col<(columnMax-2)) && (tileDataPos.col!=kingBoardPos.col)){
                continue;
            }                 
            return false;
        }
    }
    return true;
}

std::vector<TileData> King::getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const {
    const std::vector<MoveDirection> boardMoves=getBoardMoves();
    const size_t maxKingMoves=boardMoves.size();
    std::vector<TileData> moveTiles;
    moveTiles.reserve(maxKingMoves);

    const int32_t opponentId=BoardUtils::getOpponentId(_playerId);

    const bool isAnotherPieceGetMoveTilesCalled=ChessPiece::isGetMoveTilesCalled(activePlayers);
    if(!isAnotherPieceGetMoveTilesCalled){
        _isFncGetMoveTilesCalled=true;
    }
    
    TileType tileType;
    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }

        BoardPos pos=moveDir.front();
        tileType=BoardUtils::getTileType(pos,activePlayers[_playerId],activePlayers[opponentId]);

        if(TileType::GUARD==tileType){
            moveTiles.emplace_back(pos,tileType);
            continue;
        }

        if(isAnotherPieceGetMoveTilesCalled){
            moveTiles.emplace_back(pos,tileType);
            continue;
        }        

        if(TileType::TAKE==tileType){
            if(ChessPiece::isTakeTileValid(pos,_boardPos,activePlayers)){
                moveTiles.emplace_back(pos,tileType);
            }
            continue;      
        }        

        if(TileType::MOVE==tileType){
            if(ChessPiece::isMoveTileValid(pos,_boardPos,activePlayers)){
                moveTiles.emplace_back(pos,tileType);
            }      
        }
    }

    if(isAnotherPieceGetMoveTilesCalled){
        return moveTiles;
    }    

    if(_isMoved || _isInCheck) {
        _isCastlePossible=false;
        _isFncGetMoveTilesCalled=false;
        return moveTiles;
    }

    _isCastlePossible=false;

    for(const std::unique_ptr<ChessPiece>& piece:activePlayers[_playerId]){
        if(PieceType::ROOK!=piece->getPieceType()){
            continue;
        }

        const ChessPiece* const chessPiecePtr=piece.get();
        const Rook* const rookPtr=static_cast<const Rook*>(chessPiecePtr);
    
        if(rookPtr->isMoved() || rookPtr->getIsTaken()){
            continue;
        } 

        const BoardPos rookBoardPos=piece->getBoardPos();
        const bool isCastlingPossible=King::isCastlePossible(activePlayers,rookBoardPos);

        if(isCastlingPossible){
            _isCastlePossible=isCastlingPossible;
            0==rookBoardPos.col ? moveTiles.emplace_back(BoardPos(rookBoardPos.row,(rookBoardPos.col+2)),TileType::MOVE)
                                : moveTiles.emplace_back(BoardPos(rookBoardPos.row,(rookBoardPos.col-1)),TileType::MOVE);
        }
    }
    _isFncGetMoveTilesCalled=false;
    return moveTiles;
}

bool King::getIsCastlePossible() const{
    return _isCastlePossible;
}

bool King::isMoved() const{
    return _isMoved;  
}

bool King::isInCheck() const{
    return _isInCheck;
}

void King::setIsInCheck(bool isInCheck){
    _isInCheck=isInCheck;
}

void King::setWhenFirstMoveIsMade() {
    if(_isMoved){
        return;
    }
    Defines::WHITE_PLAYER_ID==_playerId ?
    _isMoved=_boardPos!=BoardPos(Defines::WHITE_PLAYER_START_PAWN_ROW+1,Defines::KING_STARTING_COLUMN_POSITION) :
    _isMoved=_boardPos!=BoardPos(Defines::BLACK_PLAYER_START_PAWN_ROW-1,Defines::KING_STARTING_COLUMN_POSITION);   
}