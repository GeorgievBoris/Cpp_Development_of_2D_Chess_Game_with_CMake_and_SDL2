// Corresponding header
#include "game/pieces/types/Rook.h"
// C system headers
// C++ system headers
#include <iostream>
// Third-party headers
// Own headers
#include "game/utils/BoardUtils.h"

std::vector<MoveDirection> Rook::getBoardMoves() const{
    constexpr auto allowedDirs=4;
    constexpr std::array<Defines::Directions,allowedDirs> rookDirs {Defines::UP, 
                                                Defines::RIGHT, Defines::DOWN, Defines::LEFT};
    constexpr auto maxRookMoves=7;
    std::vector<MoveDirection> boardMoves(allowedDirs);

    BoardPos futurePos;
    for(int32_t dirIdx=0;dirIdx<allowedDirs;++dirIdx){
        boardMoves[dirIdx].reserve(maxRookMoves);
        futurePos=_boardPos;
        futurePos=BoardUtils::getAdjacentPos(rookDirs[dirIdx],futurePos);
        while(BoardUtils::isInsideBoard(BoardUtils::getAbsPos(futurePos))){
            boardMoves[dirIdx].push_back(futurePos);
            futurePos=BoardUtils::getAdjacentPos(rookDirs[dirIdx],futurePos);
        }
    }
    return boardMoves;
}

std::vector<TileData> Rook::getMoveTiles(const std::array<ChessPiece::PlayerPieces,
                                            Defines::PLAYERS_COUNT>& activePlayers) const{
    const std::vector<MoveDirection> boardMoves=getBoardMoves();

    std::vector<TileData> moveTiles;
    const auto opponentId=BoardUtils::getOpponentId(_playerId);

    TileData tileData;

    for(const MoveDirection& moveDir:boardMoves){
        if(moveDir.empty()){
            continue;
        }
        for(const BoardPos& boardPos:moveDir){
            const TileType tileType=BoardUtils::getTileType(boardPos,activePlayers[_playerId],activePlayers[opponentId]);
            tileData.tileType=tileType;
            tileData.boardPos=boardPos;
            moveTiles.push_back(tileData);
            if(TileType::MOVE!=tileType){
                break;
            }
        }
    }

    return moveTiles;
    
}