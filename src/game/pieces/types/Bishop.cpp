// Corresponding header
#include "game/pieces/types/Bishop.h"
// C system headers
// C++ system headers
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

std::vector<MoveDirection> Bishop::getBoardMoves() const{
    constexpr int32_t allowedDirs=4; // check what will hapen if we have -> size_t directionsCount=-4; ... is it going to be a very, very, very big number ???
    constexpr std::array<Defines::Directions,allowedDirs> bishopDirs {Defines::UP_LEFT, Defines::DOWN_RIGHT,Defines::UP_RIGHT, Defines::DOWN_LEFT};

    constexpr int32_t maxMovesPerDir=7;
    std::vector<MoveDirection> boardMoves(allowedDirs);

    BoardPos currPos;
    for (int32_t i=0;i<allowedDirs;++i){
        currPos=_boardPos;
        boardMoves[i].reserve(maxMovesPerDir);
        currPos=BoardUtils::getAdjacentPos(bishopDirs[i],currPos);
        while(BoardUtils::isInsideBoard(currPos)){
            boardMoves[i].push_back(currPos);
            currPos=BoardUtils::getAdjacentPos(bishopDirs[i],currPos);
        }
    }
    return boardMoves;
}

std::vector<TileData> Bishop::getMoveTiles(const std::array<ChessPiece::PlayerPieces,Defines::PLAYERS_COUNT>& activePlayers) const{
    if(_isTaken){
        return std::vector<TileData>();
    }
    constexpr int32_t maxTilesCount=14;
    std::vector<TileData> moveTiles;
    moveTiles.reserve(maxTilesCount);

    const std::vector<MoveDirection> boardMoves=getBoardMoves();

    const int32_t activePlayerId=getPlayerId();
    const int32_t opponentId=BoardUtils::getOpponentId(activePlayerId);

    const bool isAnotherPieceGetMoveTilesCalled=ChessPiece::isGetMoveTilesCalled(activePlayers);
    BoardPos kingBoardPos;
    if(!isAnotherPieceGetMoveTilesCalled){
        kingBoardPos=BoardUtils::getKingBoardPos(activePlayers[_playerId]);
        _isFncGetMoveTilesCalled=true;
    }

    TileType tileType;
    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }
        for(const BoardPos& pos:moveDir){
            tileType=BoardUtils::getTileType(pos,activePlayers[activePlayerId],activePlayers[opponentId]);

            if(TileType::GUARD==tileType){
                moveTiles.emplace_back(pos,tileType);
                break;
            }

            if(isAnotherPieceGetMoveTilesCalled){
                moveTiles.emplace_back(pos,tileType);
                if(TileType::MOVE!=tileType){
                    break;
                }                
                continue;
            }            

            if(TileType::TAKE==tileType){
                if(ChessPiece::isTakeTileValid(pos,kingBoardPos,activePlayers)){
                    moveTiles.emplace_back(pos,tileType);
                }
                break;      
            }        

            if(TileType::MOVE==tileType){
                if(ChessPiece::isMoveTileValid(pos,kingBoardPos,activePlayers)){
                    moveTiles.emplace_back(pos,tileType);
                }
            }
        }
    }
    _isFncGetMoveTilesCalled=false;
    return moveTiles;
}